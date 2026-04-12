/**
 * main_window.cpp -- OBYS v3 ana pencere uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * Unite 2'deki mantik korunur, merkez widget QTabWidget olarak
 * yeniden yapilandirilir. Sekmeler:
 *   0) Notlar    -- QTableView + NotlarModel + "Not Ekle" butonu
 *   1) Grafikler -- GrafikPaneli (IstatistikModel'e bagli bar chart)
 *   2) Trend     -- Sahte donem verisiyle QLineSeries trend grafigi
 *
 * NotlarModel::yenile() cagrildiginda modelReset yayilir;
 * IstatistikModel bu sinyali dinleyerek histogrami bastan hesaplar ve
 * QVBarModelMapper grafigi otomatik olarak yeniler.
 */

#include "main_window.h"

#include "ders_raporu.h"
#include "grafik_paneli.h"
#include "istatistik_model.h"
#include "not_giris_dialog.h"
#include "notlar_model.h"
#include "obys_domain.h"
#include "transkript_uretici.h"

#include <QAction>
#include <QCloseEvent>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTableView>
#include <QTabWidget>
#include <QTextDocument>
#include <QVBoxLayout>
#include <QWidget>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>

#include <array>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    merkeziWidgetKur();
    menuleriKur();

    m_servis.ornekVerileriYukle();

    // Baslangic icin birkac ornek not ekle (histogram bos kalmasin)
    m_servis.dersKaydiEkle({1, 10, SayisalNot{92.0}});
    m_servis.dersKaydiEkle({2, 10, SayisalNot{68.0}});
    m_servis.dersKaydiEkle({3, 10, SayisalNot{55.0}});
    m_servis.dersKaydiEkle({1, 11, SayisalNot{85.0}});
    m_servis.dersKaydiEkle({2, 11, SayisalNot{44.0}});

    m_notlarModel->yenile();

    m_aktifDosya.clear();
    m_dirty = false;
    basligiGuncelle();

    statusBar()->showMessage(tr("Hazır"), 2000);
    resize(840, 560);
}

QWidget* MainWindow::notlarSekmesiKur() {
    auto* sayfa    = new QWidget(this);
    auto* yerlesim = new QVBoxLayout(sayfa);

    m_notTablosu  = new QTableView(sayfa);
    m_notlarModel = new NotlarModel(&m_servis, this);
    m_notTablosu->setModel(m_notlarModel);
    m_notTablosu->horizontalHeader()->setStretchLastSection(true);
    m_notTablosu->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_notTablosu->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_notEkleButonu = new QPushButton(tr("Not Ekle..."), sayfa);

    yerlesim->addWidget(m_notTablosu);
    yerlesim->addWidget(m_notEkleButonu);

    connect(m_notEkleButonu, &QPushButton::clicked,
            this, &MainWindow::notEkleDialogAc);

    return sayfa;
}

QWidget* MainWindow::trendSekmesiKur() {
    // Sahte donem verisiyle cizgi grafik
    const std::array<QDateTime, 4> tarihler{
        QDateTime(QDate(2023, 9,  15), QTime(0, 0)),
        QDateTime(QDate(2024, 2,  15), QTime(0, 0)),
        QDateTime(QDate(2024, 9,  15), QTime(0, 0)),
        QDateTime(QDate(2025, 2,  15), QTime(0, 0)),
    };

    struct OgrenciTrend {
        QString           isim;
        std::array<double, 4> notlar;
    };
    const std::array<OgrenciTrend, 3> veri{{
        {QStringLiteral("Ayse Yilmaz"),   {72.0, 78.5, 84.0, 88.5}},
        {QStringLiteral("Mehmet Demir"),  {65.0, 68.0, 63.5, 70.0}},
        {QStringLiteral("Zeynep Kaya"),   {90.0, 92.0, 85.0, 93.0}},
    }};

    auto* grafik = new QChart();
    grafik->setTitle(QStringLiteral("Donemlik Ortalama Trendleri"));

    for (const auto& o : veri) {
        auto* seri = new QLineSeries();
        seri->setName(o.isim);
        for (int i = 0; i < 4; ++i) {
            seri->append(
                static_cast<double>(
                    tarihler[i].toMSecsSinceEpoch()),
                o.notlar[i]);
        }
        seri->setPointsVisible(true);
        seri->setPointLabelsVisible(true);
        seri->setPointLabelsFormat(QStringLiteral("@yPoint"));
        grafik->addSeries(seri);
    }

    auto* xEksen = new QDateTimeAxis();
    xEksen->setFormat(QStringLiteral("MMM yyyy"));
    xEksen->setTitleText(QStringLiteral("Donem"));
    xEksen->setTickCount(4);
    xEksen->setRange(tarihler.front(), tarihler.back());
    grafik->addAxis(xEksen, Qt::AlignBottom);

    auto* yEksen = new QValueAxis();
    yEksen->setRange(0, 100);
    yEksen->setTickCount(11);
    yEksen->setTitleText(QStringLiteral("Ortalama"));
    grafik->addAxis(yEksen, Qt::AlignLeft);

    for (auto* s : grafik->series()) {
        s->attachAxis(xEksen);
        s->attachAxis(yEksen);
    }

    grafik->legend()->setVisible(true);
    grafik->legend()->setAlignment(Qt::AlignBottom);

    auto* gorunum = new QChartView(grafik, this);
    gorunum->setRenderHint(QPainter::Antialiasing);
    return gorunum;
}

void MainWindow::merkeziWidgetKur() {
    m_sekmeler = new QTabWidget(this);

    // 0) Notlar sekmesi
    m_sekmeler->addTab(notlarSekmesiKur(), tr("Notlar"));

    // 1) Grafikler sekmesi -- bar chart
    m_grafikPanel     = new GrafikPaneli(this);
    m_istatistikModel = new IstatistikModel(3, this);
    m_istatistikModel->setKaynakModel(m_notlarModel);
    m_grafikPanel->setModel(m_istatistikModel);
    m_sekmeler->addTab(m_grafikPanel, tr("Grafikler"));

    // 2) Trend sekmesi -- line chart
    m_sekmeler->addTab(trendSekmesiKur(), tr("Trend"));

    setCentralWidget(m_sekmeler);
}

void MainWindow::menuleriKur() {
    auto* dosyaMenu = menuBar()->addMenu(tr("&Dosya"));

    m_yeniAction = dosyaMenu->addAction(tr("&Yeni"));
    m_yeniAction->setShortcut(QKeySequence::New);
    connect(m_yeniAction, &QAction::triggered,
            this, &MainWindow::dosyaYeni);

    m_acAction = dosyaMenu->addAction(tr("&Aç..."));
    m_acAction->setShortcut(QKeySequence::Open);
    connect(m_acAction, &QAction::triggered,
            this, &MainWindow::dosyaAc);

    dosyaMenu->addSeparator();

    m_kaydetAction = dosyaMenu->addAction(tr("&Kaydet"));
    m_kaydetAction->setShortcut(QKeySequence::Save);
    connect(m_kaydetAction, &QAction::triggered,
            this, [this]() { (void)dosyaKaydet(); });

    m_farkliKaydetAction =
        dosyaMenu->addAction(tr("&Farklı Kaydet..."));
    m_farkliKaydetAction->setShortcut(QKeySequence::SaveAs);
    connect(m_farkliKaydetAction, &QAction::triggered,
            this, [this]() { (void)dosyaFarkliKaydet(); });

    dosyaMenu->addSeparator();

    m_transkriptAction =
        dosyaMenu->addAction(tr("Transkript Ya&zdır..."));
    m_transkriptAction->setShortcut(QKeySequence::Print);
    connect(m_transkriptAction, &QAction::triggered,
            this, &MainWindow::transkriptYazdir);

    m_topluRaporAction =
        dosyaMenu->addAction(tr("Toplu &Rapor..."));
    connect(m_topluRaporAction, &QAction::triggered,
            this, &MainWindow::topluRaporYazdir);

    dosyaMenu->addSeparator();

    m_cikisAction = dosyaMenu->addAction(tr("Çı&kış"));
    m_cikisAction->setShortcut(QKeySequence::Quit);
    connect(m_cikisAction, &QAction::triggered,
            this, &MainWindow::close);
}

void MainWindow::basligiGuncelle() {
    const QString ad = m_aktifDosya.isEmpty()
        ? tr("İsimsiz")
        : QFileInfo(m_aktifDosya).fileName();
    const QString yildiz = m_dirty
        ? QStringLiteral(" *")
        : QString();
    setWindowTitle(QStringLiteral("OBYS v3 - %1%2").arg(ad, yildiz));
}

void MainWindow::dirtyYap(bool d) {
    m_dirty = d;
    basligiGuncelle();
}

void MainWindow::dosyaYeni() {
    if (!kaydetmeSorusu()) { return; }
    m_servis.ornekVerileriYukle();
    m_notlarModel->yenile();
    m_aktifDosya.clear();
    m_dirty = false;
    basligiGuncelle();
    statusBar()->showMessage(tr("Yeni oturum açıldı"), 2000);
}

void MainWindow::dosyaAc() {
    if (!kaydetmeSorusu()) { return; }
    const QString yol = QFileDialog::getOpenFileName(
        this, tr("OBYS Dosyası Aç"), QString(),
        tr("OBYS Dosyaları (*.obys);;Tüm Dosyalar (*)"));
    if (yol.isEmpty()) { return; }
    if (!m_persist.yukle(yol, m_servis)) {
        QMessageBox::warning(
            this, tr("Açma Hatası"), m_persist.sonHata());
        return;
    }
    m_notlarModel->yenile();
    m_aktifDosya = yol;
    m_dirty = false;
    basligiGuncelle();
    statusBar()->showMessage(tr("Dosya açıldı: %1").arg(yol), 3000);
}

bool MainWindow::dosyaKaydet() {
    if (m_aktifDosya.isEmpty()) { return dosyaFarkliKaydet(); }
    if (!m_persist.kaydet(m_aktifDosya, m_servis)) {
        QMessageBox::warning(
            this, tr("Kaydetme Hatası"), m_persist.sonHata());
        return false;
    }
    m_dirty = false;
    basligiGuncelle();
    statusBar()->showMessage(
        tr("Kaydedildi: %1").arg(m_aktifDosya), 3000);
    return true;
}

bool MainWindow::dosyaFarkliKaydet() {
    QString yol = QFileDialog::getSaveFileName(
        this, tr("Farklı Kaydet"), QStringLiteral("oturum.obys"),
        tr("OBYS Dosyaları (*.obys)"));
    if (yol.isEmpty()) { return false; }
    if (!yol.endsWith(QStringLiteral(".obys"),
                      Qt::CaseInsensitive)) {
        yol += QStringLiteral(".obys");
    }
    if (!m_persist.kaydet(yol, m_servis)) {
        QMessageBox::warning(
            this, tr("Kaydetme Hatası"), m_persist.sonHata());
        return false;
    }
    m_aktifDosya = yol;
    m_dirty = false;
    basligiGuncelle();
    statusBar()->showMessage(tr("Kaydedildi: %1").arg(yol), 3000);
    return true;
}

void MainWindow::notEkleDialogAc() {
    NotGirisDialog dlg(this);

    QList<QPair<int, QString>> ogrenciler;
    for (const auto& o : m_servis.tumOgrenciler()) {
        ogrenciler.append({o.id, o.adSoyad});
    }
    QList<QPair<int, QString>> dersler;
    for (const auto& d : m_servis.tumDersler()) {
        dersler.append(
            {d.id, QStringLiteral("%1 %2").arg(d.kod, d.ad)});
    }
    dlg.ogrencileriYukle(ogrenciler);
    dlg.dersleriYukle(dersler);

    if (dlg.exec() == QDialog::Accepted) {
        const NotDTO dto = dlg.notDTO();
        m_servis.notEkle(dto);
        m_notlarModel->yenile();
        dirtyYap(true);
        statusBar()->showMessage(tr("Not eklendi"), 2000);
    }
}

void MainWindow::belgeyiYazdirmaAkisina(
    QTextDocument* belge,
    const QString& varsayilanPdfAd)
{
    if (belge == nullptr) { return; }

    QMessageBox kutu(this);
    kutu.setWindowTitle(tr("Rapor Çıkışı"));
    kutu.setText(tr("Rapor nasıl üretilsin?"));
    auto* onizlemeBtn = kutu.addButton(
        tr("Önizleme"),  QMessageBox::AcceptRole);
    auto* yazdirBtn   = kutu.addButton(
        tr("Yazdır"),    QMessageBox::AcceptRole);
    auto* pdfBtn      = kutu.addButton(
        tr("PDF Kaydet"), QMessageBox::AcceptRole);
    kutu.addButton(QMessageBox::Cancel);
    kutu.exec();

    QAbstractButton* secilen = kutu.clickedButton();

    if (secilen == onizlemeBtn) {
        QPrinter yazici(QPrinter::HighResolution);
        QPrintPreviewDialog onizleme(&yazici, this);
        onizleme.setWindowTitle(tr("Baskı Önizleme"));
        connect(&onizleme, &QPrintPreviewDialog::paintRequested,
                this, [belge](QPrinter* p) { belge->print(p); });
        onizleme.exec();
    } else if (secilen == yazdirBtn) {
        QPrinter yazici(QPrinter::HighResolution);
        QPrintDialog dlg(&yazici, this);
        if (dlg.exec() == QDialog::Accepted) {
            belge->print(&yazici);
        }
    } else if (secilen == pdfBtn) {
        const QString yol = QFileDialog::getSaveFileName(
            this, tr("PDF Kaydet"),
            varsayilanPdfAd,
            tr("PDF Dosyaları (*.pdf)"));
        if (yol.isEmpty()) {
            belge->deleteLater();
            return;
        }
        QPrinter yazici(QPrinter::HighResolution);
        yazici.setOutputFormat(QPrinter::PdfFormat);
        yazici.setOutputFileName(yol);
        belge->print(&yazici);
        statusBar()->showMessage(
            tr("PDF kaydedildi: %1").arg(yol), 3000);
    }

    belge->deleteLater();
}

void MainWindow::transkriptYazdir() {
    const auto ogrenciler = m_servis.tumOgrenciler();
    if (ogrenciler.isEmpty()) {
        QMessageBox::information(
            this, tr("Transkript"),
            tr("Öğrenci listesi boş."));
        return;
    }

    QStringList isimler;
    for (const auto& o : ogrenciler) {
        isimler << QStringLiteral("%1 - %2")
                       .arg(o.numara, o.adSoyad);
    }
    bool ok = false;
    const QString secim = QInputDialog::getItem(
        this, tr("Transkript Yazdır"),
        tr("Öğrenci seçin:"), isimler, 0, false, &ok);
    if (!ok || secim.isEmpty()) { return; }

    const int idx = isimler.indexOf(secim);
    if (idx < 0) { return; }
    const Ogrenci ogrenci = ogrenciler.at(idx);

    TranskriptUretici uretici;
    const QString htmlMetin = uretici.html(
        ogrenci, m_servis.tumDersler(), m_servis.tumNotlar());

    auto* belge = new QTextDocument();
    belge->setHtml(htmlMetin);

    const QString pdfAd = QStringLiteral("transkript_%1.pdf")
                              .arg(ogrenci.numara);
    belgeyiYazdirmaAkisina(belge, pdfAd);
}

void MainWindow::topluRaporYazdir() {
    const auto dersler = m_servis.tumDersler();
    if (dersler.isEmpty()) {
        QMessageBox::information(
            this, tr("Toplu Rapor"),
            tr("Ders listesi boş."));
        return;
    }

    QStringList dersIsimleri;
    for (const auto& d : dersler) {
        dersIsimleri << QStringLiteral("%1 - %2")
                            .arg(d.kod, d.ad);
    }
    bool ok = false;
    const QString secim = QInputDialog::getItem(
        this, tr("Toplu Rapor"),
        tr("Ders seçin:"), dersIsimleri, 0, false, &ok);
    if (!ok || secim.isEmpty()) { return; }

    const int idx = dersIsimleri.indexOf(secim);
    if (idx < 0) { return; }
    const Ders ders = dersler.at(idx);

    DersRaporu rapor;
    QTextDocument* belge = rapor.olustur(
        ders, m_servis.tumOgrenciler(), m_servis.tumNotlar());

    const QString pdfAd = QStringLiteral("rapor_%1.pdf").arg(ders.kod);
    belgeyiYazdirmaAkisina(belge, pdfAd);
}

bool MainWindow::kaydetmeSorusu() {
    if (!m_dirty) { return true; }
    const auto sonuc = QMessageBox::question(
        this, tr("Kaydedilmemiş Değişiklikler"),
        tr("Kaydedilmemiş değişiklikler var. Kaydedilsin mi?"),
        QMessageBox::Save | QMessageBox::Discard |
            QMessageBox::Cancel,
        QMessageBox::Save);
    if (sonuc == QMessageBox::Save)   { return dosyaKaydet(); }
    if (sonuc == QMessageBox::Cancel) { return false; }
    return true;
}

void MainWindow::closeEvent(QCloseEvent* olay) {
    if (kaydetmeSorusu()) { olay->accept(); }
    else                  { olay->ignore(); }
}

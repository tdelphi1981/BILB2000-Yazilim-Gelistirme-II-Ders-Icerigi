/**
 * main_window.cpp -- OBYS v4 ana pencere uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 4 - OBYS GUI Mini Proje Tamamlama
 *
 * Unite 3'teki mantik korunur; uzerine disa aktarim (JSON), son
 * acilanlar menusu, tema secimi, hakkinda diyalogu, genisletilmis
 * kisayollar ve pencere durumu kalici saklama davranislari eklenir.
 *
 * Tum degisiklikler QSettings uzerinden kalici hale gelir:
 *   gorunum/tema                -- "light" veya "dark"
 *   pencere/geometri            -- saveGeometry() cikti
 *   pencere/durum               -- saveState()    cikti
 *   dosya/sonAcilanlar          -- QStringList
 */

#include "main_window.h"

#include "ders_raporu.h"
#include "grafik_paneli.h"
#include "istatistik_model.h"
#include "json_exporter.h"
#include "not_giris_dialog.h"
#include "notlar_model.h"
#include "obys_domain.h"
#include "transkript_uretici.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QIcon>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QInputDialog>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>
#include <QTableView>
#include <QTabWidget>
#include <QTextDocument>
#include <QTextStream>
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
    setWindowIcon(QIcon(":/icons/obys_app.svg"));
    merkeziWidgetKur();
    menuleriKur();

    m_servis.ornekVerileriYukle();

    // Baslangic icin birkac ornek not (histogram bos kalmasin)
    m_servis.dersKaydiEkle({1, 10, SayisalNot{92.0}});
    m_servis.dersKaydiEkle({2, 10, SayisalNot{68.0}});
    m_servis.dersKaydiEkle({3, 10, SayisalNot{55.0}});
    m_servis.dersKaydiEkle({1, 11, SayisalNot{85.0}});
    m_servis.dersKaydiEkle({2, 11, SayisalNot{44.0}});

    m_notlarModel->yenile();

    m_aktifDosya.clear();
    m_dirty = false;
    basligiGuncelle();

    // Kalici ayarlari (pencere durumu + son acilanlar + tema) yukle
    ayarlariYukle();
    sonAcilanlariYukle();
    sonAcilanlarMenusunuYenile();

    statusBar()->showMessage(tr("Hazır"), 2000);
    if (size().isEmpty()) {
        resize(900, 600);
    }
}

// -----------------------------------------------------------------
// Merkez widget ve sekmeler
// -----------------------------------------------------------------
QWidget* MainWindow::notlarSekmesiKur() {
    auto* sayfa    = new QWidget(this);
    auto* yerlesim = new QVBoxLayout(sayfa);

    m_notTablosu  = new QTableView(sayfa);
    m_notlarModel = new NotlarModel(&m_servis, this);
    m_notTablosu->setModel(m_notlarModel);
    m_notTablosu->horizontalHeader()->setStretchLastSection(true);
    m_notTablosu->setAlternatingRowColors(true);
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

    m_sekmeler->addTab(notlarSekmesiKur(), tr("Notlar"));

    m_grafikPanel     = new GrafikPaneli(this);
    m_istatistikModel = new IstatistikModel(3, this);
    m_istatistikModel->setKaynakModel(m_notlarModel);
    m_grafikPanel->setModel(m_istatistikModel);
    m_sekmeler->addTab(m_grafikPanel, tr("Grafikler"));

    m_sekmeler->addTab(trendSekmesiKur(), tr("Trend"));

    setCentralWidget(m_sekmeler);
}

// -----------------------------------------------------------------
// Menu kurulumu
// -----------------------------------------------------------------
void MainWindow::menuleriKur() {
    // ---- Dosya menusu ----
    auto* dosyaMenu = menuBar()->addMenu(tr("&Dosya"));

    m_yeniAction = dosyaMenu->addAction(QIcon(":/icons/yeni.svg"), tr("&Yeni"));
    m_yeniAction->setShortcut(QKeySequence::New);
    connect(m_yeniAction, &QAction::triggered,
            this, &MainWindow::dosyaYeni);

    m_acAction = dosyaMenu->addAction(QIcon(":/icons/ac.svg"), tr("&Aç..."));
    m_acAction->setShortcut(QKeySequence::Open);
    connect(m_acAction, &QAction::triggered,
            this, &MainWindow::dosyaAc);

    // Son Acilanlar alt menusu
    m_sonAcilanlarMenu = dosyaMenu->addMenu(QIcon(":/icons/son_acilanlar.svg"), tr("Son A&çılanlar"));

    dosyaMenu->addSeparator();

    m_kaydetAction = dosyaMenu->addAction(QIcon(":/icons/kaydet.svg"), tr("&Kaydet"));
    m_kaydetAction->setShortcut(QKeySequence::Save);
    connect(m_kaydetAction, &QAction::triggered,
            this, [this]() { (void)dosyaKaydet(); });

    m_farkliKaydetAction =
        dosyaMenu->addAction(QIcon(":/icons/farkli_kaydet.svg"), tr("&Farklı Kaydet..."));
    m_farkliKaydetAction->setShortcut(QKeySequence::SaveAs);
    connect(m_farkliKaydetAction, &QAction::triggered,
            this, [this]() { (void)dosyaFarkliKaydet(); });

    dosyaMenu->addSeparator();

    // Disa Aktar alt menusu
    auto* disaAktarMenu = dosyaMenu->addMenu(QIcon(":/icons/disa_aktar.svg"), tr("&Dışa Aktar"));
    m_disaAktarJsonAction =
        disaAktarMenu->addAction(tr("&JSON..."));
    m_disaAktarJsonAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_E));
    connect(m_disaAktarJsonAction, &QAction::triggered,
            this, &MainWindow::disaAktarJson);

    dosyaMenu->addSeparator();

    m_transkriptAction =
        dosyaMenu->addAction(QIcon(":/icons/yazdir.svg"), tr("Transkript Ya&zdır..."));
    m_transkriptAction->setShortcut(QKeySequence::Print);
    connect(m_transkriptAction, &QAction::triggered,
            this, &MainWindow::transkriptYazdir);

    m_topluRaporAction =
        dosyaMenu->addAction(QIcon(":/icons/toplu_rapor.svg"), tr("Toplu &Rapor..."));
    m_topluRaporAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_P));
    connect(m_topluRaporAction, &QAction::triggered,
            this, &MainWindow::topluRaporYazdir);

    dosyaMenu->addSeparator();

    m_cikisAction = dosyaMenu->addAction(QIcon(":/icons/cikis.svg"), tr("Çı&kış"));
    m_cikisAction->setShortcut(QKeySequence::Quit);
    connect(m_cikisAction, &QAction::triggered,
            this, &MainWindow::close);

    // ---- Duzen menusu (sadece Not Ekle) ----
    auto* duzenMenu = menuBar()->addMenu(tr("&Düzen"));
    auto* notEkleAction = duzenMenu->addAction(QIcon(":/icons/not_ekle.svg"), tr("&Not Ekle..."));
    notEkleAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    // Not: Ctrl+N Yeni icin rezerve; "Not Ekle" icin Ctrl+Shift+N kullanalim
    notEkleAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_N));
    connect(notEkleAction, &QAction::triggered,
            this, &MainWindow::notEkleDialogAc);

    // ---- Gorunum menusu ----
    auto* gorunumMenu = menuBar()->addMenu(tr("&Görünüm"));

    auto* temaGrup = new QActionGroup(this);
    temaGrup->setExclusive(true);

    m_aydinlikTemaAction =
        gorunumMenu->addAction(QIcon(":/icons/aydinlik_tema.svg"), tr("&Aydınlık Tema"));
    m_aydinlikTemaAction->setCheckable(true);
    m_aydinlikTemaAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::Key_1));
    temaGrup->addAction(m_aydinlikTemaAction);
    connect(m_aydinlikTemaAction, &QAction::triggered, this, [this]() {
        temayiUygula(QStringLiteral("light"));
    });

    m_karanlikTemaAction =
        gorunumMenu->addAction(QIcon(":/icons/karanlik_tema.svg"), tr("&Karanlık Tema"));
    m_karanlikTemaAction->setCheckable(true);
    m_karanlikTemaAction->setShortcut(
        QKeySequence(Qt::CTRL | Qt::Key_2));
    temaGrup->addAction(m_karanlikTemaAction);
    connect(m_karanlikTemaAction, &QAction::triggered, this, [this]() {
        temayiUygula(QStringLiteral("dark"));
    });

    // GOREV-11: (Lab 08 / Calisma 4 / Talep 3) Gorunum menusune
    // "Temayi Degistir" aksiyonu ekleyin ve Ctrl+Shift+T kisayolunu
    // baglayin. Kisayol her basildiginda aydinlik/karanlik tema
    // arasinda gecis yapsin. Aktif tema adi durum cubugunda 3 saniye
    // boyunca gosterilsin (statusBar()->showMessage(..., 3000)).
    //
    // Ipucu:
    //     auto* temayiDegistir = gorunumMenu->addAction(
    //         tr("Temayi Degistir"));
    //     temayiDegistir->setShortcut(
    //         QKeySequence(QStringLiteral("Ctrl+Shift+T")));
    //     connect(temayiDegistir, &QAction::triggered, this, [this]() {
    //         QSettings a;
    //         const bool koyu = a.value("gorunum/tema")
    //                               .toString() == "dark";
    //         temayiUygula(koyu ? "light" : "dark");
    //         statusBar()->showMessage(
    //             tr("Tema: %1").arg(koyu ? tr("Aydinlik")
    //                                     : tr("Karanlik")), 3000);
    //     });

    // ---- Yardim menusu ----
    auto* yardimMenu = menuBar()->addMenu(tr("&Yardım"));
    m_hakkindaAction = yardimMenu->addAction(QIcon(":/icons/hakkinda.svg"), tr("&Hakkında..."));
    m_hakkindaAction->setShortcut(QKeySequence::HelpContents);
    connect(m_hakkindaAction, &QAction::triggered,
            this, &MainWindow::hakkindaGoster);
}

// -----------------------------------------------------------------
// Baslik / dirty / kaydetme sorusu
// -----------------------------------------------------------------
void MainWindow::basligiGuncelle() {
    const QString ad = m_aktifDosya.isEmpty()
        ? tr("İsimsiz")
        : QFileInfo(m_aktifDosya).fileName();
    const QString yildiz = m_dirty
        ? QStringLiteral(" *")
        : QString();
    setWindowTitle(QStringLiteral("OBYS v4 - %1%2").arg(ad, yildiz));
}

void MainWindow::dirtyYap(bool d) {
    m_dirty = d;
    basligiGuncelle();
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

// -----------------------------------------------------------------
// Dosya aksiyonlari
// -----------------------------------------------------------------
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
    dosyaYoluAc(yol);
}

void MainWindow::dosyaYoluAc(const QString& yol) {
    if (!m_persist.yukle(yol, m_servis)) {
        QMessageBox::warning(
            this, tr("Açma Hatası"), m_persist.sonHata());
        return;
    }
    m_notlarModel->yenile();
    m_aktifDosya = yol;
    m_dirty = false;
    basligiGuncelle();
    sonAcilanlariGuncelle(yol);
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
    sonAcilanlariGuncelle(m_aktifDosya);
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
    sonAcilanlariGuncelle(yol);
    statusBar()->showMessage(tr("Kaydedildi: %1").arg(yol), 3000);
    return true;
}

// -----------------------------------------------------------------
// JSON disa aktarim
// -----------------------------------------------------------------
void MainWindow::disaAktarJson() {
    // GOREV-10: (Lab 08 / Calisma 4 / Talep 2) QSettings uzerinden
    // son secilen JSON dizinini "disaAktar/sonDizin" anahtari ile
    // saklayin. Bu fonksiyonun basinda ayardan son dizini okuyun,
    // QFileDialog'u o dizinden baslatin ve kullanici kayit yapinca
    // yeni dizini ayara geri yazin.
    //
    // Ipucu:
    //     QSettings ayarlar;
    //     const QString sonDizin = ayarlar.value(
    //         QStringLiteral("disaAktar/sonDizin")).toString();
    //     const QString basDizin = sonDizin.isEmpty() ? onerilen
    //         : sonDizin + QLatin1Char('/') + onerilen;

    const QString onerilen = m_aktifDosya.isEmpty()
        ? QStringLiteral("obys.json")
        : QFileInfo(m_aktifDosya).completeBaseName() +
              QStringLiteral(".json");

    QString yol = QFileDialog::getSaveFileName(
        this, tr("JSON Olarak Dışa Aktar"), onerilen,
        tr("JSON Dosyaları (*.json)"));
    if (yol.isEmpty()) { return; }
    if (!yol.endsWith(QStringLiteral(".json"),
                      Qt::CaseInsensitive)) {
        yol += QStringLiteral(".json");
    }

    JsonExporter dis;
    if (!dis.disaAktar(yol, m_servis)) {
        QMessageBox::warning(
            this, tr("Dışa Aktarım Hatası"), dis.sonHata());
        return;
    }
    // GOREV-10 (devam): Basarili kayittan sonra son kullanilan dizini
    // QSettings'e yazin:
    //     QSettings ayarlar;
    //     ayarlar.setValue(QStringLiteral("disaAktar/sonDizin"),
    //                      QFileInfo(yol).absolutePath());
    statusBar()->showMessage(
        tr("JSON olarak kaydedildi: %1").arg(yol), 3000);
}

// -----------------------------------------------------------------
// Not ekleme
// -----------------------------------------------------------------
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

// -----------------------------------------------------------------
// Yazdirma akisi yardimcisi (v3 ile ayni)
// -----------------------------------------------------------------
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

// -----------------------------------------------------------------
// Tema uygulama
// -----------------------------------------------------------------
void MainWindow::temayiUygula(const QString& temaAd) {
    const QString yol = (temaAd == QStringLiteral("dark"))
        ? QStringLiteral(":/themes/dark.qss")
        : QStringLiteral(":/themes/light.qss");

    QFile dosya(yol);
    QString qss;
    if (dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream akis(&dosya);
        qss = akis.readAll();
    }
    qApp->setStyleSheet(qss);

    // Menu durumlarini guncelle
    if (m_aydinlikTemaAction != nullptr) {
        m_aydinlikTemaAction->setChecked(
            temaAd == QStringLiteral("light"));
    }
    if (m_karanlikTemaAction != nullptr) {
        m_karanlikTemaAction->setChecked(
            temaAd == QStringLiteral("dark"));
    }

    // Kalici sakla
    QSettings ayarlar;
    ayarlar.setValue(QStringLiteral("gorunum/tema"), temaAd);

    statusBar()->showMessage(
        tr("Tema: %1").arg(temaAd), 1500);
}

// -----------------------------------------------------------------
// Hakkinda diyalogu
// -----------------------------------------------------------------
void MainWindow::hakkindaGoster() {
    const QString metin = tr(
        "<h3>OBYS v%1</h3>"
        "<p>Öğrenci Bilgi Yönetim Sistemi - Mini Proje Tamamlama</p>"
        "<p>Bölüm 8 - Qt ile OBYS GUI Tamamlama<br/>"
        "Ünite 4 - OBYS GUI Mini Proje Tamamlama</p>"
        "<p>KTÜ Bilgisayar Bilimleri - "
        "Yazılım Geliştirme II (BILB2000)</p>")
        .arg(QCoreApplication::applicationVersion());
    QMessageBox::about(this, tr("Hakkında"), metin);
}

// -----------------------------------------------------------------
// QSettings: pencere durumu + tema
// -----------------------------------------------------------------
void MainWindow::ayarlariYukle() {
    QSettings ayarlar;

    const QByteArray geom =
        ayarlar.value(QStringLiteral("pencere/geometri"))
               .toByteArray();
    if (!geom.isEmpty()) {
        restoreGeometry(geom);
    }
    const QByteArray durum =
        ayarlar.value(QStringLiteral("pencere/durum"))
               .toByteArray();
    if (!durum.isEmpty()) {
        restoreState(durum);
    }

    // Tema aksiyonlarini mevcut secime gore isaretle
    const QString temaAd =
        ayarlar.value(QStringLiteral("gorunum/tema"),
                      QStringLiteral("light")).toString();
    if (m_aydinlikTemaAction != nullptr) {
        m_aydinlikTemaAction->setChecked(
            temaAd == QStringLiteral("light"));
    }
    if (m_karanlikTemaAction != nullptr) {
        m_karanlikTemaAction->setChecked(
            temaAd == QStringLiteral("dark"));
    }
}

void MainWindow::ayarlariKaydet() {
    QSettings ayarlar;
    ayarlar.setValue(
        QStringLiteral("pencere/geometri"), saveGeometry());
    ayarlar.setValue(
        QStringLiteral("pencere/durum"),    saveState());
}

// -----------------------------------------------------------------
// Son acilanlar
// -----------------------------------------------------------------
void MainWindow::sonAcilanlariYukle() {
    QSettings ayarlar;
    m_sonAcilanlar =
        ayarlar.value(QStringLiteral("dosya/sonAcilanlar"))
               .toStringList();
    // Artik var olmayan dosyalari listeden temizle
    QStringList temiz;
    for (const auto& y : m_sonAcilanlar) {
        if (QFileInfo::exists(y)) {
            temiz.append(y);
        }
    }
    m_sonAcilanlar = temiz;
}

void MainWindow::sonAcilanlariGuncelle(const QString& yol) {
    m_sonAcilanlar.removeAll(yol);
    m_sonAcilanlar.prepend(yol);
    while (m_sonAcilanlar.size() > kMaxSonAcilan) {
        m_sonAcilanlar.removeLast();
    }

    QSettings ayarlar;
    ayarlar.setValue(QStringLiteral("dosya/sonAcilanlar"),
                     m_sonAcilanlar);

    sonAcilanlarMenusunuYenile();
}

void MainWindow::sonAcilanlarMenusunuYenile() {
    if (m_sonAcilanlarMenu == nullptr) { return; }
    m_sonAcilanlarMenu->clear();

    if (m_sonAcilanlar.isEmpty()) {
        auto* bos = m_sonAcilanlarMenu->addAction(tr("(boş)"));
        bos->setEnabled(false);
        return;
    }

    int sira = 1;
    for (const auto& yol : m_sonAcilanlar) {
        const QString metin = QStringLiteral("&%1  %2")
            .arg(sira)
            .arg(QFileInfo(yol).fileName());
        auto* act = m_sonAcilanlarMenu->addAction(metin);
        act->setToolTip(yol);
        connect(act, &QAction::triggered, this, [this, yol]() {
            if (!kaydetmeSorusu()) { return; }
            dosyaYoluAc(yol);
        });
        ++sira;
    }

    m_sonAcilanlarMenu->addSeparator();
    auto* temizle = m_sonAcilanlarMenu->addAction(tr("Listeyi &Temizle"));
    connect(temizle, &QAction::triggered, this, [this]() {
        m_sonAcilanlar.clear();
        QSettings ayarlar;
        ayarlar.setValue(QStringLiteral("dosya/sonAcilanlar"),
                         m_sonAcilanlar);
        sonAcilanlarMenusunuYenile();
    });
}

// -----------------------------------------------------------------
// Kapanis
// -----------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent* olay) {
    if (!kaydetmeSorusu()) {
        olay->ignore();
        return;
    }
    ayarlariKaydet();
    olay->accept();
}

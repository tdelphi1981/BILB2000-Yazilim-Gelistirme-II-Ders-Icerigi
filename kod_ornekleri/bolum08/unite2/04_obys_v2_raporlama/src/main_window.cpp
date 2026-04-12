/**
 * main_window.cpp -- OBYS v2 ana pencere uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 *
 * MainWindow'un uygulamasi. Unite 1'deki mantik korunur; eklenen
 * raporlama aksiyonlari QTextDocument uretir ve ortak bir
 * "belgeyiYazdirmaAkisina" yardimcisi araciligi ile kullaniciya
 * Onizleme / Yazdir / PDF secenegi sunar.
 */

#include "main_window.h"

#include "ders_raporu.h"
#include "not_giris_dialog.h"
#include "obys_domain.h"
#include "transkript_uretici.h"

#include <QAction>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTextDocument>
#include <QVBoxLayout>
#include <QWidget>

#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>

namespace {

QString notKaydiniFormatla(const DersKaydi& k,
                           const OBYSServis& servis) {
    QString ogrAd = QStringLiteral("?");
    for (const auto& o : servis.tumOgrenciler()) {
        if (o.id == k.ogrenciId) { ogrAd = o.adSoyad; break; }
    }
    QString dersAd = QStringLiteral("?");
    for (const auto& d : servis.tumDersler()) {
        if (d.id == k.dersId) {
            dersAd = d.kod + QStringLiteral(" ") + d.ad;
            break;
        }
    }
    QString notStr;
    std::visit([&notStr](const auto& deger) {
        using T = std::decay_t<decltype(deger)>;
        if constexpr (std::is_same_v<T, SayisalNot>) {
            notStr = QString::number(deger.deger, 'f', 2);
        } else if constexpr (std::is_same_v<T, HarfNotu>) {
            notStr = deger.harf;
        } else if constexpr (std::is_same_v<T, MuafNot>) {
            notStr = QStringLiteral("MUAF");
        }
    }, k.notu);
    return QStringLiteral("%1  |  %2  |  %3")
        .arg(ogrAd, dersAd, notStr);
}

} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    merkeziWidgetKur();
    menuleriKur();

    m_servis.ornekVerileriYukle();
    m_aktifDosya.clear();
    m_dirty = false;
    basligiGuncelle();
    notListesiniGuncelle();

    statusBar()->showMessage(tr("Hazır"), 2000);
    resize(720, 480);
}

void MainWindow::merkeziWidgetKur() {
    auto* merkez   = new QWidget(this);
    auto* yerlesim = new QVBoxLayout(merkez);

    m_notListesi    = new QListWidget(merkez);
    m_notEkleButonu = new QPushButton(tr("Not Ekle..."), merkez);

    yerlesim->addWidget(m_notListesi);
    yerlesim->addWidget(m_notEkleButonu);

    setCentralWidget(merkez);

    connect(m_notEkleButonu, &QPushButton::clicked,
            this, &MainWindow::notEkleDialogAc);
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

    // Unite 2 -- Raporlama aksiyonlari
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
    setWindowTitle(QStringLiteral("OBYS v2 - %1%2").arg(ad, yildiz));
}

void MainWindow::notListesiniGuncelle() {
    m_notListesi->clear();
    for (const auto& k : m_servis.tumNotlar()) {
        m_notListesi->addItem(notKaydiniFormatla(k, m_servis));
    }
}

void MainWindow::dirtyYap(bool d) {
    m_dirty = d;
    basligiGuncelle();
}

void MainWindow::dosyaYeni() {
    if (!kaydetmeSorusu()) { return; }
    m_servis.ornekVerileriYukle();
    m_aktifDosya.clear();
    m_dirty = false;
    basligiGuncelle();
    notListesiniGuncelle();
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
    m_aktifDosya = yol;
    m_dirty = false;
    basligiGuncelle();
    notListesiniGuncelle();
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
        notListesiniGuncelle();
        dirtyYap(true);
        statusBar()->showMessage(tr("Not eklendi"), 2000);
    }
}

// ---- Unite 2: Raporlama aksiyonlari -------------------------------

void MainWindow::belgeyiYazdirmaAkisina(
    QTextDocument* belge,
    const QString& varsayilanPdfAd)
{
    if (belge == nullptr) { return; }

    // Uc seceneklik bir mesaj kutusu
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

    // Ogrenci secimi icin QInputDialog kullaniyoruz
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

    // HTML uret, QTextDocument'a yukle
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

// --------------------------------------------------------------------

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

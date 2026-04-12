#include "main_window.h"
#include "not_giris_dialog.h"
#include "notlar_model.h"
#include "obys_domain.h"
#include "ui_ogrenci_ekle_dialog.h"
#include "ui_ders_ekle_dialog.h"

#include <QAction>
#include <QIcon>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>
#include <QTableView>
#include <QTabWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    merkeziWidgetKur();
    menuleriKur();

    m_servis.ornekVerileriYukle();

    // Ornek notlar
    m_servis.dersKaydiEkle(
        {1, 10, SayisalNot{92.0}});
    m_servis.dersKaydiEkle(
        {2, 10, SayisalNot{68.0}});
    m_servis.dersKaydiEkle(
        {3, 10, SayisalNot{55.0}});
    m_servis.dersKaydiEkle(
        {1, 11, SayisalNot{85.0}});

    m_notlarModel->yenile();
    m_aktifDosya.clear();
    m_dirty = false;
    basligiGuncelle();

    ayarlariYukle();
    sonAcilanlariYukle();
    sonAcilanlarMenusunuYenile();

    statusBar()->showMessage("Hazır", 2000);
    if (size().isEmpty()) resize(900, 600);
}

QWidget* MainWindow::notlarSekmesiKur() {
    auto* sayfa    = new QWidget(this);
    auto* yerlesim = new QVBoxLayout(sayfa);

    m_notTablosu  = new QTableView(sayfa);
    m_notlarModel = new NotlarModel(
                        &m_servis, this);
    m_notTablosu->setModel(m_notlarModel);
    m_notTablosu->horizontalHeader()
                ->setStretchLastSection(true);
    m_notTablosu->setAlternatingRowColors(true);
    m_notTablosu->setSelectionBehavior(
        QAbstractItemView::SelectRows);
    m_notTablosu->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    m_notEkleButonu =
        new QPushButton("Not Ekle...", sayfa);

    yerlesim->addWidget(m_notTablosu);
    yerlesim->addWidget(m_notEkleButonu);

    connect(m_notEkleButonu,
            &QPushButton::clicked,
            this,
            &MainWindow::notEkleDialogAc);

    return sayfa;
}

void MainWindow::merkeziWidgetKur() {
    m_sekmeler = new QTabWidget(this);
    m_sekmeler->addTab(
        notlarSekmesiKur(), "Notlar");
    // Grafik ve trend sekmeleri
    // Asama 6'da eklenecek
    setCentralWidget(m_sekmeler);
}

void MainWindow::menuleriKur() {
    // Dosya menusu
    auto* dosyaMenu =
        menuBar()->addMenu("&Dosya");

    auto* yeniAct =
        dosyaMenu->addAction("&Yeni");
    yeniAct->setShortcut(QKeySequence::New);
    connect(yeniAct, &QAction::triggered,
            this, &MainWindow::dosyaYeni);

    auto* acAct =
        dosyaMenu->addAction("&Aç...");
    acAct->setShortcut(QKeySequence::Open);
    connect(acAct, &QAction::triggered,
            this, &MainWindow::dosyaAc);

    m_sonAcilanlarMenu =
        dosyaMenu->addMenu("Son Açılanlar");

    dosyaMenu->addSeparator();

    auto* kaydetAct =
        dosyaMenu->addAction("&Kaydet");
    kaydetAct->setShortcut(QKeySequence::Save);
    connect(kaydetAct, &QAction::triggered,
            this, [this]{ dosyaKaydet(); });

    auto* farkliAct = dosyaMenu->addAction(
        "&Farklı Kaydet...");
    farkliAct->setShortcut(QKeySequence::SaveAs);
    connect(farkliAct, &QAction::triggered,
            this, [this]{ dosyaFarkliKaydet(); });

    dosyaMenu->addSeparator();

    auto* cikisAct = dosyaMenu->addAction(
        "Çıkış");
    cikisAct->setShortcut(QKeySequence::Quit);
    connect(cikisAct, &QAction::triggered,
            this, &MainWindow::close);

    // Veri menusu
    auto* veriMenu = menuBar()->addMenu(
        "&Veri");

    auto* ogrEkle = veriMenu->addAction(
        "Öğrenci Ekle...");
    connect(ogrEkle, &QAction::triggered,
            this,
            &MainWindow::ogrenciEkleDialogAc);

    auto* dersEkle = veriMenu->addAction(
        "Ders Ekle...");
    connect(dersEkle, &QAction::triggered,
            this,
            &MainWindow::dersEkleDialogAc);

    veriMenu->addSeparator();

    auto* notEkleAct = veriMenu->addAction(
        "Not Ekle...");
    notEkleAct->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT
                     | Qt::Key_N));
    connect(notEkleAct, &QAction::triggered,
            this,
            &MainWindow::notEkleDialogAc);
}

void MainWindow::basligiGuncelle() {
    const QString ad = m_aktifDosya.isEmpty()
        ? "İsimsiz"
        : QFileInfo(m_aktifDosya).fileName();
    const QString yildiz = m_dirty ? " *" : "";
    setWindowTitle(
        QString("OBYS - %1%2").arg(ad, yildiz));
}

void MainWindow::dirtyYap(bool d) {
    m_dirty = d;
    basligiGuncelle();
}

bool MainWindow::kaydetmeSorusu() {
    if (!m_dirty) return true;
    const auto s = QMessageBox::question(
        this, "Kaydedilmemiş Değişiklikler",
        "Kaydedilsin mi?",
        QMessageBox::Save | QMessageBox::Discard
        | QMessageBox::Cancel,
        QMessageBox::Save);
    if (s == QMessageBox::Save)
        return dosyaKaydet();
    if (s == QMessageBox::Cancel)
        return false;
    return true;
}

void MainWindow::dosyaYeni() {
    if (!kaydetmeSorusu()) return;
    m_servis.ornekVerileriYukle();
    m_notlarModel->yenile();
    m_aktifDosya.clear();
    m_dirty = false;
    basligiGuncelle();
}

void MainWindow::dosyaAc() {
    if (!kaydetmeSorusu()) return;
    const QString yol =
        QFileDialog::getOpenFileName(
            this, "OBYS Dosyası Aç", "",
            "OBYS (*.obys);;Tumu (*)");
    if (yol.isEmpty()) return;
    dosyaYoluAc(yol);
}

void MainWindow::dosyaYoluAc(
    const QString& yol) {
    if (!m_persist.yukle(yol, m_servis)) {
        QMessageBox::warning(this, "Hata",
            m_persist.sonHata());
        return;
    }
    m_notlarModel->yenile();
    m_aktifDosya = yol;
    m_dirty = false;
    basligiGuncelle();
    sonAcilanlariGuncelle(yol);
}

bool MainWindow::dosyaKaydet() {
    if (m_aktifDosya.isEmpty())
        return dosyaFarkliKaydet();
    if (!m_persist.kaydet(
            m_aktifDosya, m_servis)) {
        QMessageBox::warning(this, "Hata",
            m_persist.sonHata());
        return false;
    }
    m_dirty = false;
    basligiGuncelle();
    sonAcilanlariGuncelle(m_aktifDosya);
    return true;
}

bool MainWindow::dosyaFarkliKaydet() {
    QString yol =
        QFileDialog::getSaveFileName(
            this, "Farklı Kaydet",
            "oturum.obys",
            "OBYS (*.obys)");
    if (yol.isEmpty()) return false;
    if (!yol.endsWith(".obys",
            Qt::CaseInsensitive))
        yol += ".obys";
    if (!m_persist.kaydet(yol, m_servis)) {
        QMessageBox::warning(this, "Hata",
            m_persist.sonHata());
        return false;
    }
    m_aktifDosya = yol;
    m_dirty = false;
    basligiGuncelle();
    sonAcilanlariGuncelle(yol);
    return true;
}

void MainWindow::ogrenciEkleDialogAc() {
    QDialog dlg(this);
    Ui::OgrenciEkleDialog ui;
    ui.setupUi(&dlg);

    connect(ui.buttonBox,
            &QDialogButtonBox::accepted,
            &dlg, &QDialog::accept);
    connect(ui.buttonBox,
            &QDialogButtonBox::rejected,
            &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    const QString numara =
        ui.numaraEdit->text().trimmed();
    const QString adSoyad =
        ui.adSoyadEdit->text().trimmed();
    if (numara.isEmpty() || adSoyad.isEmpty()) {
        QMessageBox::warning(this, "Uyarı",
            "Tüm alanları doldurun.");
        return;
    }

    // Basit ID atama
    int yeniId = 1;
    for (const auto& o :
         m_servis.tumOgrenciler()) {
        if (o.id >= yeniId) yeniId = o.id + 1;
    }

    m_servis.ogrenciEkle(
        {yeniId, numara, adSoyad});
    dirtyYap(true);
    statusBar()->showMessage(
        "Öğrenci eklendi: " + adSoyad, 2000);
}

void MainWindow::dersEkleDialogAc() {
    QDialog dlg(this);
    Ui::DersEkleDialog ui;
    ui.setupUi(&dlg);

    connect(ui.buttonBox,
            &QDialogButtonBox::accepted,
            &dlg, &QDialog::accept);
    connect(ui.buttonBox,
            &QDialogButtonBox::rejected,
            &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    const QString kod =
        ui.kodEdit->text().trimmed();
    const QString ad =
        ui.adEdit->text().trimmed();
    const int kredi = ui.krediSpin->value();
    if (kod.isEmpty() || ad.isEmpty()) {
        QMessageBox::warning(this, "Uyarı",
            "Tüm alanları doldurun.");
        return;
    }

    int yeniId = 1;
    for (const auto& d : m_servis.tumDersler()) {
        if (d.id >= yeniId) yeniId = d.id + 1;
    }

    m_servis.dersEkle({yeniId, kod, ad, kredi});
    dirtyYap(true);
    statusBar()->showMessage(
        "Ders eklendi: " + ad, 2000);
}

void MainWindow::notEkleDialogAc() {
    NotGirisDialog dlg(this);

    QList<QPair<int, QString>> ogrenciler;
    for (const auto& o :
         m_servis.tumOgrenciler()) {
        ogrenciler.append({o.id, o.adSoyad});
    }
    QList<QPair<int, QString>> dersler;
    for (const auto& d : m_servis.tumDersler()) {
        dersler.append(
            {d.id,
             QString("%1 %2").arg(d.kod, d.ad)});
    }
    dlg.ogrencileriYukle(ogrenciler);
    dlg.dersleriYukle(dersler);

    if (dlg.exec() == QDialog::Accepted) {
        m_servis.notEkle(dlg.notDTO());
        m_notlarModel->yenile();
        dirtyYap(true);
        statusBar()->showMessage(
            "Not eklendi", 2000);
    }
}

// QSettings
void MainWindow::ayarlariYukle() {
    QSettings s;
    const QByteArray g =
        s.value("pencere/geometri").toByteArray();
    if (!g.isEmpty()) restoreGeometry(g);
    const QByteArray d =
        s.value("pencere/durum").toByteArray();
    if (!d.isEmpty()) restoreState(d);
}

void MainWindow::ayarlariKaydet() {
    QSettings s;
    s.setValue("pencere/geometri",
              saveGeometry());
    s.setValue("pencere/durum",
              saveState());
}

// Son acilanlar
void MainWindow::sonAcilanlariYukle() {
    QSettings s;
    m_sonAcilanlar =
        s.value("dosya/sonAcilanlar")
         .toStringList();
    QStringList temiz;
    for (const auto& y : m_sonAcilanlar) {
        if (QFileInfo::exists(y))
            temiz.append(y);
    }
    m_sonAcilanlar = temiz;
}

void MainWindow::sonAcilanlariGuncelle(
    const QString& yol) {
    m_sonAcilanlar.removeAll(yol);
    m_sonAcilanlar.prepend(yol);
    while (m_sonAcilanlar.size() > kMaxSonAcilan)
        m_sonAcilanlar.removeLast();
    QSettings s;
    s.setValue("dosya/sonAcilanlar",
              m_sonAcilanlar);
    sonAcilanlarMenusunuYenile();
}

void MainWindow::sonAcilanlarMenusunuYenile() {
    if (!m_sonAcilanlarMenu) return;
    m_sonAcilanlarMenu->clear();
    if (m_sonAcilanlar.isEmpty()) {
        auto* bos = m_sonAcilanlarMenu->addAction(
            "(boş)");
        bos->setEnabled(false);
        return;
    }
    int sira = 1;
    for (const auto& yol : m_sonAcilanlar) {
        auto* act =
            m_sonAcilanlarMenu->addAction(
                QString("&%1 %2").arg(sira)
                    .arg(QFileInfo(yol)
                        .fileName()));
        connect(act, &QAction::triggered,
                this, [this, yol]{
            if (kaydetmeSorusu())
                dosyaYoluAc(yol);
        });
        ++sira;
    }
}

void MainWindow::closeEvent(
    QCloseEvent* olay) {
    if (!kaydetmeSorusu()) {
        olay->ignore();
        return;
    }
    ayarlariKaydet();
    olay->accept();
}

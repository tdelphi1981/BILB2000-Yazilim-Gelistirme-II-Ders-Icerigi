#include "main_window.h"
#include "not_giris_dialog.h"
#include "notlar_model.h"
#include "obys_domain.h"
#include "ui_ogrenci_ekle_dialog.h"
#include "ui_ders_ekle_dialog.h"
#include "ui_ayarlar_dialog.h"
#include "json_exporter.h"
#include "grafik_paneli.h"
#include "istatistik_model.h"
#include "transkript_uretici.h"

#include <QAction>
#include <QIcon>
#include <QActionGroup>
#include <QCloseEvent>
#include <QFile>
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
#include <QTextDocument>
#include <QTextStream>
#include <QVBoxLayout>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>

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

    m_grafikPanel = new GrafikPaneli(this);
    m_istatistikModel =
        new IstatistikModel(3, this);
    m_istatistikModel->setKaynakModel(
        m_notlarModel);
    m_grafikPanel->setModel(
        m_istatistikModel);
    m_sekmeler->addTab(
        m_grafikPanel, "Grafikler");

    setCentralWidget(m_sekmeler);
}

void MainWindow::menuleriKur() {
    // Dosya menusu
    auto* dosyaMenu =
        menuBar()->addMenu("&Dosya");

    auto* yeniAct = dosyaMenu->addAction(
        QIcon(":/resources/icons/file-plus.svg"), "&Yeni");
    yeniAct->setShortcut(QKeySequence::New);
    connect(yeniAct, &QAction::triggered,
            this, &MainWindow::dosyaYeni);

    auto* acAct = dosyaMenu->addAction(
        QIcon(":/resources/icons/folder-open.svg"), "&Aç...");
    acAct->setShortcut(QKeySequence::Open);
    connect(acAct, &QAction::triggered,
            this, &MainWindow::dosyaAc);

    m_sonAcilanlarMenu =
        dosyaMenu->addMenu("Son Açılanlar");

    dosyaMenu->addSeparator();

    auto* kaydetAct = dosyaMenu->addAction(
        QIcon(":/resources/icons/save.svg"), "&Kaydet");
    kaydetAct->setShortcut(QKeySequence::Save);
    connect(kaydetAct, &QAction::triggered,
            this, [this]{ dosyaKaydet(); });

    auto* farkliAct = dosyaMenu->addAction(
        QIcon(":/resources/icons/save.svg"), "&Farklı Kaydet...");
    farkliAct->setShortcut(QKeySequence::SaveAs);
    connect(farkliAct, &QAction::triggered,
            this, [this]{ dosyaFarkliKaydet(); });

    // Disa aktar alt menusu (Asama 5)
    dosyaMenu->addSeparator();
    auto* disaAktarMenu =
        dosyaMenu->addMenu("Dışa Aktar");
    auto* jsonAct = disaAktarMenu->addAction(
        QIcon(":/resources/icons/download.svg"), "JSON...");
    jsonAct->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT
                     | Qt::Key_E));
    connect(jsonAct, &QAction::triggered,
            this, &MainWindow::disaAktarJson);

    // Transkript (Asama 6)
    dosyaMenu->addSeparator();
    auto* transkriptAct = dosyaMenu->addAction(
        QIcon(":/resources/icons/printer.svg"), "Transkript...");
    transkriptAct->setShortcut(QKeySequence::Print);
    connect(transkriptAct, &QAction::triggered,
            this, &MainWindow::transkriptYazdir);

    dosyaMenu->addSeparator();

    auto* cikisAct = dosyaMenu->addAction(
        QIcon(":/resources/icons/log-out.svg"), "Çıkış");
    cikisAct->setShortcut(QKeySequence::Quit);
    connect(cikisAct, &QAction::triggered,
            this, &MainWindow::close);

    // Veri menusu
    auto* veriMenu = menuBar()->addMenu(
        "&Veri");

    auto* ogrEkle = veriMenu->addAction(
        QIcon(":/resources/icons/user-plus.svg"), "Öğrenci Ekle...");
    connect(ogrEkle, &QAction::triggered,
            this,
            &MainWindow::ogrenciEkleDialogAc);

    auto* dersEkle = veriMenu->addAction(
        QIcon(":/resources/icons/book-plus.svg"), "Ders Ekle...");
    connect(dersEkle, &QAction::triggered,
            this,
            &MainWindow::dersEkleDialogAc);

    veriMenu->addSeparator();

    auto* notEkleAct = veriMenu->addAction(
        QIcon(":/resources/icons/circle-plus.svg"), "Not Ekle...");
    notEkleAct->setShortcut(
        QKeySequence(Qt::CTRL | Qt::SHIFT
                     | Qt::Key_N));
    connect(notEkleAct, &QAction::triggered,
            this,
            &MainWindow::notEkleDialogAc);

    // Ayarlar (Asama 5)
    veriMenu->addSeparator();
    auto* ayarlarAct = veriMenu->addAction(
        QIcon(":/resources/icons/settings.svg"), "Ayarlar...");
    connect(ayarlarAct, &QAction::triggered,
            this, &MainWindow::ayarlariGoster);

    // Gorunum menusu
    auto* gorunumMenu =
        menuBar()->addMenu("Görünüm");

    auto* temaGrup = new QActionGroup(this);
    temaGrup->setExclusive(true);

    auto* aydinlikAct = gorunumMenu->addAction(
        QIcon(":/resources/icons/sun.svg"), "Aydınlık Tema");
    aydinlikAct->setCheckable(true);
    aydinlikAct->setShortcut(
        QKeySequence(Qt::CTRL | Qt::Key_1));
    temaGrup->addAction(aydinlikAct);
    connect(aydinlikAct, &QAction::triggered,
        this, [this]{ temayiUygula("light"); });

    auto* karanlikAct = gorunumMenu->addAction(
        QIcon(":/resources/icons/moon.svg"), "Karanlık Tema");
    karanlikAct->setCheckable(true);
    karanlikAct->setShortcut(
        QKeySequence(Qt::CTRL | Qt::Key_2));
    temaGrup->addAction(karanlikAct);
    connect(karanlikAct, &QAction::triggered,
        this, [this]{ temayiUygula("dark"); });

    // Mevcut tema durumunu yansit
    QSettings s;
    const QString tema =
        s.value("gorunum/tema", "light").toString();
    aydinlikAct->setChecked(tema == "light");
    karanlikAct->setChecked(tema == "dark");

    // Yardim menusu
    auto* yardimMenu =
        menuBar()->addMenu("Yardım");
    auto* hakkindaAct = yardimMenu->addAction(
        QIcon(":/resources/icons/help-circle.svg"), "Hakkında...");
    connect(hakkindaAct, &QAction::triggered,
            this, &MainWindow::hakkindaGoster);
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

void MainWindow::disaAktarJson() {
    QString yol =
        QFileDialog::getSaveFileName(
            this, "JSON Aktar", "obys.json",
            "JSON (*.json)");
    if (yol.isEmpty()) return;
    if (!yol.endsWith(".json",
            Qt::CaseInsensitive))
        yol += ".json";

    JsonExporter dis;
    if (!dis.disaAktar(yol, m_servis)) {
        QMessageBox::warning(this, "Hata",
            dis.sonHata());
        return;
    }
    statusBar()->showMessage(
        "JSON: " + yol, 3000);
}

void MainWindow::ayarlariGoster() {
    QDialog dlg(this);
    Ui::AyarlarDialog ui;
    ui.setupUi(&dlg);

    // Mevcut ayarlari yukle
    QSettings s;
    const QString tema =
        s.value("gorunum/tema", "light")
         .toString();
    ui.aydinlikRadio->setChecked(
        tema == "light");
    ui.karanlikRadio->setChecked(
        tema == "dark");
    ui.otomatikKaydetSpin->setValue(
        s.value("kayit/otomatikDakika", 5)
         .toInt());
    ui.sonAcilanSayiSpin->setValue(
        s.value("dosya/maxSonAcilan", 5)
         .toInt());

    connect(ui.buttonBox,
            &QDialogButtonBox::accepted,
            &dlg, &QDialog::accept);
    connect(ui.buttonBox,
            &QDialogButtonBox::rejected,
            &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    // Ayarlari kaydet
    const QString yeniTema =
        ui.karanlikRadio->isChecked()
            ? "dark" : "light";
    s.setValue("gorunum/tema", yeniTema);
    s.setValue("kayit/otomatikDakika",
              ui.otomatikKaydetSpin->value());
    s.setValue("dosya/maxSonAcilan",
              ui.sonAcilanSayiSpin->value());

    statusBar()->showMessage(
        "Ayarlar kaydedildi", 2000);
}

void MainWindow::transkriptYazdir() {
    const auto ogrenciler =
        m_servis.tumOgrenciler();
    if (ogrenciler.isEmpty()) {
        QMessageBox::information(this,
            "Transkript", "Liste boş.");
        return;
    }

    QStringList isimler;
    for (const auto& o : ogrenciler)
        isimler << QString("%1 - %2")
                       .arg(o.numara, o.adSoyad);

    bool ok = false;
    const QString secim =
        QInputDialog::getItem(
            this, "Transkript",
            "Öğrenci seçin:", isimler,
            0, false, &ok);
    if (!ok) return;

    const int idx = isimler.indexOf(secim);
    if (idx < 0) return;
    const Ogrenci& ogr = ogrenciler.at(idx);

    TranskriptUretici uretici;
    auto* belge = new QTextDocument();
    belge->setHtml(uretici.html(
        ogr, m_servis.tumDersler(),
        m_servis.tumNotlar()));

    QPrinter yazici(QPrinter::HighResolution);
    QPrintPreviewDialog onizleme(
        &yazici, this);
    connect(&onizleme,
        &QPrintPreviewDialog::paintRequested,
        this, [belge](QPrinter* p) {
            belge->print(p);
        });
    onizleme.exec();
    belge->deleteLater();
}

void MainWindow::temayiUygula(
    const QString& temaAd) {
    const QString yol =
        (temaAd == "dark")
            ? ":/resources/themes/dark.qss"
            : ":/resources/themes/light.qss";

    QFile dosya(yol);
    QString qss;
    if (dosya.open(QIODevice::ReadOnly
                   | QIODevice::Text)) {
        QTextStream akis(&dosya);
        qss = akis.readAll();
    }
    qApp->setStyleSheet(qss);

    QSettings s;
    s.setValue("gorunum/tema", temaAd);
    statusBar()->showMessage(
        "Tema: " + temaAd, 1500);
}

void MainWindow::hakkindaGoster() {
    QMessageBox::about(this, "Hakkında",
        "<h3>OBYS Tutorial v1.0</h3>"
        "<p>Öğrenci Bilgi Yönetim Sistemi</p>"
        "<p>KTU Bilgisayar Bilimleri - "
        "Yazılım Geliştirme II</p>");
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

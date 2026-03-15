/**
 * OBYS Ana Pencere - Tam Uygulama (Bölüm 5 Kapanış Örneği)
 *
 * Bu program, Bölüm 5'te öğrenilen tüm kavramları bir araya getirir:
 * düzen yöneticileri, ana pencere bileşenleri, menüler, araç çubuğu,
 * durum çubuğu, dock widget ve diyalog pencereleri. OBYS projesinin
 * Qt GUI arayüzünün temel iskeletini oluşturur.
 *
 * Öğrenilecek kavramlar:
 * - QMainWindow alt sınıfı ile tam uygulama mimarisi
 * - Header/source ayrımı ve Q_OBJECT
 * - Menü, araç çubuğu, durum çubuğu entegrasyonu
 * - Dock widget ile gezinme paneli
 * - Merkezi widget olarak kayıt formu
 * - QAction ve kısayol tuşları
 * - Diyalog pencereleri ile kullanıcı etkileşimi
 *
 * Bölüm: 05 - Qt Pencere ve Düzen Yönetimi
 * Ünite: 4 - Qt Designer ve UI Dosyaları
 *
 * Derleme (CMake önerilir, CMakeLists.txt dosyasına bakınız):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 */

#include "02_obys_ana_pencere.h"

#include <QApplication>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

// =============================================
// YAPILANDIRICI
// =============================================
ObysAnaPencere::ObysAnaPencere(QWidget* parent)
    : QMainWindow(parent) {

    setWindowTitle("OBYS - Öğrenci Bilgi Yönetim Sistemi");
    resize(900, 650);
    setMinimumSize(700, 500);

    // Arayüz bileşenlerini oluştur (sıra önemli)
    merkeziWidgetOlustur();
    menuOlustur();
    aracCubuguOlustur();
    durumCubuguOlustur();
    dockOlustur();
}

// =============================================
// MENÜ OLUŞTUR
// =============================================
void ObysAnaPencere::menuOlustur() {
    // -- Eylemler --
    yeniEylem_ = new QAction("&Yeni", this);
    yeniEylem_->setShortcut(QKeySequence::New);
    yeniEylem_->setStatusTip("Yeni kayıt formu");

    acEylem_ = new QAction("&Aç...", this);
    acEylem_->setShortcut(QKeySequence::Open);

    kaydetEylem_ = new QAction("&Kaydet", this);
    kaydetEylem_->setShortcut(QKeySequence::Save);

    cikisEylem_ = new QAction("Çı&kış", this);
    cikisEylem_->setShortcut(QKeySequence::Quit);
    connect(cikisEylem_, &QAction::triggered,
            qApp, &QApplication::quit);

    ogrenciEkleEylem_ = new QAction("Öğrenci &Ekle", this);
    ogrenciEkleEylem_->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_E));
    connect(ogrenciEkleEylem_, &QAction::triggered,
            this, &ObysAnaPencere::yeniOgrenci);

    ogrenciListeEylem_ = new QAction("Öğrenci &Listele", this);
    connect(ogrenciListeEylem_, &QAction::triggered,
            this, &ObysAnaPencere::ogrenciListele);

    dersEkleEylem_ = new QAction("Ders &Ekle", this);
    connect(dersEkleEylem_, &QAction::triggered,
            this, &ObysAnaPencere::dersEkle);

    dersListeEylem_ = new QAction("Ders &Listele", this);
    connect(dersListeEylem_, &QAction::triggered,
            this, &ObysAnaPencere::dersListele);

    hakkindaEylem_ = new QAction("&Hakkında", this);
    connect(hakkindaEylem_, &QAction::triggered,
            this, &ObysAnaPencere::hakkinda);

    // -- Menü Çubuğu --
    dosyaMenu_ = menuBar()->addMenu("&Dosya");
    dosyaMenu_->addAction(yeniEylem_);
    dosyaMenu_->addAction(acEylem_);
    dosyaMenu_->addAction(kaydetEylem_);
    dosyaMenu_->addSeparator();
    dosyaMenu_->addAction(cikisEylem_);

    ogrenciMenu_ = menuBar()->addMenu("Ö&ğrenci");
    ogrenciMenu_->addAction(ogrenciEkleEylem_);
    ogrenciMenu_->addAction(ogrenciListeEylem_);

    dersMenu_ = menuBar()->addMenu("&Ders");
    dersMenu_->addAction(dersEkleEylem_);
    dersMenu_->addAction(dersListeEylem_);

    yardimMenu_ = menuBar()->addMenu("&Yardım");
    yardimMenu_->addAction(hakkindaEylem_);
}

// =============================================
// ARAÇ ÇUBUĞU OLUŞTUR
// =============================================
void ObysAnaPencere::aracCubuguOlustur() {
    anaAracCubugu_ = addToolBar("Ana Araç Çubuğu");
    anaAracCubugu_->addAction(yeniEylem_);
    anaAracCubugu_->addAction(kaydetEylem_);
    anaAracCubugu_->addSeparator();
    anaAracCubugu_->addAction(ogrenciEkleEylem_);
    anaAracCubugu_->addAction(ogrenciListeEylem_);
    anaAracCubugu_->addSeparator();
    anaAracCubugu_->addAction(dersEkleEylem_);
}

// =============================================
// DURUM ÇUBUĞU OLUŞTUR
// =============================================
void ObysAnaPencere::durumCubuguOlustur() {
    statusBar()->showMessage("OBYS hazır", 5000);

    kayitSayisiEtiket_ = new QLabel("Kayıt: 0");
    kayitSayisiEtiket_->setStyleSheet("padding: 0 10px; color: #2c3e50;");
    statusBar()->addPermanentWidget(kayitSayisiEtiket_);

    QLabel* surumEtiket = new QLabel("v1.0");
    surumEtiket->setStyleSheet(
        "padding: 2px 8px; background-color: #3498db; "
        "color: white; border-radius: 3px;"
    );
    statusBar()->addPermanentWidget(surumEtiket);
}

// =============================================
// DOCK WIDGET OLUŞTUR
// =============================================
void ObysAnaPencere::dockOlustur() {
    gezinmeDock_ = new QDockWidget("Gezinme", this);
    gezinmeDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    gezinmeListe_ = new QListWidget();
    gezinmeListe_->addItem("Öğrenci Kayıt");
    gezinmeListe_->addItem("Öğrenci Listesi");
    gezinmeListe_->addItem("Ders Ekleme");
    gezinmeListe_->addItem("Ders Listesi");
    gezinmeListe_->addItem("Not Girişi");
    gezinmeListe_->addItem("Raporlar");
    gezinmeListe_->setStyleSheet(
        "QListWidget { font-size: 13px; }"
        "QListWidget::item { padding: 8px; }"
        "QListWidget::item:selected { "
        "  background-color: #3498db; color: white; }"
    );

    gezinmeDock_->setWidget(gezinmeListe_);
    addDockWidget(Qt::LeftDockWidgetArea, gezinmeDock_);

    // Gezinme listesi seçim değişimi
    connect(gezinmeListe_, &QListWidget::currentTextChanged,
            this, [this](const QString& secilen) {
        statusBar()->showMessage(
            QString("Modül: %1").arg(secilen), 3000
        );
        qDebug() << "Gezinme:" << secilen;
    });
}

// =============================================
// MERKEZİ WIDGET OLUŞTUR (Kayıt Formu)
// =============================================
void ObysAnaPencere::merkeziWidgetOlustur() {
    QWidget* merkeziWidget = new QWidget();
    QVBoxLayout* anaDuzen = new QVBoxLayout(merkeziWidget);
    anaDuzen->setSpacing(12);
    anaDuzen->setContentsMargins(20, 20, 20, 20);

    // -- Başlık --
    QLabel* baslik = new QLabel("Yeni Öğrenci Kaydı");
    baslik->setAlignment(Qt::AlignCenter);
    baslik->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #2c3e50; "
        "padding: 12px; background-color: #eaf2f8; border-radius: 6px;"
    );
    anaDuzen->addWidget(baslik);

    // -- Form Alanları --
    QGroupBox* formGrup = new QGroupBox("Öğrenci Bilgileri");
    QFormLayout* formDuzen = new QFormLayout(formGrup);
    formDuzen->setSpacing(10);

    isimGiris_ = new QLineEdit();
    isimGiris_->setPlaceholderText("Örneğin: Ayşe Yılmaz");
    isimGiris_->setMaxLength(60);
    formDuzen->addRow("Ad Soyad:", isimGiris_);

    numaraGiris_ = new QSpinBox();
    numaraGiris_->setRange(1000, 9999);
    numaraGiris_->setValue(1000);
    formDuzen->addRow("Öğrenci No:", numaraGiris_);

    bolumSecim_ = new QComboBox();
    bolumSecim_->addItems({
        "-- Bölüm Seçiniz --",
        "Bilgisayar Bilimleri",
        "Yazılım Mühendisliği",
        "Elektrik-Elektronik Müh.",
        "Matematik",
        "Fizik"
    });
    formDuzen->addRow("Bölüm:", bolumSecim_);

    anaDuzen->addWidget(formGrup);

    // -- Durum Etiketi --
    durumEtiket_ = new QLabel("");
    durumEtiket_->setAlignment(Qt::AlignCenter);
    durumEtiket_->setWordWrap(true);
    durumEtiket_->setStyleSheet("font-size: 12px; padding: 8px; border-radius: 4px;");
    anaDuzen->addWidget(durumEtiket_);

    // -- Butonlar --
    QHBoxLayout* butonDuzen = new QHBoxLayout();

    kaydetButon_ = new QPushButton("Kaydet");
    kaydetButon_->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; "
        "font-size: 15px; font-weight: bold; padding: 10px 30px; "
        "border-radius: 6px; border: none; }"
        "QPushButton:hover { background-color: #219a52; }"
    );

    temizleButon_ = new QPushButton("Temizle");
    temizleButon_->setStyleSheet(
        "QPushButton { background-color: #7f8c8d; color: white; "
        "font-size: 15px; padding: 10px 30px; "
        "border-radius: 6px; border: none; }"
        "QPushButton:hover { background-color: #6c7a7b; }"
    );

    butonDuzen->addWidget(kaydetButon_);
    butonDuzen->addWidget(temizleButon_);
    anaDuzen->addLayout(butonDuzen);

    anaDuzen->addStretch();

    setCentralWidget(merkeziWidget);

    // -- Kaydet butonu bağlantısı --
    connect(kaydetButon_, &QPushButton::clicked, this, [this]() {
        QString isim = isimGiris_->text().trimmed();

        if (isim.isEmpty()) {
            durumEtiket_->setText("[HATA] Öğrenci adı boş bırakılamaz!");
            durumEtiket_->setStyleSheet(
                "font-size: 12px; padding: 8px; border-radius: 4px; "
                "background-color: #fadbd8; color: #e74c3c;"
            );
            isimGiris_->setFocus();
            return;
        }

        if (bolumSecim_->currentIndex() == 0) {
            durumEtiket_->setText("[HATA] Lütfen bir bölüm seçiniz!");
            durumEtiket_->setStyleSheet(
                "font-size: 12px; padding: 8px; border-radius: 4px; "
                "background-color: #fadbd8; color: #e74c3c;"
            );
            bolumSecim_->setFocus();
            return;
        }

        // Başarılı kayıt
        kayitSayaci_++;
        kayitSayisiEtiket_->setText(QString("Kayıt: %1").arg(kayitSayaci_));

        QString mesaj = QString("[OK] %1 (No: %2) - %3 kaydedildi")
                        .arg(isim)
                        .arg(numaraGiris_->value())
                        .arg(bolumSecim_->currentText());

        durumEtiket_->setText(mesaj);
        durumEtiket_->setStyleSheet(
            "font-size: 12px; padding: 8px; border-radius: 4px; "
            "background-color: #d5f5e3; color: #27ae60;"
        );

        statusBar()->showMessage(mesaj, 5000);
        qDebug() << mesaj;

        QMessageBox::information(this, "Kayıt Başarılı",
                                  QString("%1 başarıyla kaydedildi.\n"
                                          "Öğrenci No: %2\nBölüm: %3")
                                  .arg(isim)
                                  .arg(numaraGiris_->value())
                                  .arg(bolumSecim_->currentText()));
    });

    // -- Temizle butonu bağlantısı --
    connect(temizleButon_, &QPushButton::clicked, this, [this]() {
        isimGiris_->clear();
        numaraGiris_->setValue(1000);
        bolumSecim_->setCurrentIndex(0);
        durumEtiket_->setText("");
        durumEtiket_->setStyleSheet("font-size: 12px; padding: 8px; border-radius: 4px;");
        isimGiris_->setFocus();
        statusBar()->showMessage("Form temizlendi", 3000);
    });
}

// =============================================
// SLOT UYGULAMALARI
// =============================================

void ObysAnaPencere::yeniOgrenci() {
    // Formu temizle ve odağı isim alanına ver
    isimGiris_->clear();
    numaraGiris_->setValue(1000);
    bolumSecim_->setCurrentIndex(0);
    durumEtiket_->setText("");
    isimGiris_->setFocus();
    statusBar()->showMessage("Yeni öğrenci kaydı hazır", 3000);
    qDebug() << "[OK] Yeni öğrenci formu açıldı";
}

void ObysAnaPencere::ogrenciListele() {
    QMessageBox::information(this, "Öğrenci Listesi",
                              QString("Toplam %1 öğrenci kayıtlı.\n\n"
                                      "(Tam liste görünümü Bölüm 6'da\n"
                                      "Model/View mimarisi ile gelecek)")
                              .arg(kayitSayaci_));
    qDebug() << "[OK] Öğrenci listesi gösterildi";
}

void ObysAnaPencere::dersEkle() {
    bool tamam;
    QString dersAdi = QInputDialog::getText(
        this, "Ders Ekle",
        "Ders adını giriniz:",
        QLineEdit::Normal,
        "",
        &tamam
    );

    if (tamam && !dersAdi.isEmpty()) {
        statusBar()->showMessage(
            QString("[OK] Ders eklendi: %1").arg(dersAdi), 5000
        );
        qDebug() << "[OK] Ders eklendi:" << dersAdi;
    }
}

void ObysAnaPencere::dersListele() {
    QMessageBox::information(this, "Ders Listesi",
                              "Kayıtlı dersler:\n\n"
                              "1. Yazılım Geliştirme II\n"
                              "2. Veri Yapıları\n"
                              "3. Algoritmalar\n\n"
                              "(Tam liste Model/View ile gelecek)");
    qDebug() << "[OK] Ders listesi gösterildi";
}

void ObysAnaPencere::hakkinda() {
    QMessageBox::about(this, "OBYS Hakkında",
                        "OBYS - Öğrenci Bilgi Yönetim Sistemi\n"
                        "Sürüm: 1.0\n\n"
                        "Yazılım Geliştirme II Dersi\n"
                        "Bölüm 5 - Qt Pencere ve Düzen Yönetimi\n\n"
                        "Bu uygulama eğitim amaçlıdır.");
}

// =============================================
// ANA PROGRAM
// =============================================
int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    ObysAnaPencere pencere;
    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "OBYS - Öğrenci Bilgi Yönetim Sistemi"
 * Boyut: 900x650 piksel (minimum: 700x500)
 *
 * Yapı:
 *   +--[Menü Çubuğu]-----------------------------------+
 *   | Dosya | Öğrenci | Ders | Yardım                  |
 *   +--[Araç Çubuğu]-----------------------------------+
 *   | [Yeni][Kaydet] | [Öğrenci Ekle][Liste] | [Ders+] |
 *   +-----------+--------------------------------------+
 *   | Gezinme   |  "Yeni Öğrenci Kaydı"               |
 *   | [Liste]   |                                      |
 *   | - Öğrenci |  Ad Soyad:    [___________]          |
 *   |   Kayıt   |  Öğrenci No:  [1000]                |
 *   | - Öğrenci |  Bölüm:       [-- Seçiniz --]       |
 *   |   Listesi |                                      |
 *   | - Ders    |  [durum etiketi]                     |
 *   |   Ekleme  |                                      |
 *   | - ...     |  [Kaydet]  [Temizle]                 |
 *   +-----------+--------------------------------------+
 *   | Durum Çubuğu          | Kayıt: 0 | v1.0         |
 *   +----------------------------------------------+
 *
 * Menüler:
 *   Dosya: Yeni (Ctrl+N), Aç, Kaydet (Ctrl+S), Çıkış (Ctrl+Q)
 *   Öğrenci: Ekle (Ctrl+Shift+E), Listele
 *   Ders: Ekle, Listele
 *   Yardım: Hakkında
 *
 * Davranış:
 *   - Kayıt formu: Ad, numara, bölüm giriş alanları
 *   - Kaydet: Doğrulama -> başarılı kayıt -> sayaç artar
 *   - Temizle: Form sıfırlanır
 *   - Gezinme paneli: Sol dock, sürüklenebilir
 *   - Durum çubuğu: Geçici mesajlar + kalıcı kayıt sayacı
 *   - Ders ekleme: QInputDialog ile metin girişi
 *
 * Konsol çıktısı (örnek):
 *   [OK] Yeni öğrenci formu açıldı
 *   [OK] Ayşe Yılmaz (No: 2045) - Bilgisayar Bilimleri kaydedildi
 *   [OK] Ders eklendi: Yazılım Geliştirme II
 */

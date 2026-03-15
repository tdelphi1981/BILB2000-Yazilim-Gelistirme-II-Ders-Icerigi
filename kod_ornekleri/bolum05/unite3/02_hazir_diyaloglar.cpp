/**
 * Hazır Diyalog Pencereleri - QMessageBox, QFileDialog, QInputDialog
 *
 * Bu program, Qt'nin sağladığı hazır diyalog pencerelerinin kullanımını
 * gösterir. Bilgi, uyarı, hata ve soru mesaj kutuları, dosya açma/kaydetme
 * diyalogları ve giriş diyalogları ile kullanıcı etkileşimi sağlanır.
 *
 * Öğrenilecek kavramlar:
 * - QMessageBox::information(), warning(), critical(), question()
 * - QFileDialog::getOpenFileName(), getSaveFileName()
 * - QInputDialog::getText(), getInt(), getItem()
 * - Standart butonlar ve dönüş değerleri
 * - Diyalog sonuçlarını işleme
 *
 * Bölüm: 05 - Qt Pencere ve Düzen Yönetimi
 * Ünite: 3 - Diyalog Pencereleri
 *
 * Derleme (CMake önerilir, CMakeLists.txt dosyasına bakınız):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 *
 * Doğrudan derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 02_hazir_diyaloglar 02_hazir_diyaloglar.cpp
 */

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QMainWindow anaPencere;
    anaPencere.setWindowTitle("OBYS - Hazır Diyalog Pencereleri");
    anaPencere.resize(700, 550);

    // Ana widget ve düzen
    QWidget* merkeziWidget = new QWidget();
    QVBoxLayout* anaDuzen = new QVBoxLayout(merkeziWidget);
    anaDuzen->setSpacing(10);
    anaDuzen->setContentsMargins(15, 15, 15, 15);

    // =============================================
    // 1) MESAJ KUTULARI (QMessageBox)
    // =============================================
    QGroupBox* mesajGrup = new QGroupBox("QMessageBox Örnekleri");
    QHBoxLayout* mesajDuzen = new QHBoxLayout(mesajGrup);

    QPushButton* bilgiBtn = new QPushButton("Bilgi");
    bilgiBtn->setStyleSheet("background-color: #3498db; color: white; padding: 8px;");

    QPushButton* uyariBtn = new QPushButton("Uyarı");
    uyariBtn->setStyleSheet("background-color: #f39c12; color: white; padding: 8px;");

    QPushButton* hataBtn = new QPushButton("Hata");
    hataBtn->setStyleSheet("background-color: #e74c3c; color: white; padding: 8px;");

    QPushButton* soruBtn = new QPushButton("Soru");
    soruBtn->setStyleSheet("background-color: #27ae60; color: white; padding: 8px;");

    mesajDuzen->addWidget(bilgiBtn);
    mesajDuzen->addWidget(uyariBtn);
    mesajDuzen->addWidget(hataBtn);
    mesajDuzen->addWidget(soruBtn);
    anaDuzen->addWidget(mesajGrup);

    // =============================================
    // 2) DOSYA DİYALOGLARI (QFileDialog)
    // =============================================
    QGroupBox* dosyaGrup = new QGroupBox("QFileDialog Örnekleri");
    QHBoxLayout* dosyaDuzen = new QHBoxLayout(dosyaGrup);

    QPushButton* dosyaAcBtn = new QPushButton("Dosya Aç...");
    dosyaAcBtn->setStyleSheet("padding: 8px;");

    QPushButton* dosyaKaydetBtn = new QPushButton("Farklı Kaydet...");
    dosyaKaydetBtn->setStyleSheet("padding: 8px;");

    dosyaDuzen->addWidget(dosyaAcBtn);
    dosyaDuzen->addWidget(dosyaKaydetBtn);
    anaDuzen->addWidget(dosyaGrup);

    // =============================================
    // 3) GİRİŞ DİYALOGLARI (QInputDialog)
    // =============================================
    QGroupBox* girisGrup = new QGroupBox("QInputDialog Örnekleri");
    QHBoxLayout* girisDuzen = new QHBoxLayout(girisGrup);

    QPushButton* metinGirisBtn = new QPushButton("Metin Girişi");
    metinGirisBtn->setStyleSheet("padding: 8px;");

    QPushButton* sayiGirisBtn = new QPushButton("Sayı Girişi");
    sayiGirisBtn->setStyleSheet("padding: 8px;");

    girisDuzen->addWidget(metinGirisBtn);
    girisDuzen->addWidget(sayiGirisBtn);
    anaDuzen->addWidget(girisGrup);

    // =============================================
    // 4) LOG ALANI
    // =============================================
    QTextEdit* log = new QTextEdit();
    log->setReadOnly(true);
    log->setStyleSheet(
        "font-family: 'Consolas', monospace; font-size: 12px; "
        "background-color: #2c3e50; color: #ecf0f1; padding: 8px;"
    );
    log->append("[OK] Uygulama başlatıldı - Diyalog butonlarını deneyin\n");
    anaDuzen->addWidget(log);

    anaPencere.setCentralWidget(merkeziWidget);

    // =============================================
    // 5) SİNYAL/SLOT BAĞLANTILARI
    // =============================================

    // -- Bilgi mesajı --
    QObject::connect(bilgiBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        QMessageBox::information(&anaPencere, "Bilgi",
                                 "Kayıt işlemi başarıyla tamamlandı.\n"
                                 "Öğrenci sisteme eklendi.");
        log->append("[OK] Bilgi diyalogu gösterildi");
    });

    // -- Uyarı mesajı --
    QObject::connect(uyariBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        QMessageBox::warning(&anaPencere, "Uyarı",
                             "Öğrenci numarası zaten mevcut!\n"
                             "Lütfen farklı bir numara giriniz.");
        log->append("[UYARI] Uyarı diyalogu gösterildi");
    });

    // -- Hata mesajı --
    QObject::connect(hataBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        QMessageBox::critical(&anaPencere, "Hata",
                              "Veritabanı bağlantısı kurulamadı!\n"
                              "Lütfen ağ ayarlarını kontrol edin.");
        log->append("[HATA] Hata diyalogu gösterildi");
    });

    // -- Soru mesajı (Evet/Hayır) --
    QObject::connect(soruBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        auto sonuc = QMessageBox::question(
            &anaPencere, "Onay",
            "Bu öğrenciyi silmek istediğinize emin misiniz?\n"
            "Bu işlem geri alınamaz!",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No  // Varsayılan buton
        );

        if (sonuc == QMessageBox::Yes) {
            log->append("[OK] Kullanıcı 'Evet' dedi - silme onaylandı");
        } else {
            log->append("[UYARI] Kullanıcı 'Hayır' dedi - silme iptal edildi");
        }
    });

    // -- Dosya aç --
    QObject::connect(dosyaAcBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        QString dosyaYolu = QFileDialog::getOpenFileName(
            &anaPencere,
            "Dosya Aç",
            QString(),  // Başlangıç dizini (boş = varsayılan)
            "Metin Dosyaları (*.txt);;C++ Dosyaları (*.cpp *.h);;Tüm Dosyalar (*)"
        );

        if (!dosyaYolu.isEmpty()) {
            log->append(QString("[OK] Dosya seçildi: %1").arg(dosyaYolu));
        } else {
            log->append("[UYARI] Dosya seçimi iptal edildi");
        }
    });

    // -- Dosya kaydet --
    QObject::connect(dosyaKaydetBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        QString dosyaYolu = QFileDialog::getSaveFileName(
            &anaPencere,
            "Farklı Kaydet",
            "ogrenci_listesi.txt",
            "Metin Dosyaları (*.txt);;CSV Dosyaları (*.csv)"
        );

        if (!dosyaYolu.isEmpty()) {
            log->append(QString("[OK] Kayıt yolu: %1").arg(dosyaYolu));
        } else {
            log->append("[UYARI] Kaydetme iptal edildi");
        }
    });

    // -- Metin girişi --
    QObject::connect(metinGirisBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        bool tamam;
        QString isim = QInputDialog::getText(
            &anaPencere,
            "Öğrenci Adı",
            "Öğrenci adını giriniz:",
            QLineEdit::Normal,
            "Ayşe Yılmaz",
            &tamam
        );

        if (tamam && !isim.isEmpty()) {
            log->append(QString("[OK] Girilen isim: %1").arg(isim));
        } else {
            log->append("[UYARI] Metin girişi iptal edildi");
        }
    });

    // -- Sayı girişi --
    QObject::connect(sayiGirisBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        bool tamam;
        int numara = QInputDialog::getInt(
            &anaPencere,
            "Öğrenci Numarası",
            "Öğrenci numarasını giriniz:",
            1000,   // Varsayılan değer
            1000,   // Minimum
            9999,   // Maksimum
            1,      // Adım
            &tamam
        );

        if (tamam) {
            log->append(QString("[OK] Girilen numara: %1").arg(numara));
        } else {
            log->append("[UYARI] Sayı girişi iptal edildi");
        }
    });

    anaPencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "OBYS - Hazır Diyalog Pencereleri"
 * Boyut: 700x550 piksel
 *
 * İçerik:
 *   [QMessageBox Örnekleri]
 *     [Bilgi] [Uyarı] [Hata] [Soru]
 *
 *   [QFileDialog Örnekleri]
 *     [Dosya Aç...] [Farklı Kaydet...]
 *
 *   [QInputDialog Örnekleri]
 *     [Metin Girişi] [Sayı Girişi]
 *
 *   [Log Alanı - koyu arka plan]
 *     [OK] Uygulama başlatıldı - Diyalog butonlarını deneyin
 *
 * Davranış:
 *   - "Bilgi": Mavi bilgi ikonu ile mesaj kutusu
 *   - "Uyarı": Sarı uyarı ikonu ile mesaj kutusu
 *   - "Hata": Kırmızı hata ikonu ile mesaj kutusu
 *   - "Soru": Evet/Hayır butonlu soru kutusu
 *   - "Dosya Aç": Dosya seçim diyalogu (filtre: .txt, .cpp, .h)
 *   - "Farklı Kaydet": Kayıt yeri seçim diyalogu
 *   - "Metin Girişi": Metin giriş kutusu (varsayılan: Ayşe Yılmaz)
 *   - "Sayı Girişi": Sayı giriş kutusu (1000-9999 arası)
 *   - Her işlem sonucu log alanında gösterilir
 */

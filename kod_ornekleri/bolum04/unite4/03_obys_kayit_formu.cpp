/**
 * OBYS Öğrenci Kayıt Formu - Doğrulama ile Tam Form Örneği
 *
 * Bu program, OBYS (Öğrenci Bilgi Yönetim Sistemi) bağlamında
 * bir öğrenci kayıt formunu gösterir. Ad soyad, öğrenci numarası,
 * bölüm seçimi ve kaydet butonu içeren form, doğrulama (validation)
 * ile birlikte çalışmaktadır.
 *
 * Öğrenilecek kavramlar:
 * - QLineEdit ile metin girişi ve doğrulama
 * - QSpinBox ile sınırlı sayısal giriş
 * - QComboBox ile seçim
 * - QPushButton ile form gönderimi
 * - QLabel ile durum/hata mesajları
 * - Form doğrulama (validation) mantığı
 * - QFormLayout ile form düzeni
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 4 - Temel Widget'lar
 *
 * Derleme (CMake önerilir, CMakeLists.txt dosyasına bakınız):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 *
 * Doğrudan derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 03_obys_kayit_formu 03_obys_kayit_formu.cpp
 */

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) ANA PENCERE
    // =============================================
    QWidget pencere;
    pencere.setWindowTitle("OBYS - Öğrenci Kayıt Formu");
    pencere.setMinimumWidth(450);

    QVBoxLayout* anaDuzen = new QVBoxLayout(&pencere);
    anaDuzen->setSpacing(12);
    anaDuzen->setContentsMargins(20, 20, 20, 20);

    // =============================================
    // 2) BASLIK
    // =============================================
    QLabel* baslikEtiket = new QLabel("Yeni Öğrenci Kaydı");
    baslikEtiket->setAlignment(Qt::AlignCenter);
    baslikEtiket->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #2c3e50; "
        "padding: 10px; background-color: #eaf2f8; "
        "border-radius: 6px;"
    );
    anaDuzen->addWidget(baslikEtiket);

    // =============================================
    // 3) FORM ALANLARI
    // =============================================
    // QFormLayout: Etiket-widget çiftlerini düzgün hizalar
    QFormLayout* formDuzen = new QFormLayout();
    formDuzen->setSpacing(10);

    // -- Ad Soyad --
    QLineEdit* isimGiris = new QLineEdit();
    isimGiris->setPlaceholderText("Örneğin: Ayşe Yılmaz");
    isimGiris->setMaxLength(60);
    isimGiris->setStyleSheet(
        "padding: 8px; border: 2px solid #bdc3c7; border-radius: 4px; "
        "font-size: 13px;"
    );
    formDuzen->addRow("Ad Soyad:", isimGiris);

    // -- Öğrenci Numarası --
    QSpinBox* numaraGiris = new QSpinBox();
    numaraGiris->setRange(1000, 9999);
    numaraGiris->setValue(1000);
    numaraGiris->setStyleSheet("padding: 8px; font-size: 13px;");
    formDuzen->addRow("Öğrenci No:", numaraGiris);

    // -- Bölüm Seçimi --
    QComboBox* bolumSecim = new QComboBox();
    bolumSecim->addItem("-- Bölüm Seçiniz --");
    bolumSecim->addItem("Bilgisayar Bilimleri");
    bolumSecim->addItem("Yazılım Mühendisliği");
    bolumSecim->addItem("Elektrik-Elektronik Müh.");
    bolumSecim->addItem("Matematik");
    bolumSecim->addItem("Fizik");
    bolumSecim->setStyleSheet(
        "QComboBox { padding: 8px; font-size: 13px; }"
        "QComboBox QAbstractItemView {"
        "  background-color: white; selection-background-color: #3498db;"
        "  selection-color: white;"
        "}"
    );
    formDuzen->addRow("Bölüm:", bolumSecim);

    anaDuzen->addLayout(formDuzen);

    // =============================================
    // 4) DURUM ETİKETİ
    // =============================================
    QLabel* durumEtiket = new QLabel("");
    durumEtiket->setAlignment(Qt::AlignCenter);
    durumEtiket->setWordWrap(true);
    durumEtiket->setStyleSheet(
        "font-size: 12px; padding: 8px; border-radius: 4px;"
    );
    anaDuzen->addWidget(durumEtiket);

    // =============================================
    // 5) BUTONLAR
    // =============================================
    QHBoxLayout* butonDuzen = new QHBoxLayout();

    QPushButton* kaydetButon = new QPushButton("Kaydet");
    kaydetButon->setStyleSheet(
        "QPushButton {"
        "  background-color: #27ae60; color: white;"
        "  font-size: 15px; font-weight: bold;"
        "  padding: 10px 30px; border-radius: 6px; border: none;"
        "}"
        "QPushButton:hover { background-color: #219a52; }"
        "QPushButton:pressed { background-color: #1a8044; }"
    );

    QPushButton* temizleButon = new QPushButton("Temizle");
    temizleButon->setStyleSheet(
        "QPushButton {"
        "  background-color: #7f8c8d; color: white;"
        "  font-size: 15px; padding: 10px 30px;"
        "  border-radius: 6px; border: none;"
        "}"
        "QPushButton:hover { background-color: #6c7a7b; }"
    );

    butonDuzen->addWidget(kaydetButon);
    butonDuzen->addWidget(temizleButon);
    anaDuzen->addLayout(butonDuzen);

    anaDuzen->addStretch();

    // =============================================
    // 6) SİNYAL/SLOT BAĞLANTILARI
    // =============================================

    // -- Kaydet butonu: doğrulama yap ve kaydet --
    QObject::connect(kaydetButon, &QPushButton::clicked,
                     &pencere, [&pencere, isimGiris, numaraGiris, bolumSecim, durumEtiket]() {
        QString isim = isimGiris->text().trimmed();
        int numara = numaraGiris->value();
        QString bolum = bolumSecim->currentText();

        // -- Doğrulama --

        // İsim kontrolü
        if (isim.isEmpty()) {
            durumEtiket->setText("[HATA] Öğrenci adı boş bırakılamaz!");
            durumEtiket->setStyleSheet(
                "font-size: 12px; padding: 8px; border-radius: 4px; "
                "background-color: #fadbd8; color: #e74c3c;"
            );
            isimGiris->setFocus();
            return;
        }

        // İsim uzunluk kontrolü
        if (isim.length() < 3) {
            durumEtiket->setText("[HATA] Ad soyad en az 3 karakter olmalı!");
            durumEtiket->setStyleSheet(
                "font-size: 12px; padding: 8px; border-radius: 4px; "
                "background-color: #fadbd8; color: #e74c3c;"
            );
            isimGiris->setFocus();
            return;
        }

        // Bölüm seçim kontrolü
        if (bolumSecim->currentIndex() == 0) {
            durumEtiket->setText("[HATA] Lütfen bir bölüm seçiniz!");
            durumEtiket->setStyleSheet(
                "font-size: 12px; padding: 8px; border-radius: 4px; "
                "background-color: #fadbd8; color: #e74c3c;"
            );
            bolumSecim->setFocus();
            return;
        }

        // -- Başarılı kayıt --
        QString mesaj = QString(
            "[OK] Kayıt başarılı!\n%1 (No: %2) - %3"
        ).arg(isim).arg(numara).arg(bolum);

        durumEtiket->setText(mesaj);
        durumEtiket->setStyleSheet(
            "font-size: 12px; padding: 8px; border-radius: 4px; "
            "background-color: #d5f5e3; color: #27ae60;"
        );

        // Konsola da yazdır
        qDebug() << "Öğrenci kaydedildi:"
                 << isim << "|" << numara << "|" << bolum;

        // Bilgi mesajı göster
        QMessageBox::information(
            &pencere,
            "Kayıt Başarılı",
            QString("%1 başarıyla kaydedildi.\n"
                    "Öğrenci No: %2\nBölüm: %3")
                .arg(isim).arg(numara).arg(bolum)
        );
    });

    // -- Temizle butonu: formu sıfırla --
    QObject::connect(temizleButon, &QPushButton::clicked,
                     &pencere, [isimGiris, numaraGiris, bolumSecim, durumEtiket]() {
        isimGiris->clear();
        numaraGiris->setValue(1000);
        bolumSecim->setCurrentIndex(0);
        durumEtiket->setText("");
        durumEtiket->setStyleSheet(
            "font-size: 12px; padding: 8px; border-radius: 4px;"
        );
        isimGiris->setFocus();
    });

    // -- Giriş alanlarında anlık geribildirim --
    QObject::connect(isimGiris, &QLineEdit::textChanged,
                     durumEtiket, [durumEtiket](const QString& metin) {
        if (metin.trimmed().isEmpty()) {
            durumEtiket->setText("");
            durumEtiket->setStyleSheet(
                "font-size: 12px; padding: 8px; border-radius: 4px;"
            );
        } else if (metin.trimmed().length() < 3) {
            durumEtiket->setText("İsim en az 3 karakter olmalı...");
            durumEtiket->setStyleSheet(
                "font-size: 12px; padding: 8px; border-radius: 4px; "
                "background-color: #fdebd0; color: #e67e22;"
            );
        } else {
            durumEtiket->setText(QString("Giriş: %1").arg(metin.trimmed()));
            durumEtiket->setStyleSheet(
                "font-size: 12px; padding: 8px; border-radius: 4px; "
                "background-color: #eaf2f8; color: #2980b9;"
            );
        }
    });

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "OBYS - Öğrenci Kayıt Formu"
 * Boyut: 450x400 piksel (sabit)
 *
 * İçerik:
 *   - "Yeni Öğrenci Kaydı" başlığı (mavi arka plan, büyük yazı)
 *   - Form alanları (QFormLayout ile hizalı):
 *       Ad Soyad:    [__________________] (placeholder: Örneğin: Ayşe Yılmaz)
 *       Öğrenci No:  [1000 ^v]            (SpinBox, 1000-9999 arası)
 *       Bölüm:       [-- Bölüm Seçiniz --] (ComboBox, 6 seçenek)
 *   - Durum etiketi (boş, doğrulama sonuçlarına göre renkli mesaj gösterir)
 *   - [Kaydet] (yeşil) ve [Temizle] (gri) butonları
 *
 * Davranış:
 *   - Ad soyad alanına yazarken anlık geribildirim:
 *     3 karakterden az: turuncu uyarı
 *     3+ karakter: mavi bilgi mesajı
 *   - "Kaydet" tıklandığında doğrulama:
 *     - Boş isim -> kırmızı hata
 *     - Kısa isim (< 3) -> kırmızı hata
 *     - Bölüm seçilmemiş -> kırmızı hata
 *     - Başarılı -> yeşil mesaj + QMessageBox bilgi penceresi
 *   - "Temizle" tıklandığında tüm alanlar sıfırlanır
 *
 * Konsol çıktısı (başarılı kayıt durumunda):
 *   Öğrenci kaydedildi: "Ayşe Yılmaz" | 1003 | "Bilgisayar Bilimleri"
 */

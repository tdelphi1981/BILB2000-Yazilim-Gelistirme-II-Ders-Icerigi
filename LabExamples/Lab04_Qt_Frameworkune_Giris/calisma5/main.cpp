/**
 * Lab 04 - Çalışma 5 Başlangıç Kodu
 * OBYS Öğrenci Kayıt Formu
 *
 * Bu dosya Lab 04 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - Tam bir form uygulaması tasarlama
 * - QFormLayout ile etiket-alan düzeni
 * - QLineEdit, QComboBox, QSpinBox birlikte kullanımı
 * - Form doğrulama kuralları
 * - QString::toInt() ile güvenli dönüşüm
 *
 * Lab: 04 - Qt Framework'üne Giriş
 * Çalışma: 5 - OBYS Kayıt Formu (20 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştırma: ./app
 */

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget pencere;
    pencere.setWindowTitle("OBYS - Baslangic");
    pencere.resize(500, 400);

    // === GOREV 5.1 - OBYS Ogrenci Kayit Formu (20 dk) ===
    //
    // Asagidaki alanlari iceren tam bir ogrenci kayit formu olusturun:
    //
    // TODO: a) Pencere basligini ayarlayin:
    //          pencere.setWindowTitle("OBYS -- Ogrenci Kayit Formu");
    //
    // TODO: b) Ana duzen ve form duzeni olusturun:
    //          auto* anaDuzen = new QVBoxLayout(&pencere);
    //          auto* formDuzen = new QFormLayout;
    //
    // TODO: c) Form alanlari ekleyin:
    //          - QLineEdit: Ogrenci No (sadece rakam, setMaxLength(9))
    //            auto* ogrNo = new QLineEdit;
    //            ogrNo->setMaxLength(9);
    //            ogrNo->setPlaceholderText("Ornek: 123456789");
    //            formDuzen->addRow("Ogrenci No:", ogrNo);
    //
    //          - QLineEdit: Ad Soyad
    //            auto* adSoyad = new QLineEdit;
    //            adSoyad->setPlaceholderText("Ad ve soyad girin...");
    //            formDuzen->addRow("Ad Soyad:", adSoyad);
    //
    //          - QComboBox: Bolum (en az 4 bolum)
    //            auto* bolum = new QComboBox;
    //            bolum->addItems({"Bilgisayar Bilimleri",
    //                             "Yazilim Muhendisligi",
    //                             "Elektrik-Elektronik",
    //                             "Makine Muhendisligi"});
    //            formDuzen->addRow("Bolum:", bolum);
    //
    //          - QSpinBox: Sinif (1-4)
    //            auto* sinif = new QSpinBox;
    //            sinif->setRange(1, 4);
    //            sinif->setValue(1);
    //            formDuzen->addRow("Sinif:", sinif);
    //
    //          - QComboBox: Yariyil (Guz / Bahar)
    //            auto* yariyil = new QComboBox;
    //            yariyil->addItems({"Guz", "Bahar"});
    //            formDuzen->addRow("Yariyil:", yariyil);
    //
    // TODO: d) Butonlar ekleyin: "Kaydet" ve "Temizle"
    //          auto* butonDuzen = new QHBoxLayout;
    //          auto* kaydetBtn = new QPushButton("Kaydet");
    //          auto* temizleBtn = new QPushButton("Temizle");
    //          butonDuzen->addWidget(kaydetBtn);
    //          butonDuzen->addWidget(temizleBtn);
    //
    // TODO: e) Sonuc etiketi ekleyin
    //          auto* sonuc = new QLabel;
    //          sonuc->setAlignment(Qt::AlignCenter);
    //
    // TODO: f) Duzenleri birlestirin:
    //          anaDuzen->addLayout(formDuzen);
    //          anaDuzen->addLayout(butonDuzen);
    //          anaDuzen->addWidget(sonuc);
    //
    // TODO: g) Dogrulama kurallari:
    //          - Ogrenci No bos olamaz ve en az 6 karakter olmali
    //          - Ad Soyad bos olamaz
    //          - Basarili -> yesil mesaj, hatali -> kirmizi mesaj
    //
    //          QObject::connect(kaydetBtn, &QPushButton::clicked,
    //              [=]() {
    //                  if (ogrNo->text().size() < 6) {
    //                      sonuc->setStyleSheet("color: red;");
    //                      sonuc->setText("Hata: Ogrenci No en az 6 karakter!");
    //                      return;
    //                  }
    //                  if (adSoyad->text().isEmpty()) {
    //                      sonuc->setStyleSheet("color: red;");
    //                      sonuc->setText("Hata: Ad Soyad bos olamaz!");
    //                      return;
    //                  }
    //                  sonuc->setStyleSheet("color: green; font-weight: bold;");
    //                  sonuc->setText(
    //                      QString("Kaydedildi: %1 - %2")
    //                          .arg(ogrNo->text()).arg(adSoyad->text()));
    //                  qDebug() << "Kayit:"
    //                           << ogrNo->text() << adSoyad->text()
    //                           << bolum->currentText() << sinif->value()
    //                           << yariyil->currentText();
    //              });
    //
    // TODO: h) Temizle butonu: tum alanlari sifirlasin
    // ============================================

    pencere.show();
    return app.exec();
}

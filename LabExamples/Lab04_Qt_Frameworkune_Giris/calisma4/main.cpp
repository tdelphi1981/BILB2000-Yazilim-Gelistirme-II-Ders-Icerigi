/**
 * Lab 04 - Çalışma 4 Başlangıç Kodu
 * Widget'larla Form Oluşturma
 *
 * Bu dosya Lab 04 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - QFormLayout ile form alanları oluşturma
 * - QLineEdit, QComboBox, QSpinBox widget'ları
 * - QVBoxLayout ve QHBoxLayout ile düzen yönetimi
 * - Sinyal/slot ile form etkileşimi
 * - Form doğrulama (validation)
 *
 * Lab: 04 - Qt Framework'üne Giriş
 * Çalışma: 4 - Widget'larla Form Oluşturma (30 dk)
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
    pencere.setWindowTitle("OBYS - Ders Kayit Formu");
    pencere.resize(450, 350);

    // Ana duzen: dikey
    auto* anaDuzen = new QVBoxLayout(&pencere);

    // Baslik
    auto* baslik = new QLabel("Ders Kayit Formu");
    baslik->setStyleSheet(
        "font-size: 16px; font-weight: bold; color: #008080;");
    baslik->setAlignment(Qt::AlignCenter);
    anaDuzen->addWidget(baslik);

    // Form alanlari
    auto* formDuzen = new QFormLayout;

    auto* dersAdi = new QLineEdit;
    dersAdi->setPlaceholderText("Ders adini girin...");
    formDuzen->addRow("Ders Adi:", dersAdi);

    auto* bolum = new QComboBox;
    bolum->addItems({"Bilgisayar Bilimleri",
                     "Yazilim Muhendisligi",
                     "Elektrik-Elektronik",
                     "Makine Muhendisligi"});
    formDuzen->addRow("Bolum:", bolum);

    auto* kredi = new QSpinBox;
    kredi->setRange(1, 6);
    kredi->setValue(3);
    formDuzen->addRow("Kredi:", kredi);

    anaDuzen->addLayout(formDuzen);

    // Butonlar (yatay duzen)
    auto* butonDuzen = new QHBoxLayout;
    auto* kaydetButon = new QPushButton("Kaydet");
    auto* temizleButon = new QPushButton("Temizle");
    butonDuzen->addWidget(kaydetButon);
    butonDuzen->addWidget(temizleButon);
    anaDuzen->addLayout(butonDuzen);

    // Sonuc etiketi
    auto* sonuc = new QLabel;
    sonuc->setAlignment(Qt::AlignCenter);
    anaDuzen->addWidget(sonuc);

    // Sinyal/slot baglantilari
    QObject::connect(kaydetButon, &QPushButton::clicked,
        [=]() {
            QString mesaj = QString("Kaydedildi: %1 (%2, %3 kredi)")
                .arg(dersAdi->text())
                .arg(bolum->currentText())
                .arg(kredi->value());
            sonuc->setText(mesaj);
            qDebug() << mesaj;
        });

    QObject::connect(temizleButon, &QPushButton::clicked,
        [=]() {
            dersAdi->clear();
            bolum->setCurrentIndex(0);
            kredi->setValue(3);
            sonuc->clear();
        });

    pencere.show();

    // === GOREV 4.1 - Form Genisletme (15 dk) ===
    // TODO: a) QComboBox ile "Yariyil" secimi ekleyin (Guz, Bahar, Yaz):
    //          auto* yariyil = new QComboBox;
    //          yariyil->addItems({"Guz", "Bahar", "Yaz"});
    //          formDuzen->addRow("Yariyil:", yariyil);
    //
    // TODO: b) QSpinBox ile "Kontenjan" alani ekleyin
    //          (aralik: 10-200, varsayilan: 40):
    //          auto* kontenjan = new QSpinBox;
    //          kontenjan->setRange(10, 200);
    //          kontenjan->setValue(40);
    //          formDuzen->addRow("Kontenjan:", kontenjan);
    //
    // TODO: c) QCheckBox ile "Zorunlu Ders" onay kutusu ekleyin:
    //          #include <QCheckBox>
    //          auto* zorunlu = new QCheckBox("Zorunlu Ders");
    //          formDuzen->addRow("", zorunlu);
    //
    // TODO: d) Kaydet butonunun slot'unu yeni alanlari da icerecek
    //          sekilde guncelleyin
    // ============================================

    // === GOREV 4.2 - Dogrulama ve Geri Bildirim (15 dk) ===
    // TODO: a) Kaydet tiklandiginda ders adinin bos olup olmadigini
    //          kontrol edin. Bossa kirmizi uyari gosterin:
    //          if (dersAdi->text().isEmpty()) {
    //              sonuc->setStyleSheet("color: red; font-weight: bold;");
    //              sonuc->setText("Hata: Ders adi bos olamaz!");
    //              return;
    //          }
    //
    // TODO: b) Gecerli kayit durumunda sonuc etiketini yesil renkte
    //          gosterin:
    //          sonuc->setStyleSheet("color: green; font-weight: bold;");
    //
    // TODO: c) dersAdi alaninin textChanged sinyalini kullanarak,
    //          her karakter girisinde ders adinin uzunlugunu bir
    //          etikette gosterin (orn: "12 / 50 karakter"):
    //          QObject::connect(dersAdi, &QLineEdit::textChanged,
    //              [uzunlukEtiket](const QString& metin) {
    //                  uzunlukEtiket->setText(
    //                      QString("%1 / 50 karakter").arg(metin.size()));
    //              });
    // ============================================

    return app.exec();
}

/**
 * Lab 05 - Calisma 3 Baslangic Kodu
 * Diyalog Pencereleri
 *
 * Bu dosya Lab 05 foyu ile birlikte kullanilir.
 * "GOREV" isaretli bolgeleri tamamlayin.
 *
 * Ogrenilecek kavramlar:
 * - QMessageBox ile hazir diyaloglar (information, question)
 * - QDialog ile ozel diyalog olusturma
 * - QFormLayout ile form duzeni
 * - QDialogButtonBox ile standart butonlar
 * - Modal diyalog (exec()) ve donus degeri
 * - QFileDialog ile dosya secme
 *
 * Lab: 05 - Qt Pencere ve Duzen Yonetimi
 * Calisma: 3 - Diyalog Pencereleri (30 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Calistirma: ./app
 */

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget pencere;
    pencere.setWindowTitle("OBYS - Diyalog Ornekleri");
    pencere.resize(400, 300);

    auto* duzen = new QVBoxLayout(&pencere);
    auto* bilgiLabel = new QLabel("Bir butona tiklayin");
    bilgiLabel->setAlignment(Qt::AlignCenter);
    duzen->addWidget(bilgiLabel);

    // --- QMessageBox Ornekleri ---
    auto* bilgiButon = new QPushButton("Bilgi Mesaji");
    duzen->addWidget(bilgiButon);
    QObject::connect(bilgiButon, &QPushButton::clicked,
        [&pencere]() {
            QMessageBox::information(&pencere, "Bilgi",
                "Kayit islemi basarili!");
        });

    auto* onayButon = new QPushButton("Onay Diyalogu");
    duzen->addWidget(onayButon);
    QObject::connect(onayButon, &QPushButton::clicked,
        [&pencere, bilgiLabel]() {
            auto cevap = QMessageBox::question(&pencere,
                "Onay", "Kaydi silmek istiyor musunuz?",
                QMessageBox::Yes | QMessageBox::No);
            if (cevap == QMessageBox::Yes)
                bilgiLabel->setText("Kayit silindi!");
            else
                bilgiLabel->setText("Silme iptal edildi.");
        });

    // --- Ozel QDialog ---
    auto* ozelButon = new QPushButton("Ogrenci Bilgi Gir");
    duzen->addWidget(ozelButon);

    QObject::connect(ozelButon, &QPushButton::clicked,
        [&pencere, bilgiLabel]() {
            QDialog diyalog(&pencere);
            diyalog.setWindowTitle("Ogrenci Bilgi Girisi");

            auto* formDuzen = new QFormLayout(&diyalog);
            auto* adAlani = new QLineEdit;
            auto* noAlani = new QLineEdit;
            noAlani->setMaxLength(9);
            formDuzen->addRow("Ad Soyad:", adAlani);
            formDuzen->addRow("Ogrenci No:", noAlani);

            auto* butonlar = new QDialogButtonBox(
                QDialogButtonBox::Ok
                | QDialogButtonBox::Cancel);
            formDuzen->addRow(butonlar);

            QObject::connect(butonlar,
                &QDialogButtonBox::accepted,
                &diyalog, &QDialog::accept);
            QObject::connect(butonlar,
                &QDialogButtonBox::rejected,
                &diyalog, &QDialog::reject);

            if (diyalog.exec() == QDialog::Accepted) {
                bilgiLabel->setText(
                    QString("Girilen: %1 (%2)")
                    .arg(adAlani->text())
                    .arg(noAlani->text()));
            }
        });

    // === GOREV 3.1 - QMessageBox Turleri (10 dk) ===
    // TODO: a) Bir "Uyari" butonu ekleyin. Tiklandiginda
    //          QMessageBox::warning() ile uyari gosterin:
    //          auto* uyariButon = new QPushButton("Uyari");
    //          duzen->addWidget(uyariButon);
    //          QObject::connect(uyariButon, &QPushButton::clicked,
    //              [&pencere, bilgiLabel]() {
    //                  QMessageBox::warning(&pencere, "Uyari",
    //                      "Bu islem geri alinamaz!");
    //                  bilgiLabel->setText("Uyari mesaji gosterildi");
    //              });
    //
    // TODO: b) Bir "Kritik Hata" butonu ekleyin. Tiklandiginda
    //          QMessageBox::critical() ile hata gosterin:
    //          auto* kritikButon = new QPushButton("Kritik Hata");
    //          duzen->addWidget(kritikButon);
    //          QObject::connect(kritikButon, &QPushButton::clicked,
    //              [&pencere, bilgiLabel]() {
    //                  QMessageBox::critical(&pencere, "Hata",
    //                      "Veritabani baglantisi koptu!");
    //                  bilgiLabel->setText("Kritik hata mesaji gosterildi");
    //              });
    //
    // TODO: c) Her mesaj kutusundan sonra bilgiLabel'i gosterilen
    //          mesaj turuyle guncelleyin (yukaridaki orneklerde zaten var)
    // ============================================

    // === GOREV 3.2 - Dogrulamali Diyalog ve QFileDialog (20 dk) ===
    // TODO: a) Ogrenci bilgi diyaloguna dogrulama ekleyin: OK tiklandiginda
    //          ad ve numara alanlari bossa QMessageBox::warning() ile
    //          uyari gosterin ve diyalogu kapatmayin.
    //          (Ipucu: accepted sinyalini accept() yerine lambda'ya baglayin)
    //
    // TODO: b) QDialogButtonBox::accepted sinyalini dogrudan accept() yerine
    //          bir lambda'ya baglayin:
    //          QObject::connect(butonlar,
    //              &QDialogButtonBox::accepted,
    //              [&diyalog, adAlani, noAlani]() {
    //                  if (adAlani->text().isEmpty()) {
    //                      QMessageBox::warning(&diyalog, "Hata",
    //                          "Ad alani bos olamaz!");
    //                      return;
    //                  }
    //                  if (noAlani->text().isEmpty()) {
    //                      QMessageBox::warning(&diyalog, "Hata",
    //                          "Ogrenci numarasi bos olamaz!");
    //                      return;
    //                  }
    //                  diyalog.accept();
    //              });
    //
    // TODO: c) Yeni bir "Dosya Sec" butonu ekleyin. Tiklandiginda
    //          QFileDialog::getOpenFileName() ile dosya sectirin:
    //          auto* dosyaButon = new QPushButton("Dosya Sec");
    //          duzen->addWidget(dosyaButon);
    //          QObject::connect(dosyaButon, &QPushButton::clicked,
    //              [&pencere, bilgiLabel]() {
    //                  QString dosya = QFileDialog::getOpenFileName(
    //                      &pencere, "Dosya Sec", "",
    //                      "CSV Dosyalari (*.csv);;Tum Dosyalar (*)");
    //                  if (!dosya.isEmpty())
    //                      bilgiLabel->setText("Secilen: " + dosya);
    //              });
    //
    // TODO: d) Secilen dosya yolunu bilgiLabel'da gosterin
    //          (yukaridaki ornekte zaten yapiliyor)
    // ============================================

    pencere.show();
    return app.exec();
}

/**
 * Lab 04 - Çalışma 3 Başlangıç Kodu
 * Sinyal/Slot Bağlantıları
 *
 * Bu dosya Lab 04 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - connect() ile sinyal/slot bağlantısı
 * - Lambda ile slot tanımlama
 * - Bir sinyali birden fazla slota bağlama
 * - disconnect() ile bağlantı kesme
 * - QMetaObject::Connection ile bağlantı yönetimi
 *
 * Lab: 04 - Qt Framework'üne Giriş
 * Çalışma: 3 - Sinyal/Slot Bağlantıları (30 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştırma: ./app
 */

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget pencere;
    pencere.setWindowTitle("Sinyal/Slot Ornegi");
    pencere.resize(400, 300);

    auto* etiket = new QLabel("Butona tiklayin", &pencere);
    etiket->setGeometry(120, 30, 200, 30);

    auto* sayacButon = new QPushButton("Tikla: 0", &pencere);
    sayacButon->setGeometry(120, 80, 160, 40);

    auto* kapatButon = new QPushButton("Kapat", &pencere);
    kapatButon->setGeometry(120, 140, 160, 40);

    // Lambda ile sinyal/slot baglantisi
    int sayac = 0;
    QObject::connect(sayacButon, &QPushButton::clicked,
        [&sayac, sayacButon, etiket]() {
            sayac++;
            sayacButon->setText(
                QString("Tikla: %1").arg(sayac));
            etiket->setText(
                QString("Toplam tiklama: %1").arg(sayac));
        });

    // Kapat butonuna uygulamayi kapatma baglantisi
    QObject::connect(kapatButon, &QPushButton::clicked,
                     &app, &QApplication::quit);

    pencere.show();

    // === GOREV 3.1 - Temel Sinyal/Slot Baglantilari (10 dk) ===
    // TODO: a) Bir "Sifirla" butonu ekleyin. Tiklandiginda sayaci
    //          0'a dondursun ve etiket ile buton metnini guncellesin:
    //          auto* sifirlaButon = new QPushButton("Sifirla", &pencere);
    //          sifirlaButon->setGeometry(120, 200, 160, 40);
    //          QObject::connect(sifirlaButon, &QPushButton::clicked,
    //              [&sayac, sayacButon, etiket]() {
    //                  sayac = 0;
    //                  sayacButon->setText("Tikla: 0");
    //                  etiket->setText("Butona tiklayin");
    //              });
    //
    // TODO: b) Sayac 10'a ulastiginda sayacButon'u devre disi birakin:
    //          sayacButon->setEnabled(false);
    //
    // TODO: c) Sifirla butonu tiklandiginda sayacButon'u tekrar
    //          etkinlestirin:
    //          sayacButon->setEnabled(true);
    // ============================================

    // === GOREV 3.2 - Coklu Sinyal Baglantilari (10 dk) ===
    // TODO: a) Yeni bir "Bilgi" butonu ekleyin
    //
    // TODO: b) Bu butonun clicked sinyalini 3 farkli lambda'ya baglayin:
    //          - Konsola qDebug() << "Bilgi butonuna tiklandi";
    //          - Etiket metnini "Bilgi butonu tiklandi" olarak degistir
    //          - Pencere basligini "OBYS -- Bilgi Goruntulendi" yap
    //
    // TODO: c) Butona tiklayarak 3 slot'un da calistigini dogrulayin
    // ============================================

    // === GOREV 3.3 - disconnect Kullanimi (10 dk) ===
    // TODO: a) connect() cagrisinin donus degerini saklayin:
    //          auto baglanti = QObject::connect(buton, &QPushButton::clicked,
    //              []() { qDebug() << "Bagli!"; });
    //
    // TODO: b) Bir "Baglantiyi Kes" butonu ekleyin. Tiklandiginda:
    //          QObject::disconnect(baglanti);
    //
    // TODO: c) Baglanti kesildikten sonra butonun tiklanmasinin artik
    //          slot'u tetiklemedigini dogrulayin
    // ============================================

    return app.exec();
}

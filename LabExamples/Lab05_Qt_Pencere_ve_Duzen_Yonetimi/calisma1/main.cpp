/**
 * Lab 05 - Calisma 1 Baslangic Kodu
 * Duzen Yoneticileri
 *
 * Bu dosya Lab 05 foyu ile birlikte kullanilir.
 * "GOREV" isaretli bolgeleri tamamlayin.
 *
 * Ogrenilecek kavramlar:
 * - QVBoxLayout ile dikey duzenleme
 * - QHBoxLayout ile yatay duzenleme
 * - QGroupBox ile gruplama
 * - QGridLayout ile izgara duzeni
 * - addStretch() ile esnek bosluk
 * - Ic ice duzen yoneticileri (nested layouts)
 *
 * Lab: 05 - Qt Pencere ve Duzen Yonetimi
 * Calisma: 1 - Duzen Yoneticileri (30 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Calistirma: ./app
 */

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget pencere;
    pencere.setWindowTitle("OBYS - Duzen Yonetimi");
    pencere.resize(500, 400);

    // Ana duzen: dikey
    auto* anaDuzen = new QVBoxLayout(&pencere);

    // Baslik
    auto* baslik = new QLabel("OBYS Ogrenci Arama");
    baslik->setStyleSheet(
        "font-size: 16px; font-weight: bold;"
        " color: #008080;");
    baslik->setAlignment(Qt::AlignCenter);
    anaDuzen->addWidget(baslik);

    // Arama cubugu: yatay duzen
    auto* aramaDuzen = new QHBoxLayout;
    auto* aramaAlani = new QLineEdit;
    aramaAlani->setPlaceholderText(
        "Ogrenci adi veya numarasi...");
    auto* araButon = new QPushButton("Ara");
    auto* temizleButon = new QPushButton("Temizle");
    aramaDuzen->addWidget(aramaAlani);
    aramaDuzen->addWidget(araButon);
    aramaDuzen->addWidget(temizleButon);
    anaDuzen->addLayout(aramaDuzen);

    // Sonuc grubu: QGroupBox icinde dikey duzen
    auto* sonucGrubu = new QGroupBox("Arama Sonuclari");
    auto* sonucDuzen = new QVBoxLayout;
    sonucDuzen->addWidget(new QLabel("Sonuc 1: Ali Yilmaz"));
    sonucDuzen->addWidget(new QLabel("Sonuc 2: Ayse Kaya"));
    sonucDuzen->addWidget(new QLabel("Sonuc 3: Mehmet Demir"));
    sonucGrubu->setLayout(sonucDuzen);
    anaDuzen->addWidget(sonucGrubu);

    // Alt bilgi: yatay duzen
    auto* altDuzen = new QHBoxLayout;
    altDuzen->addWidget(new QLabel("Toplam: 3 sonuc"));
    altDuzen->addStretch();
    altDuzen->addWidget(new QPushButton("Disa Aktar"));
    anaDuzen->addLayout(altDuzen);

    // === GOREV 1.1 - Ic Ice Duzen Olusturma (10 dk) ===
    // TODO: a) Arama sonuclari grubunun altina yeni bir QGroupBox ekleyin:
    //          "Filtreler"
    //          auto* filtreGrubu = new QGroupBox("Filtreler");
    //
    // TODO: b) Bu grubun icinde QHBoxLayout kullanarak 3 adet QPushButton
    //          yerlestirin: "Bolum", "Sinif", "Yariyil"
    //          auto* filtreDuzen = new QHBoxLayout;
    //          filtreDuzen->addWidget(new QPushButton("Bolum"));
    //          filtreDuzen->addWidget(new QPushButton("Sinif"));
    //          filtreDuzen->addWidget(new QPushButton("Yariyil"));
    //
    // TODO: c) Filtre butonlarinin arasina addStretch() ile esit bosluk ekleyin
    //          filtreDuzen->addStretch();  // butonlar arasina ekleyin
    //          filtreGrubu->setLayout(filtreDuzen);
    //          anaDuzen->addWidget(filtreGrubu);
    // ============================================

    // === GOREV 1.2 - QGridLayout ile Hesap Makinesi Duzeni (20 dk) ===
    // TODO: a) Yeni bir pencere olusturun: "OBYS -- Not Hesaplayici"
    //          QWidget hesapPencere;
    //          hesapPencere.setWindowTitle("OBYS -- Not Hesaplayici");
    //          hesapPencere.resize(300, 400);
    //
    // TODO: b) Ust kisma bir QLineEdit ekleyin (ekran alani)
    //          auto* hesapDuzen = new QVBoxLayout(&hesapPencere);
    //          auto* ekran = new QLineEdit;
    //          ekran->setReadOnly(true);
    //          ekran->setAlignment(Qt::AlignRight);
    //          hesapDuzen->addWidget(ekran);
    //
    // TODO: c) Altina QGridLayout ile 4x4 buton izgarasi olusturun:
    //          auto* grid = new QGridLayout;
    //          // addWidget(widget, satir, sutun)
    //          grid->addWidget(new QPushButton("7"), 0, 0);
    //          grid->addWidget(new QPushButton("8"), 0, 1);
    //          grid->addWidget(new QPushButton("9"), 0, 2);
    //          grid->addWidget(new QPushButton("/"), 0, 3);
    //
    // TODO: d) Kalan satirlari da doldurun: 4-5-6-*, 1-2-3--, 0-.-=+
    //          grid->addWidget(new QPushButton("4"), 1, 0);
    //          grid->addWidget(new QPushButton("5"), 1, 1);
    //          // ... devam edin
    //
    // TODO: e) Butonlarin minimum boyutunu ayarlayin:
    //          buton->setMinimumSize(60, 40);
    //          hesapDuzen->addLayout(grid);
    //          hesapPencere.show();
    // ============================================

    pencere.show();
    return app.exec();
}

/**
 * Temel Düzen Yöneticileri - QHBoxLayout, QVBoxLayout ve İç İçe Düzenler
 *
 * Bu program, Qt'nin temel düzen yöneticilerini gösterir. Yatay (HBox),
 * dikey (VBox) ve iç içe düzenlerin nasıl kullanıldığını, widget'ların
 * düzen içinde nasıl hizalandığını ve boşlukların nasıl ayarlandığını
 * örnekler.
 *
 * Öğrenilecek kavramlar:
 * - QHBoxLayout ile yatay düzen
 * - QVBoxLayout ile dikey düzen
 * - İç içe (nested) düzenler
 * - addWidget(), addSpacing(), addStretch()
 * - Düzen yöneticilerinin parent-child ilişkisi
 *
 * Bölüm: 05 - Qt Pencere ve Düzen Yönetimi
 * Ünite: 1 - Düzen Yöneticileri
 *
 * Derleme (CMake önerilir, CMakeLists.txt dosyasına bakınız):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 *
 * Doğrudan derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 01_temel_duzenler 01_temel_duzenler.cpp
 */

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) ANA PENCERE
    // =============================================
    QWidget pencere;
    pencere.setWindowTitle("Temel Düzen Yöneticileri");
    pencere.setMinimumSize(500, 400);

    // Ana düzen: dikey (QVBoxLayout)
    QVBoxLayout* anaDuzen = new QVBoxLayout(&pencere);
    anaDuzen->setSpacing(15);
    anaDuzen->setContentsMargins(20, 20, 20, 20);

    // =============================================
    // 2) ÜST BÖLÜM - Yatay Düzen (QHBoxLayout)
    // =============================================
    QLabel* ustBaslik = new QLabel("Yatay Düzen (QHBoxLayout)");
    ustBaslik->setStyleSheet("font-weight: bold; color: #2c3e50;");
    anaDuzen->addWidget(ustBaslik);

    QHBoxLayout* yatayDuzen = new QHBoxLayout();

    QPushButton* btnKirmizi = new QPushButton("Kırmızı");
    btnKirmizi->setStyleSheet("background-color: #e74c3c; color: white; padding: 10px;");

    QPushButton* btnYesil = new QPushButton("Yeşil");
    btnYesil->setStyleSheet("background-color: #27ae60; color: white; padding: 10px;");

    QPushButton* btnMavi = new QPushButton("Mavi");
    btnMavi->setStyleSheet("background-color: #2980b9; color: white; padding: 10px;");

    yatayDuzen->addWidget(btnKirmizi);
    yatayDuzen->addSpacing(20);  // Butonlar arası 20 piksel boşluk
    yatayDuzen->addWidget(btnYesil);
    yatayDuzen->addWidget(btnMavi);

    anaDuzen->addLayout(yatayDuzen);

    // =============================================
    // 3) ORTA BÖLÜM - Dikey Düzen (QVBoxLayout)
    // =============================================
    QLabel* ortaBaslik = new QLabel("Dikey Düzen (QVBoxLayout)");
    ortaBaslik->setStyleSheet("font-weight: bold; color: #2c3e50;");
    anaDuzen->addWidget(ortaBaslik);

    QVBoxLayout* dikeyDuzen = new QVBoxLayout();

    QLabel* etiket1 = new QLabel("Birinci satır etiketi");
    etiket1->setStyleSheet("background-color: #eaf2f8; padding: 8px; border-radius: 4px;");

    QLabel* etiket2 = new QLabel("İkinci satır etiketi");
    etiket2->setStyleSheet("background-color: #d5f5e3; padding: 8px; border-radius: 4px;");

    QLabel* etiket3 = new QLabel("Üçüncü satır etiketi");
    etiket3->setStyleSheet("background-color: #fdebd0; padding: 8px; border-radius: 4px;");

    dikeyDuzen->addWidget(etiket1);
    dikeyDuzen->addWidget(etiket2);
    dikeyDuzen->addWidget(etiket3);

    anaDuzen->addLayout(dikeyDuzen);

    // =============================================
    // 4) ALT BÖLÜM - İç İçe Düzen
    // =============================================
    QLabel* altBaslik = new QLabel("İç İçe Düzen (VBox içinde HBox)");
    altBaslik->setStyleSheet("font-weight: bold; color: #2c3e50;");
    anaDuzen->addWidget(altBaslik);

    // Dikey düzen içinde yatay düzen
    QVBoxLayout* altDikeyDuzen = new QVBoxLayout();

    QHBoxLayout* icYatayDuzen = new QHBoxLayout();
    QPushButton* btnTamam = new QPushButton("Tamam");
    btnTamam->setStyleSheet("background-color: #27ae60; color: white; padding: 8px 20px;");
    QPushButton* btnIptal = new QPushButton("İptal");
    btnIptal->setStyleSheet("background-color: #e74c3c; color: white; padding: 8px 20px;");

    icYatayDuzen->addStretch();  // Butonları sağa yasla
    icYatayDuzen->addWidget(btnTamam);
    icYatayDuzen->addWidget(btnIptal);

    QLabel* altAciklama = new QLabel("addStretch() ile butonlar sağa yaslandı");
    altAciklama->setStyleSheet("color: #7f8c8d; font-style: italic;");

    altDikeyDuzen->addWidget(altAciklama);
    altDikeyDuzen->addLayout(icYatayDuzen);

    anaDuzen->addLayout(altDikeyDuzen);

    // En alta esneklik ekle
    anaDuzen->addStretch();

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "Temel Düzen Yöneticileri"
 * Boyut: 500x400 piksel (minimum)
 *
 * İçerik:
 *   - "Yatay Düzen (QHBoxLayout)" başlığı
 *     [Kırmızı] ----20px---- [Yeşil] [Mavi]  (yatay sıralı butonlar)
 *
 *   - "Dikey Düzen (QVBoxLayout)" başlığı
 *     [Birinci satır etiketi]   (mavi arka plan)
 *     [İkinci satır etiketi]    (yeşil arka plan)
 *     [Üçüncü satır etiketi]   (turuncu arka plan)
 *
 *   - "İç İçe Düzen (VBox içinde HBox)" başlığı
 *     "addStretch() ile butonlar sağa yaslandı"
 *                               [Tamam] [İptal]  (sağa yaslanmış)
 *
 * Davranış:
 *   - Pencere boyutu değiştiğinde widget'lar düzen kurallarına göre
 *     otomatik olarak yeniden boyutlandırılır
 *   - addStretch() sayesinde alt butonlar her zaman sağda kalır
 *   - addSpacing(20) ile Kırmızı ve Yeşil butonlar arasında sabit boşluk
 */

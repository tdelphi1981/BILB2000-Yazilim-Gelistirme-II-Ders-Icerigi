/**
 * CMake Proje Yapısı - Qt Uygulaması
 *
 * Bu program, CMake ile derlenen daha kapsamlı bir Qt uygulamasını
 * gösterir. Birden fazla QLabel, farklı stiller ve düzen (layout)
 * kullanımı örneklenmiştir.
 *
 * Öğrenilecek kavramlar:
 * - CMake ile Qt projesi yapılandırma
 * - QVBoxLayout ile dikey yerleştirme
 * - Birden fazla widget'i düzenleme
 * - Qt stil sayfası (stylesheet) kullanımı
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 1 - Qt'ye Genel Bakış
 *
 * Derleme (CMake):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 *   ./cmake_proje_yapisi
 */

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // Ana pencere
    QWidget pencere;
    pencere.setWindowTitle("OBYS - CMake Proje Örneği");
    pencere.setMinimumSize(400, 300);

    // Dikey yerleştirme düzenleyicisi (layout)
    // Layout, widget'ları otomatik olarak hizalar ve boyutlandırır.
    QVBoxLayout* duzen = new QVBoxLayout(&pencere);
    duzen->setSpacing(10);
    duzen->setContentsMargins(20, 20, 20, 20);

    // Başlık etiketi
    QLabel* baslik = new QLabel("Öğrenci Bilgi Yönetim Sistemi");
    baslik->setAlignment(Qt::AlignCenter);
    baslik->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #2c3e50; "
        "padding: 10px; background-color: #ecf0f1; border-radius: 5px;"
    );

    // Bilgi etiketleri
    QLabel* kurum = new QLabel("Fen Fakültesi - Bilgisayar Bilimleri Bölümü");
    kurum->setAlignment(Qt::AlignCenter);
    kurum->setStyleSheet("font-size: 14px; color: #34495e;");

    QLabel* ders = new QLabel("BILB2000 - Yazılım Geliştirme II");
    ders->setAlignment(Qt::AlignCenter);
    ders->setStyleSheet("font-size: 13px; color: #7f8c8d;");

    QLabel* donem = new QLabel("2024-2025 Bahar Dönemi");
    donem->setAlignment(Qt::AlignCenter);
    donem->setStyleSheet("font-size: 12px; color: #95a5a6;");

    // Açıklama etiketi
    QLabel* aciklama = new QLabel(
        "Bu proje CMake ile yapılandırılmış bir Qt6 uygulamasıdır.\n"
        "find_package(Qt6) ile Qt modülleri otomatik bulunur."
    );
    aciklama->setAlignment(Qt::AlignCenter);
    aciklama->setWordWrap(true);
    aciklama->setStyleSheet(
        "font-size: 11px; color: #7f8c8d; padding: 10px; "
        "border: 1px solid #bdc3c7; border-radius: 3px;"
    );

    // Kapat butonu
    QPushButton* kapatButon = new QPushButton("Uygulamayı Kapat");
    kapatButon->setStyleSheet(
        "background-color: #3498db; color: white; "
        "font-size: 14px; padding: 8px; border-radius: 5px;"
    );

    // Widget'ları düzene ekle
    duzen->addWidget(baslik);
    duzen->addWidget(kurum);
    duzen->addWidget(ders);
    duzen->addWidget(donem);
    duzen->addStretch();  // Esnek boşluk ekle
    duzen->addWidget(aciklama);
    duzen->addWidget(kapatButon);

    // Buton tıklandığında uygulamayı kapat
    QObject::connect(kapatButon, &QPushButton::clicked,
                     &uygulama, &QApplication::quit);

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "OBYS - CMake Proje Örneği"
 * Minimum boyut: 400x300 piksel
 *
 * İçerik (dikey sırada):
 *   - "Öğrenci Bilgi Yönetim Sistemi" başlığı (gri arka plan)
 *   - "Fen Fakültesi - Bilgisayar Bilimleri Bölümü"
 *   - "BILB2000 - Yazılım Geliştirme II"
 *   - "2024-2025 Bahar Dönemi"
 *   - (esnek boşluk)
 *   - CMake açıklama metni (çerçeveli)
 *   - Mavi "Uygulamayı Kapat" butonu
 *
 * Buton tıklandığında uygulama sonlanır.
 */

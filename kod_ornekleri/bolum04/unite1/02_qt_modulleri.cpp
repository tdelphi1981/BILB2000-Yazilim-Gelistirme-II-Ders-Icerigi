/**
 * Qt Modülleri - Birden Fazla Widget Kullanımı
 *
 * Bu program, bir Qt penceresinde birden fazla widget'in (QPushButton,
 * QLabel) nasıl bir arada kullanıldığını gösterir. Qt Widgets modülünün
 * temel bileşenleri tanıtılır.
 *
 * Öğrenilecek kavramlar:
 * - QWidget ile ana pencere oluşturma
 * - QPushButton ve QLabel widget'ları
 * - setWindowTitle() ve setGeometry() kullanımı
 * - Widget'ların parent-child ilişkisi
 * - Temel sinyal/slot bağlantısı (clicked -> close)
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 1 - Qt'ye Genel Bakış
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 02_qt_modulleri 02_qt_modulleri.cpp
 */

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // Ana pencere olarak QWidget kullanalım.
    // QWidget, tüm görünür widget'ların temel sınıfıdır.
    QWidget pencere;
    pencere.setWindowTitle("Qt Modülleri Örneği");
    pencere.setGeometry(100, 100, 400, 200);
    // setGeometry(x, y, genişlik, yükseklik):
    //   x, y       -> Ekrandaki konum (sol üst köşe)
    //   genişlik    -> Pencere genişliği (piksel)
    //   yükseklik   -> Pencere yüksekliği (piksel)

    // QLabel: Metin veya görsel gösteren widget.
    // &pencere parametresi bu etiketin pencereye ait olduğunu belirtir.
    QLabel* baslikEtiket = new QLabel("OBYS - Öğrenci Bilgi Yönetim Sistemi", &pencere);
    baslikEtiket->setGeometry(50, 20, 300, 30);
    baslikEtiket->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50;");

    QLabel* bilgiEtiket = new QLabel("Qt Widgets modülü ile oluşturuldu.", &pencere);
    bilgiEtiket->setGeometry(50, 60, 300, 25);
    bilgiEtiket->setStyleSheet("font-size: 12px; color: #7f8c8d;");

    // QPushButton: Tıklanabilir buton widget'i.
    QPushButton* kapat_buton = new QPushButton("Kapat", &pencere);
    kapat_buton->setGeometry(150, 120, 100, 35);
    kapat_buton->setStyleSheet(
        "background-color: #e74c3c; color: white; "
        "border-radius: 5px; font-size: 14px;"
    );

    // Yeni connect syntax'i ile sinyal/slot bağlantısı:
    // Buton tıklandığında pencere kapansın
    QObject::connect(kapat_buton, &QPushButton::clicked,
                     &pencere, &QWidget::close);

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "Qt Modülleri Örneği"
 * Konum: Ekranın (100, 100) noktasından başlayarak
 * Boyut: 400x200 piksel
 *
 * İçerik:
 *   - Üst kısımda "OBYS - Öğrenci Bilgi Yönetim Sistemi" başlığı (koyu mavi)
 *   - Altında "Qt Widgets modülü ile oluşturuldu." bilgi metni (gri)
 *   - Ortada kırmızı "Kapat" butonu
 *
 * "Kapat" butonuna tıklandığında pencere kapanır ve uygulama sonlanır.
 *
 * NOT: Bu bir GUI uygulamasıdır, konsol çıktısı üretmez.
 */

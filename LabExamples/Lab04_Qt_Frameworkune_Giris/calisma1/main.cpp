/**
 * Lab 04 - Çalışma 1 Başlangıç Kodu
 * İlk Qt Penceresi
 *
 * Bu dosya Lab 04 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - QApplication ve olay döngüsü (event loop)
 * - QWidget ile pencere oluşturma
 * - QLabel ile metin gösterme
 * - Pencere boyutu ve başlık ayarları
 * - setGeometry() ile widget konumlandırma
 *
 * Lab: 04 - Qt Framework'üne Giriş
 * Çalışma: 1 - İlk Qt Penceresi (25 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştırma: ./app
 */

#include <QApplication>
#include <QWidget>
#include <QLabel>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Ana pencere olustur
    QWidget pencere;
    pencere.setWindowTitle("OBYS - Ilk Pencere");
    pencere.resize(400, 300);

    // Pencere icine bir etiket ekle
    QLabel etiket("Merhaba Qt!", &pencere);
    etiket.setGeometry(150, 130, 200, 30);

    pencere.show();

    // === GOREV 1.1 - Pencere Ozellestirme (10 dk) ===
    // TODO: a) Pencere boyutunu 600x400 piksel olarak ayarlayin
    //          pencere.resize(600, 400);
    //
    // TODO: b) Pencere basligini degistirin:
    //          pencere.setWindowTitle("OBYS -- Ogrenci Bilgi Yonetim Sistemi");
    //
    // TODO: c) Minimum boyut belirleyin:
    //          pencere.setMinimumSize(400, 300);
    //
    // TODO: d) Maksimum boyut belirleyin:
    //          pencere.setMaximumSize(800, 600);
    // ============================================

    // === GOREV 1.2 - Birden Fazla Etiket (15 dk) ===
    // TODO: a) Pencerenin ust kismina "OBYS Giris Ekrani" baslik etiketi
    //          ekleyin (setGeometry ile konumlandir):
    //          QLabel baslikEtiket("OBYS Giris Ekrani", &pencere);
    //          baslikEtiket.setGeometry(150, 30, 300, 40);
    //
    // TODO: b) Baslik etiketinin yazi tipini buyutun:
    //          baslikEtiket.setStyleSheet("font-size: 18px; font-weight: bold;");
    //
    // TODO: c) Altina "Ogrenci No:" ve "Sifre:" etiketleri ekleyin
    //
    // TODO: d) Tum etiketlerin parent'ini &pencere olarak ayarlayin
    // ============================================

    return app.exec();
}

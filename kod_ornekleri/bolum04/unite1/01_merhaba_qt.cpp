/**
 * Merhaba Qt! - İlk Qt Uygulaması
 *
 * Bu program, en temel Qt uygulamasını oluşturur: bir pencere içinde
 * "Merhaba Qt!" yazısı gösteren bir etiket (QLabel). Qt uygulamalarının
 * temel yapısını (QApplication + widget) tanıtmak amaçlanmıştır.
 *
 * Öğrenilecek kavramlar:
 * - QApplication nesnesi ve olay döngüsü (event loop)
 * - QLabel widget'i ile metin gösterimi
 * - Pencere başlığını ve boyutunu ayarlama
 * - show() ile widget'i ekranda gösterme
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 1 - Qt'ye Genel Bakış
 *
 * Derleme (CMake):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 *
 * Doğrudan derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 01_merhaba_qt 01_merhaba_qt.cpp
 */

#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[]) {
    // Her Qt GUI uygulaması bir QApplication nesnesi ile başlar.
    // Bu nesne olay döngüsünü (event loop) yönetir.
    QApplication uygulama(argc, argv);

    // QLabel, metin veya görsel gösteren temel bir widget'tir.
    // Burada basit bir "Merhaba Qt!" mesajı oluşturuyoruz.
    QLabel etiket("Merhaba Qt!");

    // Pencere başlığını ayarlayalım
    etiket.setWindowTitle("İlk Qt Uygulamam");

    // Pencere boyutunu 300x100 piksel olarak ayarlayalım
    etiket.resize(300, 100);

    // Yazı hizasını ortala
    etiket.setAlignment(Qt::AlignCenter);

    // Yazı boyutunu büyütelim (stil sayfası ile)
    etiket.setStyleSheet("font-size: 24px; font-weight: bold;");

    // Widget'i ekranda göster
    etiket.show();

    // Olay döngüsünü başlat ve uygulama kapanana kadar bekle.
    // exec() kullanıcı pencereyi kapatana kadar geri dönmez.
    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "İlk Qt Uygulamam"
 * Boyut: 300x100 piksel
 * İçerik: Ortada büyük harflerle "Merhaba Qt!" yazısı
 *
 * Pencere kapatıldığında uygulama sonlanır ve 0 döner.
 *
 * NOT: Bu bir GUI uygulamasıdır, konsol çıktısı üretmez.
 * Derleme için Qt6 kütüphanelerinin yüklü olması gerekir.
 */

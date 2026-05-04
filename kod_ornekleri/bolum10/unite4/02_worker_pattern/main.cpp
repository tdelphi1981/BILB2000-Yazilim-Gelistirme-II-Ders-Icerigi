/**
 * main.cpp - Worker Pattern örneği giriş noktası
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 4 - Qt ile Çok Kanallı Programlama
 *
 * Derleme:
 *   mkdir build && cd build
 *   cmake ..
 *   make
 *   ./worker_pattern
 */
#include <QApplication>
#include "ana_pencere.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    AnaPencere pencere;
    pencere.show();
    return app.exec();
}

/*
 * BEKLENEN DAVRANIŞ:
 * ------------------
 * 1. Pencere açılır, "Hazır" yazar.
 * 2. "Hesaplamayı Başlat" butonu tıklayın.
 * 3. Progress bar yavaşça dolar, durum metni günceller.
 * 4. UI donmaz -- pencereyi hareket ettirebilirsiniz.
 * 5. "İptal" basarsanız yarıda duruyor.
 * 6. Tamamlandığında "Tamamlandı!" mesajı görünür.
 */

/**
 * OBYS Protokol Sunucusu — main
 *
 * Bu uygulamada:
 *  - QTcpServer + length-prefix + JSON dispatch zinciri
 *  - Komut tablosu ile sözlük tabanlı komut yönlendirme
 *  - Oturum durumu (login flag) soket başına saklanır
 *  - "ping", "login", "ogrenci_listele", "ogrenci_ekle" komutları
 *
 * OBYS bağlamı: Bu sunucu, Bölüm 13'te genişletilecek olan gerçek
 * OBYS sunucusunun çekirdek protokol katmanını gösterir. Komut
 * tablosu mimarisi sayesinde yeni komutlar tek satır eklenerek
 * tanımlanabilir.
 *
 * Bölüm 12 Ünite 4 Konu 3
 *
 * Derleme:  mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştır: ./obys_sunucu 9200
 */

#include "Sunucu.h"

#include <QCoreApplication>

#include <iostream>

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    quint16 port = 9200;
    QStringList argumanlar = uygulama.arguments();
    if (argumanlar.size() >= 2) {
        port = static_cast<quint16>(argumanlar[1].toUInt());
    }

    Sunucu sunucu;
    if (!sunucu.dinlemeBaslat(port)) {
        return 1;
    }

    std::cout << "Test için: ./obys_istemci localhost " << port << "\n";
    std::cout << "Çıkmak için Ctrl-C.\n";
    return uygulama.exec();
}

/*
BEKLENEN CIKTI (sunucu tarafı):
-------------------------------
OBYS sunucu 9200 portunda dinliyor.
Test için: ./obys_istemci localhost 9200
Çıkmak için Ctrl-C.
Bağlandı: "127.0.0.1"
Ayrıldı: "127.0.0.1"
*/

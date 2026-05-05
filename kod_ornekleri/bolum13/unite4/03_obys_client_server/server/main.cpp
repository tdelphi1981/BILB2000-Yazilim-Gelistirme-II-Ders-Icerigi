/**
 * OBYS Sunucu - Bölüm 13 Mini Projesi
 *
 * Bu dosya QCoreApplication çalıştırarak ObysSunucu sınıfını başlatır.
 * Sunucu, varsayılan olarak 5560 portunda dinler ve istemcilerin
 * gönderdiği JSON komutları cevaplar.
 *
 * Demo kullanıcı:
 *   kullanici = ogr001
 *   parola    = sifre123  (parola_hash = SHA-256(sifre123))
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 4 - Güvenlik ve Tasarım
 *
 * Derleme:
 *   mkdir build && cd build
 *   cmake .. && cmake --build .
 * Çalıştırma: ./server/obys_sunucu [port]
 */

#include <QCoreApplication>
#include <QTextStream>

#include "obys_sunucu.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    quint16 port = 5560;
    if (argc > 1) {
        port = static_cast<quint16>(QString(argv[1]).toUShort());
    }

    ObysSunucu sunucu;
    if (!sunucu.baslat(port)) {
        return 1;
    }

    QTextStream(stdout)
        << "OBYS Sunucu calisiyor. Cikmak icin Ctrl+C.\n";
    return app.exec();
}

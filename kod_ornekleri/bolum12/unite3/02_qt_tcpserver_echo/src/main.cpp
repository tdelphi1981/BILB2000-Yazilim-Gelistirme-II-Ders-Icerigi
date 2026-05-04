/**
 * Qt TCP Echo Sunucu — QTcpServer ile sinyal/slot tabanlı sunucu
 *
 * Bu uygulamada:
 *  - QCoreApplication ile event loop başlatıyoruz
 *  - QTcpServer ile bağlantı kabul ediyoruz
 *  - Her istemcinin readyRead sinyalini echo slotuna bağlıyoruz
 *  - Bağlantı kopunca deleteLater() ile temiz silme
 *
 * OBYS bağlamı: Bu sunucu, Qt sinyal/slot mekanizmasının ağ üzerinde
 * nasıl kullanıldığını gösterir. POSIX accept döngüsünün yerini Qt
 * event loop alır; her istemci için ayrı bir thread açmaya gerek
 * kalmaz çünkü tüm I/O olayları event loop içinde işlenir.
 *
 * Bölüm 12 Ünite 3 Konu 2
 *
 * Derleme:  mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştır: ./qt_tcpserver_echo 9000
 *           (başka terminal) nc localhost 9000
 */

#include "EchoSunucu.h"

#include <QCoreApplication>

#include <iostream>

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    quint16 port = 9000;
    QStringList argumanlar = uygulama.arguments();
    if (argumanlar.size() >= 2) {
        port = static_cast<quint16>(argumanlar[1].toUInt());
    }

    EchoSunucu sunucu;
    if (!sunucu.dinlemeBaslat(port)) {
        return 1;
    }

    std::cout << "Çıkmak için Ctrl-C.\n";
    return uygulama.exec();
}

/*
BEKLENEN CIKTI:
---------------
$ ./qt_tcpserver_echo 9000
Echo sunucu 9000 portunda dinliyor.
Çıkmak için Ctrl-C.
Bağlandı: "127.0.0.1" : 54123
Echo: 8 bayt yansıtılıyor.
Ayrıldı: "127.0.0.1"
*/

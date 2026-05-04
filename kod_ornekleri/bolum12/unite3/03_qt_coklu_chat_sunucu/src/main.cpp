/**
 * Qt Çoklu Chat Sunucu — Broadcast mantığı ile multi-istemci
 *
 * Bu uygulamada:
 *  - QTcpServer + QList<QTcpSocket*> ile bağlı istemcileri yönet
 *  - Bir istemciden gelen mesajı diğer tüm istemcilere ilet
 *  - Bağlantı kopunca listeden çıkar, deleteLater ile sil
 *  - Otomatik takma ad atama (kullanici1, kullanici2, ...)
 *
 * OBYS bağlamı: Bu mimari, OBYS sunucusunda da kullanılabilecek
 * temel multi-istemci pattern'idir. Bir öğretmen kayıt güncellediğinde,
 * diğer kullanıcıların ekranındaki listenin de güncellenmesi (push
 * notification) bu yöntemle yapılır.
 *
 * Bölüm 12 Ünite 3 Konu 3
 *
 * Derleme:  mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştır: ./qt_coklu_chat_sunucu 9100
 *           (birden fazla terminalde) nc localhost 9100
 */

#include "ChatSunucu.h"

#include <QCoreApplication>

#include <iostream>

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    quint16 port = 9100;
    QStringList argumanlar = uygulama.arguments();
    if (argumanlar.size() >= 2) {
        port = static_cast<quint16>(argumanlar[1].toUInt());
    }

    ChatSunucu sunucu;
    if (!sunucu.dinlemeBaslat(port)) {
        return 1;
    }

    std::cout << "Birden fazla terminalde 'nc localhost " << port
              << "' ile bağlanıp chat'i test edebilirsin.\n";
    std::cout << "Çıkmak için Ctrl-C.\n";

    return uygulama.exec();
}

/*
BEKLENEN CIKTI:
---------------
Sunucu tarafı:
$ ./qt_coklu_chat_sunucu 9100
Chat sunucu 9100 portunda dinliyor.
Bağlandı: "kullanici1" ( "127.0.0.1" )
Bağlandı: "kullanici2" ( "127.0.0.1" )
"kullanici1" : "selam"
"kullanici2" : "merhaba"
Ayrıldı: "kullanici1"

İstemci 2 tarafı (nc localhost 9100):
[sunucu] Hoş geldin kullanici2
[sunucu] kullanici1 odaya katıldı.   (yanlış sıra mümkün)
<kullanici1> selam
merhaba
[sunucu] kullanici1 odadan ayrıldı.
*/

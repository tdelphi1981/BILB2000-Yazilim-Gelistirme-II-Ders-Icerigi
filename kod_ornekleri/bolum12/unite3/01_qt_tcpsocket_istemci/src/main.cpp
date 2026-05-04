/**
 * Qt TCP İstemci Konsol Uygulaması
 *
 * Bu uygulamada:
 *  - QCoreApplication ile bir Qt event loop'u başlatıyoruz (GUI yok)
 *  - Istemci sınıfının sinyallerine lambda'larla bağlanıyoruz
 *  - QSocketNotifier ile stdin'i dinleyerek kullanıcının yazdığı
 *    satırları sunucuya gönderiyoruz
 *  - Bağlantı kopunca uygulama temiz çıkış yapar
 *
 * OBYS bağlamı: Bu uygulama "01_tcp_echo_sunucu" ya da
 * "02_qt_tcpserver_echo" projesi ile birlikte çalışacak şekilde
 * yazılmıştır. Stdin'den yazdığın satırları sunucuya yollar,
 * sunucudan gelen yanıtları "<<" prefix'iyle ekrana basar.
 *
 * Bölüm 12 Ünite 3 Konu 1
 *
 * Derleme:  mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştır: ./qt_tcpsocket_istemci localhost 9000
 */

#include "Istemci.h"

#include <QCoreApplication>
#include <QSocketNotifier>
#include <QTextStream>
#include <QTimer>

#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    // Argüman kontrolü
    QStringList argumanlar = uygulama.arguments();
    if (argumanlar.size() < 3) {
        std::cerr << "Kullanım: " << argv[0] << " <hostname> <port>\n";
        return 1;
    }

    const QString sunucu = argumanlar[1];
    const quint16 port   = static_cast<quint16>(argumanlar[2].toUInt());

    Istemci istemci;

    // Sinyal -> davranış bağlantıları
    QObject::connect(&istemci, &Istemci::baglandi, [] {
        std::cout << "[OK] Sunucuya bağlandı. Satır yaz, Enter ile gönder. Ctrl-D ile çık.\n";
    });

    QObject::connect(&istemci, &Istemci::baglantiKesildi, [&uygulama] {
        std::cout << "[INFO] Sunucu bağlantısı kesildi.\n";
        uygulama.quit();
    });

    QObject::connect(&istemci, &Istemci::hataOlustu, [&uygulama](const QString& mesaj) {
        std::cerr << "[HATA] " << mesaj.toStdString() << "\n";
        uygulama.quit();
    });

    QObject::connect(&istemci, &Istemci::satirAlindi, [](const QString& satir) {
        std::cout << "<< " << satir.toStdString() << "\n";
    });

    // Stdin'i ham fd 0 üzerinden dinle. QSocketNotifier dosya tanımlayıcı
    // okumaya hazır olunca tetiklenir; bu sayede event loop bloklamadan
    // kullanıcı girişini de izleyebiliriz.
    QSocketNotifier stdinIzleyici(STDIN_FILENO, QSocketNotifier::Read);
    QObject::connect(&stdinIzleyici, &QSocketNotifier::activated, [&] {
        std::string satir;
        if (!std::getline(std::cin, satir)) {
            // EOF (Ctrl-D)
            std::cout << "[INFO] EOF, kapanılıyor.\n";
            istemci.kapat();
            QTimer::singleShot(100, &uygulama, &QCoreApplication::quit);
            return;
        }
        istemci.satirGonder(QString::fromStdString(satir));
    });

    // Sunucuya bağlan
    istemci.baglan(sunucu, port);

    return uygulama.exec();
}

/*
BEKLENEN CIKTI:
---------------
$ ./qt_tcpsocket_istemci localhost 9000
[OK] Sunucuya bağlandı. Satır yaz, Enter ile gönder. Ctrl-D ile çık.
merhaba
<< merhaba
test
<< test
^D
[INFO] EOF, kapanılıyor.
[INFO] Sunucu bağlantısı kesildi.
*/

/**
 * Lab 12 - Çalışma 2: Qt TCP İstemci - main
 *
 * Konsol-tabanli istemci: stdin'den satir okuyup Istemci'ye gonderir,
 * gelen mesajlari stdout'a yazdirir.
 *
 * QSocketNotifier veya QFile ile stdin'i Qt event loop'una baglamak
 * platformlar arasi farklilik gosterir; basitlik adina ayri thread
 * (std::thread) ile stdin oku ve QMetaObject::invokeMethod ile
 * istemci slot'unu cagir.
 *
 * Test:
 *   ./echo_sunucu &              (Calisma 1 sunucu)
 *   ./qt_istemci 127.0.0.1 9000  (bu program)
 */

#include "Istemci.h"

#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QMetaObject>
#include <QByteArray>
#include <QString>

#include <iostream>
#include <string>
#include <thread>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QString host = (argc > 1) ? QString::fromLocal8Bit(argv[1]) : "127.0.0.1";
    quint16 port = (argc > 2) ? static_cast<quint16>(std::stoi(argv[2])) : 9000;

    Istemci istemci;

    QObject::connect(&istemci, &Istemci::baglandi, [&]() {
        qInfo() << "[OK] Sunucuya baglandi:" << host << ":" << port;
        qInfo() << "Mesaj yazin (Ctrl+D ile cik):";
    });

    QObject::connect(&istemci, &Istemci::koptu, [&]() {
        qInfo() << "[!] Sunucu baglantisi koptu.";
        QCoreApplication::quit();
    });

    QObject::connect(&istemci, &Istemci::mesajGeldi,
                     [&](const QByteArray& veri) {
        std::cout << "Sunucu> ";
        std::cout.write(veri.constData(), veri.size());
        std::cout.flush();
    });

    QObject::connect(&istemci, &Istemci::hataOlustu,
                     [&](const QString& aciklama) {
        qWarning() << "[HATA]" << aciklama;
    });

    // Olay dongusunu calistirmadan once baglanti talebini gecirmek icin
    // QTimer::singleShot ile post ediyoruz.
    QTimer::singleShot(0, [&]() { istemci.baglan(host, port); });

    // Stdin'i ayri bir thread'de oku, satirlari Qt slot'una post et.
    std::thread stdinThread([&]() {
        std::string satir;
        while (std::getline(std::cin, satir)) {
            satir.push_back('\n');
            QByteArray paket = QByteArray::fromStdString(satir);
            QMetaObject::invokeMethod(&istemci, "mesajGonder",
                                       Qt::QueuedConnection,
                                       Q_ARG(QByteArray, paket));
        }
        // EOF: ana donguyu durdur
        QMetaObject::invokeMethod(QCoreApplication::instance(), "quit",
                                   Qt::QueuedConnection);
    });
    stdinThread.detach();

    return app.exec();
}

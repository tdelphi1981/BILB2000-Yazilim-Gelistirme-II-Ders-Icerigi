/**
 * OBYS Protokol İstemcisi — Etkileşimli Konsol
 *
 * Bu uygulamada:
 *  - QTcpSocket ile sunucuya bağlan
 *  - Stdin'den komut oku, JSON'a sar, length-prefix ile gönder
 *  - Sunucudan gelen length-prefix paketleri çöz, JSON cevap yazdır
 *  - Komut sözdizimi:
 *      ping
 *      login <kullanici> <sifre>
 *      listele
 *      ekle <numara> <ad> <bolum>
 *      cikis
 *
 * Bölüm 12 Ünite 4 Konu 3
 *
 * Derleme:  mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştır: ./obys_istemci localhost 9200
 */

#include "CerceveCozucu.h"
#include "Protokol.h"

#include <QCoreApplication>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSocketNotifier>
#include <QStringList>
#include <QTcpSocket>
#include <QTextStream>
#include <QTimer>

#include <iostream>
#include <unistd.h>

namespace {

// Yardımcı: bir Komut'u çerçevele ve gönder
void komutuYolla(QTcpSocket& sok, const Komut& k) {
    QByteArray govde = komutuKodla(k);
    sok.write(cerceveyeSar(govde));
}

// Stdin'den gelen satırı parse et ve uygun Komut'a dönüştür
std::optional<Komut> satiriParseEt(const QString& satir, int& sonrakiId) {
    auto parcalar = satir.split(' ', Qt::SkipEmptyParts);
    if (parcalar.isEmpty()) return std::nullopt;

    Komut k;
    k.id = sonrakiId++;
    const QString komut = parcalar[0].toLower();

    if (komut == "ping") {
        k.komut = "ping";
        return k;
    }
    if (komut == "login" && parcalar.size() >= 3) {
        k.komut = "login";
        k.veri  = QJsonObject{{"kullanici", parcalar[1]}, {"sifre", parcalar[2]}};
        return k;
    }
    if (komut == "listele") {
        k.komut = "ogrenci_listele";
        return k;
    }
    if (komut == "ekle" && parcalar.size() >= 4) {
        k.komut = "ogrenci_ekle";
        k.veri  = QJsonObject{
            {"numara", parcalar[1].toInt()},
            {"ad",     parcalar[2]},
            {"bolum",  QStringList(parcalar.mid(3)).join(' ')},
        };
        return k;
    }
    return std::nullopt;
}

void yardimYazdir() {
    std::cout
        << "Komutlar:\n"
        << "  ping\n"
        << "  login <kullanici> <sifre>          (örn: login admin 1234)\n"
        << "  listele\n"
        << "  ekle <numara> <ad> <bolum>         (örn: ekle 100003 Mehmet Bilgisayar Mühendisliği)\n"
        << "  cikis\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    QStringList argumanlar = uygulama.arguments();
    if (argumanlar.size() < 3) {
        std::cerr << "Kullanım: " << argv[0] << " <hostname> <port>\n";
        return 1;
    }
    const QString sunucu = argumanlar[1];
    const quint16 port   = static_cast<quint16>(argumanlar[2].toUInt());

    QTcpSocket    soket;
    CerceveCozucu cozucu;
    int           sonrakiId = 1;

    QObject::connect(&soket, &QTcpSocket::connected, [&] {
        std::cout << "[OK] Bağlandı.\n";
        yardimYazdir();
    });

    QObject::connect(&soket, &QTcpSocket::errorOccurred,
                     [&](QAbstractSocket::SocketError) {
        std::cerr << "[HATA] " << soket.errorString().toStdString() << "\n";
        uygulama.quit();
    });

    QObject::connect(&soket, &QTcpSocket::disconnected, [&] {
        std::cout << "[INFO] Sunucu bağlantısı kesildi.\n";
        uygulama.quit();
    });

    QObject::connect(&soket, &QTcpSocket::readyRead, [&] {
        cozucu.besle(soket.readAll());
        try {
            while (auto paket = cozucu.sonrakiPaket()) {
                auto cevap = cevabiCoz(*paket);
                if (!cevap) {
                    std::cerr << "[!] Çözülemeyen paket: "
                              << paket->toStdString() << "\n";
                    continue;
                }
                std::cout << "[CEVAP id=" << cevap->id
                          << " durum=" << cevap->durum.toStdString() << "]\n";
                if (!cevap->hata.isEmpty()) {
                    std::cout << "  hata: " << cevap->hata.toStdString() << "\n";
                }
                if (!cevap->veri.isEmpty()) {
                    QByteArray ön = QJsonDocument(cevap->veri).toJson(QJsonDocument::Indented);
                    std::cout << "  veri: " << ön.toStdString();
                }
            }
        } catch (const std::exception& ex) {
            std::cerr << "[HATA] Çerçeve: " << ex.what() << "\n";
            soket.disconnectFromHost();
        }
    });

    // Stdin'i event loop içinde dinle
    QSocketNotifier stdinIzleyici(STDIN_FILENO, QSocketNotifier::Read);
    QObject::connect(&stdinIzleyici, &QSocketNotifier::activated, [&] {
        std::string satir;
        if (!std::getline(std::cin, satir)) {
            std::cout << "[INFO] EOF.\n";
            soket.disconnectFromHost();
            QTimer::singleShot(100, &uygulama, &QCoreApplication::quit);
            return;
        }

        QString trSatir = QString::fromStdString(satir).trimmed();
        if (trSatir.isEmpty()) return;
        if (trSatir == "cikis") {
            soket.disconnectFromHost();
            QTimer::singleShot(100, &uygulama, &QCoreApplication::quit);
            return;
        }

        auto k = satiriParseEt(trSatir, sonrakiId);
        if (!k) {
            std::cout << "[!] Anlaşılmayan komut.\n";
            yardimYazdir();
            return;
        }
        komutuYolla(soket, *k);
    });

    std::cout << "Bağlanılıyor: " << sunucu.toStdString()
              << ":" << port << "\n";
    soket.connectToHost(sunucu, port);

    return uygulama.exec();
}

/*
ÖRNEK OTURUM:
-------------
$ ./obys_istemci localhost 9200
Bağlanılıyor: localhost:9200
[OK] Bağlandı.
Komutlar:
  ping
  login <kullanici> <sifre>          (örn: login admin 1234)
  listele
  ekle <numara> <ad> <bolum>         (örn: ekle 100003 Mehmet Bilgisayar Mühendisliği)
  cikis

ping
[CEVAP id=1 durum=ok]
  veri: {
    "yanit": "pong"
}

listele
[CEVAP id=2 durum=hata]
  hata: Önce login olmalısın

login admin 1234
[CEVAP id=3 durum=ok]
  veri: {
    "yetki": "ogretmen"
}

listele
[CEVAP id=4 durum=ok]
  veri: {
    "ogrenciler": [
        { "numara": 100001, "ad": "Ali Yılmaz", "bolum": "..." },
        ...
    ]
}

ekle 100003 Mehmet Bilgisayar Mühendisliği
[CEVAP id=5 durum=ok]
  veri: {
    "eklendi": 100003,
    "toplam": 3
}

cikis
*/

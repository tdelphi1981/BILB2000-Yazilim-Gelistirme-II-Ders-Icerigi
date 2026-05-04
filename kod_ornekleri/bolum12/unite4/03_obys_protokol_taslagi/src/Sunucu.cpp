/**
 * Sunucu.cpp — OBYS sunucu protokol gerçekleştirimi
 *
 * Bölüm 12 Ünite 4 Konu 3
 */

#include "Sunucu.h"

#include <QDebug>
#include <QHostAddress>
#include <QJsonArray>

Sunucu::Sunucu(QObject* anaNesne)
    : QObject(anaNesne), m_sunucu(new QTcpServer(this)) {
    connect(m_sunucu, &QTcpServer::newConnection,
            this, &Sunucu::yeniBaglanti);

    // Komut tablosunu hazırla
    m_komutlar["ping"]            = [](Sunucu* s, QTcpSocket* sok, const Komut& k) {
        return s->ping(sok, k);
    };
    m_komutlar["login"]           = [](Sunucu* s, QTcpSocket* sok, const Komut& k) {
        return s->login(sok, k);
    };
    m_komutlar["ogrenci_listele"] = [](Sunucu* s, QTcpSocket* sok, const Komut& k) {
        return s->ogrenciListele(sok, k);
    };
    m_komutlar["ogrenci_ekle"]    = [](Sunucu* s, QTcpSocket* sok, const Komut& k) {
        return s->ogrenciEkle(sok, k);
    };

    // Demo verisi
    m_ogrenciler[100001] = {100001, QStringLiteral("Ali Yılmaz"),
                            QStringLiteral("Bilgisayar Mühendisliği")};
    m_ogrenciler[100002] = {100002, QStringLiteral("Ayşe Çağlar"),
                            QStringLiteral("Yazılım Mühendisliği")};
}

bool Sunucu::dinlemeBaslat(quint16 port) {
    if (!m_sunucu->listen(QHostAddress::Any, port)) {
        qCritical() << "Dinleme başlatılamadı:" << m_sunucu->errorString();
        return false;
    }
    qInfo() << "OBYS sunucu" << port << "portunda dinliyor.";
    return true;
}

void Sunucu::yeniBaglanti() {
    while (m_sunucu->hasPendingConnections()) {
        QTcpSocket* sok = m_sunucu->nextPendingConnection();
        m_cozucu[sok]        = CerceveCozucu{};
        m_girisYapildi[sok]  = false;

        qInfo() << "Bağlandı:" << sok->peerAddress().toString();

        connect(sok, &QTcpSocket::readyRead,    this, &Sunucu::istemciVeri);
        connect(sok, &QTcpSocket::disconnected, this, &Sunucu::istemciAyrildi);
    }
}

void Sunucu::istemciVeri() {
    auto* sok = qobject_cast<QTcpSocket*>(sender());
    if (!sok) return;

    auto& cozucu = m_cozucu[sok];
    cozucu.besle(sok->readAll());

    try {
        while (auto paket = cozucu.sonrakiPaket()) {
            auto cozulen = komutuCoz(*paket);
            if (!cozulen) {
                komutGonder(sok, hataCevabi(0, QStringLiteral("Geçersiz JSON")));
                continue;
            }
            komutTetikle(sok, *cozulen);
        }
    } catch (const std::exception& ex) {
        qWarning() << "Çerçeve hatası:" << ex.what();
        sok->disconnectFromHost();
    }
}

void Sunucu::istemciAyrildi() {
    auto* sok = qobject_cast<QTcpSocket*>(sender());
    if (!sok) return;
    qInfo() << "Ayrıldı:" << sok->peerAddress().toString();

    m_cozucu.remove(sok);
    m_girisYapildi.remove(sok);
    sok->deleteLater();
}

void Sunucu::komutGonder(QTcpSocket* hedef, const Cevap& cvp) {
    QByteArray govde = cevabiKodla(cvp);
    hedef->write(cerceveyeSar(govde));
}

void Sunucu::komutTetikle(QTcpSocket* gonderen, const Komut& k) {
    auto it = m_komutlar.find(k.komut);
    if (it == m_komutlar.end()) {
        komutGonder(gonderen,
                    hataCevabi(k.id,
                               QStringLiteral("Bilinmeyen komut: %1").arg(k.komut)));
        return;
    }

    // ping ve login dışındaki komutlar için oturum kontrolü
    if (k.komut != "ping" && k.komut != "login" &&
        !m_girisYapildi.value(gonderen, false)) {
        komutGonder(gonderen,
                    hataCevabi(k.id, QStringLiteral("Önce login olmalısın")));
        return;
    }

    Cevap cvp = it.value()(this, gonderen, k);
    komutGonder(gonderen, cvp);
}

// ---------------- KOMUT İŞLEYİCİLERİ ----------------

Cevap Sunucu::ping(QTcpSocket* /*s*/, const Komut& k) {
    QJsonObject veri{{"yanit", "pong"}};
    return okCevabi(k.id, veri);
}

Cevap Sunucu::login(QTcpSocket* sok, const Komut& k) {
    QString kullanici = k.veri.value("kullanici").toString();
    QString sifre     = k.veri.value("sifre").toString();

    // Demo: tek geçerli hesap
    if (kullanici == "admin" && sifre == "1234") {
        m_girisYapildi[sok] = true;
        return okCevabi(k.id, QJsonObject{{"yetki", "ogretmen"}});
    }
    return hataCevabi(k.id, QStringLiteral("Kullanıcı adı veya şifre hatalı"));
}

Cevap Sunucu::ogrenciListele(QTcpSocket* /*s*/, const Komut& k) {
    QJsonArray dizi;
    for (const auto& o : m_ogrenciler) {
        dizi.append(QJsonObject{
            {"numara", o.numara},
            {"ad",     o.ad},
            {"bolum",  o.bolum},
        });
    }
    return okCevabi(k.id, QJsonObject{{"ogrenciler", dizi}});
}

Cevap Sunucu::ogrenciEkle(QTcpSocket* /*s*/, const Komut& k) {
    int     numara = k.veri.value("numara").toInt(0);
    QString ad     = k.veri.value("ad").toString();
    QString bolum  = k.veri.value("bolum").toString();

    if (numara <= 0 || ad.isEmpty()) {
        return hataCevabi(k.id, QStringLiteral("Eksik alan: numara ve ad zorunlu"));
    }
    if (m_ogrenciler.contains(numara)) {
        return hataCevabi(k.id,
                          QStringLiteral("Bu numarayla kayıt zaten var: %1").arg(numara));
    }

    m_ogrenciler[numara] = {numara, ad, bolum};
    return okCevabi(k.id,
                    QJsonObject{{"eklendi", numara},
                                {"toplam",  m_ogrenciler.size()}});
}

/**
 * ObysSunucu uygulaması.
 */
#include "obys_sunucu.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>

ObysSunucu::ObysSunucu(QObject* parent)
    : QObject(parent),
      server_(new QTcpServer(this)) {
    // Demo veri
    ogrenciler_ = {
        {"OGR001", "Ali Yılmaz", 3.21},
        {"OGR002", "Ayşe Kaya", 3.85},
        {"OGR003", "Mehmet Demir", 2.74},
        {"OGR004", "Zeynep Şahin", 3.49},
    };

    // Demo kullanıcı: ogr001 / sifre123
    const QByteArray sha256 = QCryptographicHash::hash(
        QByteArray("sifre123"), QCryptographicHash::Sha256);
    kullaniciSifreler_["ogr001"] = QString::fromLatin1(sha256.toHex());

    connect(server_, &QTcpServer::newConnection,
            this, &ObysSunucu::yeniBaglanti);
}

bool ObysSunucu::baslat(quint16 port) {
    if (!server_->listen(QHostAddress::Any, port)) {
        qWarning() << "[Sunucu] listen() başarısız:"
                   << server_->errorString();
        return false;
    }
    qInfo() << "[Sunucu] Port" << port << "üzerinde dinliyor.";
    return true;
}

void ObysSunucu::yeniBaglanti() {
    while (server_->hasPendingConnections()) {
        QTcpSocket* istemci = server_->nextPendingConnection();
        qInfo() << "[Sunucu] Yeni istemci:" << istemci->peerAddress();

        connect(istemci, &QTcpSocket::readyRead,
                this, &ObysSunucu::veriHazir);
        connect(istemci, &QTcpSocket::disconnected,
                this, &ObysSunucu::baglantiKesildi);
    }
}

void ObysSunucu::veriHazir() {
    auto* istemci = qobject_cast<QTcpSocket*>(sender());
    if (!istemci) return;

    while (istemci->canReadLine()) {
        const QByteArray satir = istemci->readLine().trimmed();
        if (satir.isEmpty()) continue;

        const QString cevap = komutuIsle(istemci, QString::fromUtf8(satir));
        const QByteArray paket = cevap.toUtf8() + "\n";
        istemci->write(paket);
        istemci->flush();
    }
}

void ObysSunucu::baglantiKesildi() {
    auto* istemci = qobject_cast<QTcpSocket*>(sender());
    if (!istemci) return;

    oturumlar_.remove(istemci);
    qInfo() << "[Sunucu] Bağlantı kesildi.";
    istemci->deleteLater();
}

QString ObysSunucu::komutuIsle(QTcpSocket* istemci, const QString& satir) {
    const QString komut = jsonAlanOku(satir, "komut");

    if (komut == "PING") {
        return R"({"durum":"OK","mesaj":"PONG"})";
    }

    if (komut == "LOGIN") {
        const QString kullanici = jsonAlanOku(satir, "kullanici");
        const QString hash = jsonAlanOku(satir, "parola_hash");
        if (kullaniciSifreler_.value(kullanici) == hash) {
            const QString token = uretToken();
            oturumlar_[istemci] = token;
            return QString(R"({"durum":"LOGIN_OK","token":"%1"})")
                .arg(token);
        }
        return R"({"durum":"LOGIN_FAIL","hata":"Kimlik dogrulanamadi"})";
    }

    if (komut == "OGRENCI_LISTE") {
        const QString token = jsonAlanOku(satir, "token");
        if (oturumlar_.value(istemci) != token || token.isEmpty()) {
            return R"({"durum":"YETKISIZ","hata":"Once LOGIN gerekli"})";
        }

        QString liste = R"({"durum":"OK","kayitlar":[)";
        for (int i = 0; i < ogrenciler_.size(); ++i) {
            const auto& o = ogrenciler_[i];
            liste += QString(R"({"numara":"%1","ad":"%2","gano":%3})")
                .arg(o.numara, o.adSoyad)
                .arg(o.gano, 0, 'f', 2);
            if (i + 1 < ogrenciler_.size()) liste += ",";
        }
        liste += "]}";
        return liste;
    }

    return R"({"durum":"HATA","hata":"Bilinmeyen komut"})";
}

QString ObysSunucu::jsonAlanOku(const QString& metin,
                                const QString& anahtar) const {
    const QString desen = "\"" + anahtar + "\":";
    const int konum = metin.indexOf(desen);
    if (konum < 0) return {};

    int i = konum + desen.size();
    while (i < metin.size() && metin[i].isSpace()) ++i;
    if (i >= metin.size()) return {};

    if (metin[i] == '"') {
        const int son = metin.indexOf('"', i + 1);
        if (son < 0) return {};
        return metin.mid(i + 1, son - i - 1);
    }
    int son = i;
    while (son < metin.size()
           && metin[son] != ',' && metin[son] != '}') {
        ++son;
    }
    return metin.mid(i, son - i).trimmed();
}

QString ObysSunucu::uretToken() const {
    const auto t = QDateTime::currentMSecsSinceEpoch();
    const auto r = QRandomGenerator::global()->generate();
    return QString("tok_%1_%2").arg(t).arg(r, 0, 16);
}

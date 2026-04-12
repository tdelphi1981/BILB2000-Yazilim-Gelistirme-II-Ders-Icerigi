/**
 * json_exporter.cpp -- JsonExporter uygulamasi (OBYS v4)
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 4 - OBYS GUI Mini Proje Tamamlama
 *
 * QJsonDocument + QJsonObject + QJsonArray ile OBYSServis icerigini
 * JSON'a serilestirir. NotTuru variant'inin her alternatifi icin
 * overloaded + std::visit deseni kullanilir. Yazma islemi QSaveFile
 * ile atomik olarak yapilir (ayni dosya Bolum 7 Unite 5'teki
 * persistence_manager.cpp icinde de oldugu gibi).
 */

#include "json_exporter.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSaveFile>

#include <variant>

namespace {
// overloaded helper (C++17 sınıf sablonu argüman cikarsama deseni)
template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
} // namespace

QJsonObject JsonExporter::notTuruyuJson(const NotTuru& n) const {
    QJsonObject j;
    std::visit(overloaded{
        [&j](const SayisalNot& s) {
            j["tur"]   = QStringLiteral("sayisal");
            j["deger"] = s.deger;
        },
        [&j](const HarfNotu& h) {
            j["tur"]  = QStringLiteral("harf");
            j["harf"] = h.harf;
        },
        [&j](const MuafNot&) {
            j["tur"] = QStringLiteral("muaf");
        }
    }, n);
    return j;
}

QJsonObject JsonExporter::ogrenciyeJson(const Ogrenci& o) const {
    QJsonObject j;
    j["id"]      = o.id;
    j["numara"]  = o.numara;
    j["adSoyad"] = o.adSoyad;
    return j;
}

QJsonObject JsonExporter::dersyeJson(const Ders& d) const {
    QJsonObject j;
    j["id"]    = d.id;
    j["kod"]   = d.kod;
    j["ad"]    = d.ad;
    j["kredi"] = d.kredi;
    return j;
}

QJsonObject JsonExporter::notKaydiJson(const DersKaydi& k) const {
    QJsonObject j;
    j["ogrenciId"] = k.ogrenciId;
    j["dersId"]    = k.dersId;
    j["not"]       = notTuruyuJson(k.notu);
    return j;
}

bool JsonExporter::disaAktar(const QString& yol,
                             const OBYSServis& servis)
{
    m_sonHata.clear();

    QJsonObject kok;
    kok["surum"] = QStringLiteral("1.0.0");
    kok["olusturulmaZamani"] =
        QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonArray ogrArr;
    for (const auto& o : servis.tumOgrenciler()) {
        ogrArr.append(ogrenciyeJson(o));
    }
    kok["ogrenciler"] = ogrArr;

    QJsonArray drsArr;
    for (const auto& d : servis.tumDersler()) {
        drsArr.append(dersyeJson(d));
    }
    kok["dersler"] = drsArr;

    QJsonArray notArr;
    for (const auto& k : servis.tumNotlar()) {
        notArr.append(notKaydiJson(k));
    }
    kok["notlar"] = notArr;

    QJsonDocument belge(kok);
    const QByteArray veri = belge.toJson(QJsonDocument::Indented);

    QSaveFile dosya(yol);
    if (!dosya.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        m_sonHata = QStringLiteral(
            "JSON dosyasi acilamadi: %1").arg(dosya.errorString());
        return false;
    }
    if (dosya.write(veri) != veri.size()) {
        m_sonHata = QStringLiteral(
            "JSON yazma hatasi: %1").arg(dosya.errorString());
        dosya.cancelWriting();
        return false;
    }
    if (!dosya.commit()) {
        m_sonHata = QStringLiteral(
            "JSON tamamlanamadi: %1").arg(dosya.errorString());
        return false;
    }
    return true;
}

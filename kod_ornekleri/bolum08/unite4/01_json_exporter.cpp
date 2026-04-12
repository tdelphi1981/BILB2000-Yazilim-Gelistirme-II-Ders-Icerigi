/**
 * 01_json_exporter.cpp -- JsonExporter uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 4 - OBYS GUI Mini Proje Tamamlama
 *
 * QJsonDocument ve QJsonObject kullanarak OBYS verisini insan
 * okunabilir bir metin dosyasina yazar. std::visit yardimiyla
 * NotTuru variant'inin her alternatifi icin ayri bir JSON temsili
 * uretilir. overloaded helper'i (lambda agi) ile visit desenini
 * kullanmanin modern C++ bicimini gosterir.
 */

#include "01_json_exporter.h"

#include <QDateTime>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSaveFile>

#include <variant>

// -----------------------------------------------------------------
// overloaded helper: birden fazla lambda'yi tek bir callable haline
// getirir. std::visit ile variant ziyaretinde cok kullanisli.
// -----------------------------------------------------------------
namespace {
template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
} // namespace

// -----------------------------------------------------------------
// NotTuru -> QJsonObject donusumu
// -----------------------------------------------------------------
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

// -----------------------------------------------------------------
// Tekil kayit donusumleri
// -----------------------------------------------------------------
QJsonObject JsonExporter::ogrenciyeJson(const Ogrenci& o) const {
    QJsonObject j;
    j["id"]       = o.id;
    j["numara"]   = o.numara;
    j["adSoyad"]  = o.adSoyad;
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

// -----------------------------------------------------------------
// Ana disa aktarim metodu
// -----------------------------------------------------------------
bool JsonExporter::disaAktar(const QString& yol,
                             const OBYSServis& servis)
{
    m_sonHata.clear();

    // Kok nesne
    QJsonObject kok;
    kok["surum"] = QStringLiteral("1.0.0");
    kok["olusturulmaZamani"] =
        QDateTime::currentDateTime().toString(Qt::ISODate);

    // Ogrenciler dizisi
    QJsonArray ogrArr;
    for (const auto& o : servis.tumOgrenciler()) {
        ogrArr.append(ogrenciyeJson(o));
    }
    kok["ogrenciler"] = ogrArr;

    // Dersler dizisi
    QJsonArray drsArr;
    for (const auto& d : servis.tumDersler()) {
        drsArr.append(dersyeJson(d));
    }
    kok["dersler"] = drsArr;

    // Notlar dizisi
    QJsonArray notArr;
    for (const auto& k : servis.tumNotlar()) {
        notArr.append(notKaydiJson(k));
    }
    kok["notlar"] = notArr;

    // QJsonDocument olustur (indent'li yaz)
    QJsonDocument belge(kok);
    const QByteArray veri = belge.toJson(QJsonDocument::Indented);

    // QSaveFile ile atomik yazim: once gecici dosyaya, sonra rename
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

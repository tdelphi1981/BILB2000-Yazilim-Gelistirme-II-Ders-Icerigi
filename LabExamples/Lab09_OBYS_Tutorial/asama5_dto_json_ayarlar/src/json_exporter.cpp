#include "json_exporter.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSaveFile>
#include <variant>

// overloaded helper (C++17)
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

QJsonObject JsonExporter::notTuruyuJson(
    const NotTuru& n) const {
    QJsonObject j;
    std::visit(overloaded{
        [&j](const SayisalNot& s) {
            j["tur"]   = "sayisal";
            j["deger"] = s.deger;
        },
        [&j](const HarfNotu& h) {
            j["tur"]  = "harf";
            j["harf"] = h.harf;
        },
        [&j](const MuafNot&) {
            j["tur"] = "muaf";
        }
    }, n);
    return j;
}

QJsonObject JsonExporter::ogrenciyeJson(
    const Ogrenci& o) const {
    return {{"id", o.id},
            {"numara", o.numara},
            {"adSoyad", o.adSoyad}};
}

QJsonObject JsonExporter::dersyeJson(
    const Ders& d) const {
    return {{"id", d.id}, {"kod", d.kod},
            {"ad", d.ad}, {"kredi", d.kredi}};
}

QJsonObject JsonExporter::notKaydiJson(
    const DersKaydi& k) const {
    return {{"ogrenciId", k.ogrenciId},
            {"dersId", k.dersId},
            {"not", notTuruyuJson(k.notu)}};
}

bool JsonExporter::disaAktar(
    const QString& yol,
    const OBYSServis& servis) {
    m_sonHata.clear();

    QJsonObject kok;
    kok["surum"] = "1.0.0";
    kok["olusturulmaZamani"] =
        QDateTime::currentDateTime()
            .toString(Qt::ISODate);

    QJsonArray ogrArr;
    for (const auto& o :
         servis.tumOgrenciler())
        ogrArr.append(ogrenciyeJson(o));
    kok["ogrenciler"] = ogrArr;

    QJsonArray drsArr;
    for (const auto& d : servis.tumDersler())
        drsArr.append(dersyeJson(d));
    kok["dersler"] = drsArr;

    QJsonArray notArr;
    for (const auto& k : servis.tumNotlar())
        notArr.append(notKaydiJson(k));
    kok["notlar"] = notArr;

    QSaveFile dosya(yol);
    if (!dosya.open(QIODevice::WriteOnly)) {
        m_sonHata = dosya.errorString();
        return false;
    }
    const QByteArray veri =
        QJsonDocument(kok).toJson(
            QJsonDocument::Indented);
    dosya.write(veri);
    if (!dosya.commit()) {
        m_sonHata = dosya.errorString();
        return false;
    }
    return true;
}

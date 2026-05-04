/**
 * Protokol.cpp — JSON kodlama / çözme gerçekleştirimi
 *
 * Bölüm 12 Ünite 4 Konu 3
 */

#include "Protokol.h"

#include <QJsonDocument>

QByteArray komutuKodla(const Komut& k) {
    QJsonObject kok;
    kok["komut"] = k.komut;
    kok["id"]    = k.id;
    kok["veri"]  = k.veri;
    return QJsonDocument(kok).toJson(QJsonDocument::Compact);
}

std::optional<Komut> komutuCoz(const QByteArray& bayt) {
    QJsonParseError hata{};
    QJsonDocument belge = QJsonDocument::fromJson(bayt, &hata);
    if (hata.error != QJsonParseError::NoError || !belge.isObject()) {
        return std::nullopt;
    }
    QJsonObject kok = belge.object();
    if (!kok.contains("komut") || !kok["komut"].isString()) return std::nullopt;
    if (!kok.contains("id")    || !kok["id"].isDouble())    return std::nullopt;

    Komut k;
    k.komut = kok["komut"].toString();
    k.id    = kok["id"].toInt();
    k.veri  = kok.value("veri").toObject();
    return k;
}

QByteArray cevabiKodla(const Cevap& c) {
    QJsonObject kok;
    kok["id"]    = c.id;
    kok["durum"] = c.durum;
    if (!c.veri.isEmpty()) kok["veri"] = c.veri;
    if (!c.hata.isEmpty()) kok["hata"] = c.hata;
    return QJsonDocument(kok).toJson(QJsonDocument::Compact);
}

std::optional<Cevap> cevabiCoz(const QByteArray& bayt) {
    QJsonParseError hata{};
    QJsonDocument belge = QJsonDocument::fromJson(bayt, &hata);
    if (hata.error != QJsonParseError::NoError || !belge.isObject()) {
        return std::nullopt;
    }
    QJsonObject kok = belge.object();
    if (!kok.contains("id")    || !kok["id"].isDouble())    return std::nullopt;
    if (!kok.contains("durum") || !kok["durum"].isString()) return std::nullopt;

    Cevap c;
    c.id    = kok["id"].toInt();
    c.durum = kok["durum"].toString();
    c.veri  = kok.value("veri").toObject();
    c.hata  = kok.value("hata").toString();
    return c;
}

Cevap okCevabi(int id, const QJsonObject& veri) {
    Cevap c;
    c.id    = id;
    c.durum = QStringLiteral("ok");
    c.veri  = veri;
    return c;
}

Cevap hataCevabi(int id, const QString& mesaj) {
    Cevap c;
    c.id    = id;
    c.durum = QStringLiteral("hata");
    c.hata  = mesaj;
    return c;
}

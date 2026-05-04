/**
 * Protokol.h — OBYS komut/cevap paket yapıları (JSON)
 *
 * Komut formatı:  {"komut":"...","id":N,"veri":{...}}
 * Cevap formatı:  {"id":N,"durum":"ok|hata","veri":{...},"hata":"..."}
 *
 * Bölüm 12 Ünite 4 Konu 3
 */

#pragma once

#include <QByteArray>
#include <QJsonObject>
#include <QString>

#include <optional>

struct Komut {
    QString     komut;
    int         id;
    QJsonObject veri;
};

struct Cevap {
    int         id;
    QString     durum;   // "ok" veya "hata"
    QJsonObject veri;
    QString     hata;
};

QByteArray komutuKodla(const Komut& k);
std::optional<Komut> komutuCoz(const QByteArray& bayt);

QByteArray cevabiKodla(const Cevap& c);
std::optional<Cevap> cevabiCoz(const QByteArray& bayt);

Cevap okCevabi(int id, const QJsonObject& veri = {});
Cevap hataCevabi(int id, const QString& mesaj);

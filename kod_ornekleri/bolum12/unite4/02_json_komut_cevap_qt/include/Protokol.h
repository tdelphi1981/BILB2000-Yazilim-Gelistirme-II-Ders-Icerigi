/**
 * Protokol.h — JSON komut/cevap paket yapıları ve dönüşüm fonksiyonları
 *
 * Komut formatı:  {"komut":"...","id":N,"veri":{...}}
 * Cevap formatı:  {"id":N,"durum":"ok|hata","veri":{...},"hata":"..."}
 *
 * Bölüm 12 Ünite 4 Konu 2
 */

#pragma once

#include <QByteArray>
#include <QJsonObject>
#include <QString>

#include <optional>

// Sunucuya gönderilen istek paketi
struct Komut {
    QString     komut;   // Çağrılacak komutun adı (login, ping, ...)
    int         id;      // İsteği eşleştirmek için id
    QJsonObject veri;    // Komuta özel veri yükü
};

// Sunucudan dönen yanıt paketi
struct Cevap {
    int         id;      // Hangi komuta yanıt
    QString     durum;   // "ok" ya da "hata"
    QJsonObject veri;    // Yanıt yükü
    QString     hata;    // durum=="hata" ise hata mesajı
};

// Komut nesnesini JSON UTF-8 byte dizisine kodla
QByteArray komutuKodla(const Komut& k);

// JSON UTF-8 byte dizisinden Komut nesnesi çıkar (parse hatası -> nullopt)
std::optional<Komut> komutuCoz(const QByteArray& bayt);

// Cevap nesnesini JSON UTF-8 byte dizisine kodla
QByteArray cevabiKodla(const Cevap& c);

// JSON UTF-8 byte dizisinden Cevap nesnesi çıkar
std::optional<Cevap> cevabiCoz(const QByteArray& bayt);

// Yardımcı: hızlı hata cevabı oluştur
Cevap hataCevabi(int id, const QString& mesaj);

// Yardımcı: hızlı ok cevabı oluştur
Cevap okCevabi(int id, const QJsonObject& veri = {});

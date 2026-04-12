#ifndef OBYS_DOMAIN_H
#define OBYS_DOMAIN_H

#include <QDataStream>
#include <QString>
#include <variant>

// Not turleri -- std::variant ile tip guvenli birlestirme
struct SayisalNot { double deger{0.0}; };
struct HarfNotu   { QString harf; };
struct MuafNot    { };

using NotTuru = std::variant<SayisalNot, HarfNotu, MuafNot>;

struct Ogrenci {
    int     id{-1};
    QString numara;      // Ogrenci numarasi (ornegin "2023001")
    QString adSoyad;     // Ad ve soyad
};

struct Ders {
    int     id{-1};
    QString kod;         // Ders kodu (ornegin "BILB2000")
    QString ad;          // Ders adi
    int     kredi{0};    // Kredi sayisi
};

struct DersKaydi {
    int     ogrenciId{-1};
    int     dersId{-1};
    NotTuru notu{SayisalNot{0.0}};
};

// Ogrenci serilestirme
inline QDataStream& operator<<(QDataStream& s,
                                const Ogrenci& o) {
    s << static_cast<qint32>(o.id)
      << o.numara << o.adSoyad;
    return s;
}

inline QDataStream& operator>>(QDataStream& s, Ogrenci& o) {
    qint32 id = -1;
    s >> id >> o.numara >> o.adSoyad;
    o.id = id;
    return s;
}

// Ders serilestirme
inline QDataStream& operator<<(QDataStream& s,
                                const Ders& d) {
    s << static_cast<qint32>(d.id)
      << d.kod << d.ad
      << static_cast<qint32>(d.kredi);
    return s;
}

inline QDataStream& operator>>(QDataStream& s, Ders& d) {
    qint32 id = -1, kredi = 0;
    s >> id >> d.kod >> d.ad >> kredi;
    d.id    = id;
    d.kredi = kredi;
    return s;
}

// NotTuru: once etiket (0=Sayisal, 1=Harf, 2=Muaf)
inline QDataStream& operator<<(QDataStream& s,
                                const NotTuru& n) {
    const quint8 etiket = static_cast<quint8>(n.index());
    s << etiket;
    std::visit([&s](const auto& deger) {
        using T = std::decay_t<decltype(deger)>;
        if constexpr (std::is_same_v<T, SayisalNot>) {
            s << deger.deger;
        } else if constexpr (std::is_same_v<T, HarfNotu>) {
            s << deger.harf;
        }
        // MuafNot icin ek veri yok
    }, n);
    return s;
}

inline QDataStream& operator>>(QDataStream& s, NotTuru& n) {
    quint8 etiket = 0;
    s >> etiket;
    switch (etiket) {
        case 0: {
            SayisalNot v;
            s >> v.deger;
            n = v;
            break;
        }
        case 1: {
            HarfNotu v;
            s >> v.harf;
            n = v;
            break;
        }
        case 2:
            n = MuafNot{};
            break;
        default:
            s.setStatus(QDataStream::ReadCorruptData);
            break;
    }
    return s;
}

inline QDataStream& operator<<(QDataStream& s,
                                const DersKaydi& k) {
    s << static_cast<qint32>(k.ogrenciId)
      << static_cast<qint32>(k.dersId)
      << k.notu;
    return s;
}

inline QDataStream& operator>>(QDataStream& s,
                                DersKaydi& k) {
    qint32 ogr = -1, drs = -1;
    s >> ogr >> drs >> k.notu;
    k.ogrenciId = ogr;
    k.dersId    = drs;
    return s;
}

// Dialog ile ana pencere arasinda veri tasimak icin DTO
struct NotDTO {
    int     ogrenciId{-1};
    int     dersId{-1};
    NotTuru notu{SayisalNot{0.0}};
};

#endif // OBYS_DOMAIN_H

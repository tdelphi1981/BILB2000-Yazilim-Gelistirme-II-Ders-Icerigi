/**
 * obys_domain.h -- OBYS domain stub'lari (bagimsiz derleme icin)
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * Bu dosya, Bolum 3 Unite 5'te tanimlanan gercek OBYS domain
 * siniflarinin basitlestirilmis bir kopyasidir. Amaci, 04 numarali
 * tam ornek projesinin bagimsiz derlenebilmesini saglamaktir.
 * Gercek kitapta domain sinifi ayri bir kutuphane olarak
 * paylasilir; burada butunluk icin tekrar verilmistir.
 *
 * operator<< ve operator>> asiri yuklemeleri Bolum 7 Unite 5'teki
 * yaklasima sadik kalinarak hazirlanmistir.
 *
 * Derleme: Qt6::Core yeterlidir.
 */

#ifndef OBYS_DOMAIN_H
#define OBYS_DOMAIN_H

#include <QDataStream>
#include <QString>
#include <variant>

// -----------------------------------------------------------------
// NotTuru variant
// -----------------------------------------------------------------

struct SayisalNot { double deger{0.0}; };
struct HarfNotu   { QString harf; };
struct MuafNot    { };

using NotTuru = std::variant<SayisalNot, HarfNotu, MuafNot>;

// -----------------------------------------------------------------
// Temel domain struct'lari
// -----------------------------------------------------------------

struct Ogrenci {
    int     id{-1};
    QString numara;
    QString adSoyad;
};

struct Ders {
    int     id{-1};
    QString kod;
    QString ad;
    int     kredi{0};
};

struct DersKaydi {
    int     ogrenciId{-1};
    int     dersId{-1};
    NotTuru notu{SayisalNot{0.0}};
};

// -----------------------------------------------------------------
// QDataStream asiri yuklemeleri (Bolum 7 U5 tarzi)
// -----------------------------------------------------------------

inline QDataStream& operator<<(QDataStream& s, const Ogrenci& o) {
    s << static_cast<qint32>(o.id) << o.numara << o.adSoyad;
    return s;
}

inline QDataStream& operator>>(QDataStream& s, Ogrenci& o) {
    qint32 id = -1;
    s >> id >> o.numara >> o.adSoyad;
    o.id = id;
    return s;
}

inline QDataStream& operator<<(QDataStream& s, const Ders& d) {
    s << static_cast<qint32>(d.id)
      << d.kod << d.ad
      << static_cast<qint32>(d.kredi);
    return s;
}

inline QDataStream& operator>>(QDataStream& s, Ders& d) {
    qint32 id = -1;
    qint32 kredi = 0;
    s >> id >> d.kod >> d.ad >> kredi;
    d.id    = id;
    d.kredi = kredi;
    return s;
}

// NotTuru serilestirmesi: once etiket (tag) sonra icerik
inline QDataStream& operator<<(QDataStream& s, const NotTuru& n) {
    const quint8 etiket = static_cast<quint8>(n.index());
    s << etiket;
    std::visit([&s](const auto& deger) {
        using T = std::decay_t<decltype(deger)>;
        if constexpr (std::is_same_v<T, SayisalNot>) {
            s << deger.deger;
        } else if constexpr (std::is_same_v<T, HarfNotu>) {
            s << deger.harf;
        } else if constexpr (std::is_same_v<T, MuafNot>) {
            // icerik yok
        }
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
        case 2: {
            n = MuafNot{};
            break;
        }
        default:
            s.setStatus(QDataStream::ReadCorruptData);
            break;
    }
    return s;
}

inline QDataStream& operator<<(QDataStream& s, const DersKaydi& k) {
    s << static_cast<qint32>(k.ogrenciId)
      << static_cast<qint32>(k.dersId)
      << k.notu;
    return s;
}

inline QDataStream& operator>>(QDataStream& s, DersKaydi& k) {
    qint32 ogr = -1;
    qint32 drs = -1;
    s >> ogr >> drs >> k.notu;
    k.ogrenciId = ogr;
    k.dersId    = drs;
    return s;
}

// NotGirisDialog'un urettigi DTO (dialog ile ayni yapiyi paylasir)
struct NotDTO {
    int     ogrenciId{-1};
    int     dersId{-1};
    NotTuru notu{SayisalNot{0.0}};
};

#endif // OBYS_DOMAIN_H

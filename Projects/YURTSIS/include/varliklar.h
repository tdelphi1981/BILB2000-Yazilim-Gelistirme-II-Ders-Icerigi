#pragma once

/**
 * varliklar.h -- YURTSIS Domain Varlıkları
 *
 * Öğrenci Yurt Yönetim Sistemi için temel veri yapıları.
 *
 * Bölüm: Proje Ödevi - YURTSIS
 * Derleme: g++ -std=c++20 -o yurtsis src/main.cpp
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Enum Tanımları
// ---------------------------------------------------------------------------

enum class OdemeDurum {
    Odendi,        // Odeme yapildi
    Beklemede,     // Odeme henuz yapilmadi
    Gecikmis       // Odeme suresi gecmis
};

// Durum enum'unu metne cevirir
inline std::string durumYazisi(OdemeDurum durum) {
    switch (durum) {
        case OdemeDurum::Odendi:    return "Odendi";
        case OdemeDurum::Beklemede: return "Beklemede";
        case OdemeDurum::Gecikmis:  return "Gecikmis";
    }
    return "Bilinmiyor";
}

// ---------------------------------------------------------------------------
// Ogrenci
// ---------------------------------------------------------------------------

struct Ogrenci {
    int numara{};
    std::string isim;
    std::string soyisim;
    std::string bolum;
    int sinif{};
};

inline std::ostream& operator<<(std::ostream& os, const Ogrenci& o) {
    os << "[" << o.numara << "] " << o.isim << " " << o.soyisim
       << " (" << o.bolum << ", " << o.sinif << ". sinif)";
    return os;
}

// ---------------------------------------------------------------------------
// Oda
// ---------------------------------------------------------------------------

struct Oda {
    std::string oda_no;       // Ornegin "A-101"
    std::string blok;         // Ornegin "A"
    int kapasite{};
    std::string oda_tipi;     // "Tek", "Cift", "Uc", "Dort"
    double aylik_ucret{};
};

inline std::ostream& operator<<(std::ostream& os, const Oda& o) {
    os << "[" << o.oda_no << "] Blok: " << o.blok
       << ", Kapasite: " << o.kapasite
       << ", Tip: " << o.oda_tipi
       << ", Ucret: " << o.aylik_ucret << " TL";
    return os;
}

// ---------------------------------------------------------------------------
// Yerlestirme Kaydı
// ---------------------------------------------------------------------------

struct YerlestirmeKaydi {
    int kayit_id{};
    int ogrenci_no{};
    std::string oda_no;
    std::string donem;                        // "2025-Bahar"
    std::string baslangic;                    // "YYYY-MM-DD"
    std::optional<std::string> bitis;         // Devam ediyorsa nullopt
};

inline std::ostream& operator<<(std::ostream& os, const YerlestirmeKaydi& yk) {
    os << "[Kayit #" << yk.kayit_id << "] Ogrenci: " << yk.ogrenci_no
       << ", Oda: " << yk.oda_no
       << ", Donem: " << yk.donem
       << ", Baslangic: " << yk.baslangic
       << ", Bitis: " << yk.bitis.value_or("---");
    return os;
}

// ---------------------------------------------------------------------------
// Odeme Kaydı
// ---------------------------------------------------------------------------

struct OdemeKaydi {
    int odeme_id{};
    int ogrenci_no{};
    double tutar{};
    std::string tarih;                        // "YYYY-MM-DD"
    OdemeDurum durum{OdemeDurum::Beklemede};
};

inline std::ostream& operator<<(std::ostream& os, const OdemeKaydi& ok) {
    os << "[Odeme #" << ok.odeme_id << "] Ogrenci: " << ok.ogrenci_no
       << ", Tutar: " << ok.tutar << " TL"
       << ", Tarih: " << ok.tarih
       << ", Durum: " << durumYazisi(ok.durum);
    return os;
}

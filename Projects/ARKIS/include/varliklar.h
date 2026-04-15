#pragma once

/**
 * varliklar.h -- ARKIS Domain Varlıkları
 *
 * Araç Kiralama Sistemi için temel veri yapıları.
 *
 * Bölüm: Proje Ödevi - ARKIS
 * Derleme: g++ -std=c++20 -o arkis src/main.cpp
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Enum Tanımları
// ---------------------------------------------------------------------------

enum class AracDurum {
    Musait,        // Araç kiralanabilir durumda
    Kirada,        // Araç şu anda kiralanmış
    Bakimda        // Araç bakımda, kiralanmaz
};

// Durum enum'unu metne çevirir
inline std::string durumYazisi(AracDurum durum) {
    switch (durum) {
        case AracDurum::Musait:  return "Musait";
        case AracDurum::Kirada:  return "Kirada";
        case AracDurum::Bakimda: return "Bakimda";
    }
    return "Bilinmiyor";
}

// ---------------------------------------------------------------------------
// Araç
// ---------------------------------------------------------------------------

struct Arac {
    std::string plaka;
    std::string marka;
    std::string model;
    int yil{};
    std::string yakit_tipi;
    double gunluk_ucret{};
    AracDurum durum{AracDurum::Musait};
};

inline std::ostream& operator<<(std::ostream& os, const Arac& a) {
    os << "[" << a.plaka << "] " << a.marka << " " << a.model
       << " (" << a.yil << ")"
       << " [Yakit: " << a.yakit_tipi
       << ", Ucret: " << a.gunluk_ucret << " TL/gun"
       << ", Durum: " << durumYazisi(a.durum) << "]";
    return os;
}

// ---------------------------------------------------------------------------
// Müşteri
// ---------------------------------------------------------------------------

struct Musteri {
    std::string tc_no;
    std::string isim;
    std::string soyisim;
    std::string telefon;
    std::string ehliyet_no;
};

inline std::ostream& operator<<(std::ostream& os, const Musteri& m) {
    os << "[TC: " << m.tc_no << "] " << m.isim << " " << m.soyisim
       << " (Tel: " << m.telefon
       << ", Ehliyet: " << m.ehliyet_no << ")";
    return os;
}

// ---------------------------------------------------------------------------
// Kiralama Sözleşmesi
// ---------------------------------------------------------------------------

struct KiralamaSozlesmesi {
    int sozlesme_id{};
    std::string plaka;
    std::string tc_no;
    std::string baslangic_tarihi;                // "YYYY-MM-DD"
    std::optional<std::string> bitis_tarihi;     // Devam ediyorsa nullopt
    double toplam_tutar{};
};

inline std::ostream& operator<<(std::ostream& os, const KiralamaSozlesmesi& ks) {
    os << "[Sozlesme #" << ks.sozlesme_id << "] Plaka: " << ks.plaka
       << ", TC: " << ks.tc_no
       << ", Baslangic: " << ks.baslangic_tarihi
       << ", Bitis: " << ks.bitis_tarihi.value_or("---")
       << ", Tutar: " << ks.toplam_tutar << " TL";
    return os;
}

#pragma once

/**
 * varliklar.h -- HARSIS Domain Varlıkları
 *
 * Hastane Randevu Sistemi için temel veri yapıları.
 *
 * Bölüm: Proje Ödevi - HARSIS
 * Derleme: g++ -std=c++20 -o harsis src/main.cpp
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Enum Tanımları
// ---------------------------------------------------------------------------

enum class RandevuDurum {
    Beklemede,     // Randevu henüz onaylanmadı
    Onaylandi,     // Randevu onaylandı
    Iptal,         // Randevu iptal edildi
    Tamamlandi     // Randevu gerçekleşti
};

// Durum enum'unu metne çevirir
inline std::string durumYazisi(RandevuDurum durum) {
    switch (durum) {
        case RandevuDurum::Beklemede:   return "Beklemede";
        case RandevuDurum::Onaylandi:   return "Onaylandi";
        case RandevuDurum::Iptal:       return "Iptal";
        case RandevuDurum::Tamamlandi:  return "Tamamlandi";
    }
    return "Bilinmiyor";
}

// ---------------------------------------------------------------------------
// Hasta
// ---------------------------------------------------------------------------

struct Hasta {
    std::string tc_no;
    std::string isim;
    std::string soyisim;
    std::string dogum_tarihi;  // "YYYY-MM-DD" formatında
    std::string telefon;
};

inline std::ostream& operator<<(std::ostream& os, const Hasta& h) {
    os << "[TC: " << h.tc_no << "] " << h.isim << " " << h.soyisim
       << " (Dogum: " << h.dogum_tarihi
       << ", Tel: " << h.telefon << ")";
    return os;
}

// ---------------------------------------------------------------------------
// Doktor
// ---------------------------------------------------------------------------

struct Doktor {
    std::string sicil_no;
    std::string isim;
    std::string soyisim;
    std::string brans;
    std::vector<std::string> uygun_saatler;
};

inline std::ostream& operator<<(std::ostream& os, const Doktor& d) {
    os << "[Sicil: " << d.sicil_no << "] Dr. " << d.isim << " " << d.soyisim
       << " (Brans: " << d.brans << ", Uygun saatler:";
    for (const auto& saat : d.uygun_saatler) {
        os << " " << saat;
    }
    os << ")";
    return os;
}

// ---------------------------------------------------------------------------
// Randevu
// ---------------------------------------------------------------------------

struct Randevu {
    int randevu_id{};
    std::string tc_no;
    std::string sicil_no;
    std::string tarih;                        // "YYYY-MM-DD"
    std::string saat;                         // "HH:MM"
    RandevuDurum durum{RandevuDurum::Beklemede};
    std::optional<std::string> notlar;        // Doktor/hasta notları, nullopt olabilir
};

inline std::ostream& operator<<(std::ostream& os, const Randevu& r) {
    os << "[Randevu #" << r.randevu_id << "] Hasta TC: " << r.tc_no
       << ", Doktor: " << r.sicil_no
       << ", Tarih: " << r.tarih << " " << r.saat
       << ", Durum: " << durumYazisi(r.durum)
       << ", Not: " << r.notlar.value_or("---");
    return os;
}

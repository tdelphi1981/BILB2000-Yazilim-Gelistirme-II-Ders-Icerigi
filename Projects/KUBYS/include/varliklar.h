#pragma once

/**
 * varliklar.h -- KUBYS Domain Varlıkları
 *
 * Kütüphane Yönetim Sistemi için temel veri yapıları.
 *
 * Bölüm: Proje Ödevi - KUBYS
 * Derleme: g++ -std=c++20 -o kubys src/main.cpp
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Enum Tanımları
// ---------------------------------------------------------------------------

enum class OduncDurum {
    Oduncte,       // Kitap hala ödünç alınmış durumda
    IadeEdildi,    // Kitap iade edildi
    Gecikmis       // İade süresi geçmiş
};

// Durum enum'unu metne çevirir
inline std::string durumYazisi(OduncDurum durum) {
    switch (durum) {
        case OduncDurum::Oduncte:    return "Oduncte";
        case OduncDurum::IadeEdildi: return "Iade Edildi";
        case OduncDurum::Gecikmis:   return "Gecikmis";
    }
    return "Bilinmiyor";
}

// ---------------------------------------------------------------------------
// Kitap
// ---------------------------------------------------------------------------

struct Kitap {
    std::string isbn;
    std::string baslik;
    std::string yazar;
    int yayin_yili{};
    std::string kategori;
    int kopya_sayisi{};
};

inline std::ostream& operator<<(std::ostream& os, const Kitap& k) {
    os << "[" << k.isbn << "] " << k.baslik
       << " - " << k.yazar << " (" << k.yayin_yili << ")"
       << " [Kategori: " << k.kategori
       << ", Kopya: " << k.kopya_sayisi << "]";
    return os;
}

// ---------------------------------------------------------------------------
// Üye
// ---------------------------------------------------------------------------

struct Uye {
    int uye_no{};
    std::string isim;
    std::string soyisim;
    std::string telefon;
    std::string kayit_tarihi;  // "YYYY-MM-DD" formatında
};

inline std::ostream& operator<<(std::ostream& os, const Uye& u) {
    os << "[" << u.uye_no << "] " << u.isim << " " << u.soyisim
       << " (Tel: " << u.telefon << ")";
    return os;
}

// ---------------------------------------------------------------------------
// Ödünç Kaydı
// ---------------------------------------------------------------------------

struct OduncKaydi {
    int kayit_id{};
    int uye_no{};
    std::string isbn;
    std::string odunc_tarihi;               // "YYYY-MM-DD"
    std::optional<std::string> iade_tarihi; // Henüz iade edilmediyse nullopt
    OduncDurum durum{OduncDurum::Oduncte};
};

inline std::ostream& operator<<(std::ostream& os, const OduncKaydi& ok) {
    os << "[Kayit #" << ok.kayit_id << "] Uye: " << ok.uye_no
       << ", ISBN: " << ok.isbn
       << ", Odunc: " << ok.odunc_tarihi
       << ", Iade: " << ok.iade_tarihi.value_or("---")
       << ", Durum: " << durumYazisi(ok.durum);
    return os;
}

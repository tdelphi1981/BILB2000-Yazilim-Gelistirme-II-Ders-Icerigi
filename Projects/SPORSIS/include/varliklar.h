#pragma once

/**
 * varliklar.h -- SPORSIS Domain Varlıkları
 *
 * Spor Salonu Üyelik Sistemi için temel veri yapıları.
 *
 * Bölüm: Proje Ödevi - SPORSIS
 * Derleme: g++ -std=c++20 -o sporsis src/main.cpp
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Enum Tanımları
// ---------------------------------------------------------------------------

enum class UyelikDurum {
    Aktif,           // Üyelik hala geçerli
    SuresiDolmus,    // Üyelik süresi sona ermiş
    Dondurulmus      // Üyelik geçici olarak dondurulmuş
};

// Durum enum'unu metne çevirir
inline std::string durumYazisi(UyelikDurum durum) {
    switch (durum) {
        case UyelikDurum::Aktif:         return "Aktif";
        case UyelikDurum::SuresiDolmus:  return "Suresi Dolmus";
        case UyelikDurum::Dondurulmus:   return "Dondurulmus";
    }
    return "Bilinmiyor";
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
       << " (Tel: " << u.telefon
       << ", Kayit: " << u.kayit_tarihi << ")";
    return os;
}

// ---------------------------------------------------------------------------
// Üyelik Paketi
// ---------------------------------------------------------------------------

struct UyelikPaketi {
    int paket_id{};
    std::string paket_adi;
    int sure_gun{};       // Paketin süresi (gün cinsinden)
    double fiyat{};       // Paket fiyatı (TL)
};

inline std::ostream& operator<<(std::ostream& os, const UyelikPaketi& p) {
    os << "[Paket #" << p.paket_id << "] " << p.paket_adi
       << " (" << p.sure_gun << " gun, " << p.fiyat << " TL)";
    return os;
}

// ---------------------------------------------------------------------------
// Üyelik Kaydı
// ---------------------------------------------------------------------------

struct UyelikKaydi {
    int kayit_id{};
    int uye_no{};
    int paket_id{};
    std::string baslangic;                  // "YYYY-MM-DD"
    std::optional<std::string> bitis;       // Aktif üyeliklerde henüz belli olmayabilir
    UyelikDurum durum{UyelikDurum::Aktif};
};

inline std::ostream& operator<<(std::ostream& os, const UyelikKaydi& uk) {
    os << "[Kayit #" << uk.kayit_id << "] Uye: " << uk.uye_no
       << ", Paket: " << uk.paket_id
       << ", Baslangic: " << uk.baslangic
       << ", Bitis: " << uk.bitis.value_or("---")
       << ", Durum: " << durumYazisi(uk.durum);
    return os;
}

// ---------------------------------------------------------------------------
// Antrenman Kaydı
// ---------------------------------------------------------------------------

struct AntrenmanKaydi {
    int kayit_id{};
    int uye_no{};
    std::string tarih;    // "YYYY-MM-DD"
    int sure_dk{};        // Antrenman süresi (dakika)
    std::string aktivite; // Örn: "Kardiyo", "Agirlik", "Yoga"
};

inline std::ostream& operator<<(std::ostream& os, const AntrenmanKaydi& ak) {
    os << "[Antrenman #" << ak.kayit_id << "] Uye: " << ak.uye_no
       << ", Tarih: " << ak.tarih
       << ", Sure: " << ak.sure_dk << " dk"
       << ", Aktivite: " << ak.aktivite;
    return os;
}

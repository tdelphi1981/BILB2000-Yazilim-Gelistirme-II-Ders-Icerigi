#pragma once

/**
 * varliklar.h -- RESYS Domain Varlıkları
 *
 * Restoran Sipariş ve Stok Yönetim Sistemi için temel veri yapıları.
 *
 * Bölüm: Proje Ödevi - RESYS
 * Derleme: g++ -std=c++20 -o resys src/main.cpp
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Enum Tanımları
// ---------------------------------------------------------------------------

enum class SiparisDurum {
    Hazirlaniyor,   // Sipariş mutfakta hazırlanıyor
    Hazir,          // Sipariş hazır, servise bekliyor
    TeslimEdildi,   // Sipariş masaya teslim edildi
    Iptal           // Sipariş iptal edildi
};

// Durum enum'unu metne çevirir
inline std::string durumYazisi(SiparisDurum durum) {
    switch (durum) {
        case SiparisDurum::Hazirlaniyor: return "Hazirlaniyor";
        case SiparisDurum::Hazir:        return "Hazir";
        case SiparisDurum::TeslimEdildi: return "Teslim Edildi";
        case SiparisDurum::Iptal:        return "Iptal";
    }
    return "Bilinmiyor";
}

// ---------------------------------------------------------------------------
// MenuItem (Menü Öğesi)
// ---------------------------------------------------------------------------

struct MenuItem {
    int urun_id{};
    std::string urun_adi;
    std::string kategori;
    double fiyat{};
};

inline std::ostream& operator<<(std::ostream& os, const MenuItem& m) {
    os << "[" << m.urun_id << "] " << m.urun_adi
       << " (" << m.kategori << ") "
       << m.fiyat << " TL";
    return os;
}

// ---------------------------------------------------------------------------
// SiparisKalemi (Sipariş Kalemi)
// ---------------------------------------------------------------------------

struct SiparisKalemi {
    int urun_id{};
    int adet{};
    std::optional<std::string> notlar;  // Opsiyonel: "az pişmiş", "soslu" vb.
};

inline std::ostream& operator<<(std::ostream& os, const SiparisKalemi& sk) {
    os << "Urun #" << sk.urun_id
       << " x" << sk.adet;
    if (sk.notlar.has_value()) {
        os << " [Not: " << sk.notlar.value() << "]";
    }
    return os;
}

// ---------------------------------------------------------------------------
// Sipariş
// ---------------------------------------------------------------------------

struct Siparis {
    int siparis_id{};
    int masa_no{};
    std::string tarih_saat;  // "YYYY-MM-DD HH:MM" formatında
    std::vector<SiparisKalemi> kalemler;
    SiparisDurum durum{SiparisDurum::Hazirlaniyor};
};

inline std::ostream& operator<<(std::ostream& os, const Siparis& s) {
    os << "[Siparis #" << s.siparis_id << "] Masa: " << s.masa_no
       << ", Tarih: " << s.tarih_saat
       << ", Durum: " << durumYazisi(s.durum)
       << ", Kalem sayisi: " << s.kalemler.size();
    return os;
}

// ---------------------------------------------------------------------------
// StokMalzeme (Stok Malzemesi)
// ---------------------------------------------------------------------------

struct StokMalzeme {
    int malzeme_id{};
    std::string malzeme_adi;
    double miktar{};
    std::string birim;        // "kg", "lt", "adet" vb.
    double minimum_stok{};
};

inline std::ostream& operator<<(std::ostream& os, const StokMalzeme& sm) {
    os << "[" << sm.malzeme_id << "] " << sm.malzeme_adi
       << ": " << sm.miktar << " " << sm.birim
       << " (Min: " << sm.minimum_stok << " " << sm.birim << ")";
    return os;
}

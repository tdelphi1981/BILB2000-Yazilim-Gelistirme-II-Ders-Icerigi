#pragma once
// dosyalama.h - Binary dosyalama modulu (header-only)
// reinterpret_cast tabanli POD yazma/okuma
// string, variant, map ve Depo icin ozel surumler
//
// Bolum: 03 - Unite 5
// Derleme: C++20

#include <fstream>
#include <string>
#include <map>
#include <variant>
#include <cstdint>

#include "varliklar/ogrenci.h"
#include "varliklar/ders.h"
#include "varliklar/ders_kaydi.h"
#include "depo.h"

// ============================================================
// Genel binary yazma/okuma (POD turler)
// ============================================================

template <typename T>
inline void dosyaya_yaz(std::ofstream& dosya, const T& veri) {
    dosya.write(reinterpret_cast<const char*>(&veri), sizeof(T));
}

template <typename T>
inline void dosyadan_oku(std::ifstream& dosya, T& veri) {
    dosya.read(reinterpret_cast<char*>(&veri), sizeof(T));
}

// ============================================================
// string specialization: once boyut, sonra veri
// ============================================================

template <>
inline void dosyaya_yaz<std::string>(std::ofstream& dosya,
                                     const std::string& veri) {
    std::size_t boyut = veri.size();
    dosya.write(reinterpret_cast<const char*>(&boyut), sizeof(boyut));
    dosya.write(veri.data(), static_cast<std::streamsize>(boyut));
}

template <>
inline void dosyadan_oku<std::string>(std::ifstream& dosya,
                                      std::string& veri) {
    std::size_t boyut = 0;
    dosya.read(reinterpret_cast<char*>(&boyut), sizeof(boyut));
    veri.resize(boyut);
    dosya.read(veri.data(), static_cast<std::streamsize>(boyut));
}

// ============================================================
// Muaf specialization: sebep string'i ile birlikte
// ============================================================

template <>
inline void dosyaya_yaz<Muaf>(std::ofstream& dosya, const Muaf& veri) {
    dosyaya_yaz(dosya, veri.sebep);
}

template <>
inline void dosyadan_oku<Muaf>(std::ifstream& dosya, Muaf& veri) {
    dosyadan_oku(dosya, veri.sebep);
}

// ============================================================
// variant<int, string, Muaf> (NotTuru)
// Yazma: once index() sonra deger
// Okuma: once index oku, sonra uygun turu oku
// ============================================================

inline void dosyaya_yaz(std::ofstream& dosya, const NotTuru& veri) {
    uint8_t idx = static_cast<uint8_t>(veri.index());
    dosya.write(reinterpret_cast<const char*>(&idx), sizeof(idx));

    std::visit([&dosya](const auto& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
            dosyaya_yaz(dosya, val);
        } else if constexpr (std::is_same_v<T, std::string>) {
            dosyaya_yaz(dosya, val);
        } else {
            // Muaf
            dosyaya_yaz(dosya, val);
        }
    }, veri);
}

inline void dosyadan_oku(std::ifstream& dosya, NotTuru& veri) {
    uint8_t idx = 0;
    dosya.read(reinterpret_cast<char*>(&idx), sizeof(idx));

    switch (idx) {
        case 0: {
            int val{};
            dosyadan_oku(dosya, val);
            veri = val;
            break;
        }
        case 1: {
            std::string val;
            dosyadan_oku(dosya, val);
            veri = val;
            break;
        }
        case 2: {
            Muaf val;
            dosyadan_oku(dosya, val);
            veri = val;
            break;
        }
    }
}

// ============================================================
// Ogrenci: numara + isim + soyisim + dogum_tarihi + cinsiyet
// ============================================================

template <>
inline void dosyaya_yaz<Ogrenci>(std::ofstream& dosya,
                                  const Ogrenci& veri) {
    dosyaya_yaz(dosya, veri.numara);
    dosyaya_yaz(dosya, veri.isim);
    dosyaya_yaz(dosya, veri.soyisim);
    dosyaya_yaz(dosya, veri.dogum_tarihi);
    dosyaya_yaz(dosya, veri.cinsiyet);
}

template <>
inline void dosyadan_oku<Ogrenci>(std::ifstream& dosya, Ogrenci& veri) {
    dosyadan_oku(dosya, veri.numara);
    dosyadan_oku(dosya, veri.isim);
    dosyadan_oku(dosya, veri.soyisim);
    dosyadan_oku(dosya, veri.dogum_tarihi);
    dosyadan_oku(dosya, veri.cinsiyet);
}

// ============================================================
// Ders: ders_kodu + ders_adi
// ============================================================

template <>
inline void dosyaya_yaz<Ders>(std::ofstream& dosya, const Ders& veri) {
    dosyaya_yaz(dosya, veri.ders_kodu);
    dosyaya_yaz(dosya, veri.ders_adi);
}

template <>
inline void dosyadan_oku<Ders>(std::ifstream& dosya, Ders& veri) {
    dosyadan_oku(dosya, veri.ders_kodu);
    dosyadan_oku(dosya, veri.ders_adi);
}

// ============================================================
// DersKaydi: ogrenci_no + ders_kodu + vize + final_notu + odev
// ============================================================

template <>
inline void dosyaya_yaz<DersKaydi>(std::ofstream& dosya,
                                    const DersKaydi& veri) {
    dosyaya_yaz(dosya, veri.ogrenci_no);
    dosyaya_yaz(dosya, veri.ders_kodu);
    dosyaya_yaz(dosya, veri.vize);
    dosyaya_yaz(dosya, veri.final_notu);
    dosyaya_yaz(dosya, veri.odev);
}

template <>
inline void dosyadan_oku<DersKaydi>(std::ifstream& dosya,
                                     DersKaydi& veri) {
    dosyadan_oku(dosya, veri.ogrenci_no);
    dosyadan_oku(dosya, veri.ders_kodu);
    dosyadan_oku(dosya, veri.vize);
    dosyadan_oku(dosya, veri.final_notu);
    dosyadan_oku(dosya, veri.odev);
}

// ============================================================
// map<K,V>: boyut + key/value ciftleri
// ============================================================

template <typename K, typename V>
inline void dosyaya_yaz(std::ofstream& dosya,
                         const std::map<K, V>& veri) {
    std::size_t boyut = veri.size();
    dosya.write(reinterpret_cast<const char*>(&boyut), sizeof(boyut));

    for (const auto& [anahtar, deger] : veri) {
        dosyaya_yaz(dosya, anahtar);
        dosyaya_yaz(dosya, deger);
    }
}

template <typename K, typename V>
inline void dosyadan_oku(std::ifstream& dosya, std::map<K, V>& veri) {
    std::size_t boyut = 0;
    dosya.read(reinterpret_cast<char*>(&boyut), sizeof(boyut));

    veri.clear();
    for (std::size_t i = 0; i < boyut; ++i) {
        K anahtar{};
        V deger{};
        dosyadan_oku(dosya, anahtar);
        dosyadan_oku(dosya, deger);
        veri.emplace(std::move(anahtar), std::move(deger));
    }
}

// ============================================================
// Depo<K,V>: tumunu_al() ile map'i yaz, ekle() ile geri yukle
// ============================================================

template <typename K, typename V>
inline void dosyaya_yaz(std::ofstream& dosya,
                         const Depo<K, V>& depo) {
    dosyaya_yaz(dosya, depo.tumunu_al());
}

template <typename K, typename V>
inline void dosyadan_oku(std::ifstream& dosya, Depo<K, V>& depo) {
    std::map<K, V> gecici;
    dosyadan_oku(dosya, gecici);

    depo.temizle();
    for (auto& [anahtar, deger] : gecici) {
        depo.ekle(anahtar, std::move(deger));
    }
}

// ============================================================
// Yardimci: Dosyaya kaydet / Dosyadan yukle (ust duzey)
// ============================================================

template <typename K, typename V>
inline bool dosyaya_kaydet(const std::string& dosya_adi,
                            const Depo<K, V>& depo) {
    std::ofstream dosya(dosya_adi, std::ios::binary);
    if (!dosya.is_open()) return false;

    dosyaya_yaz(dosya, depo);
    return dosya.good();
}

template <typename K, typename V>
inline bool dosyadan_yukle(const std::string& dosya_adi,
                            Depo<K, V>& depo) {
    std::ifstream dosya(dosya_adi, std::ios::binary);
    if (!dosya.is_open()) return false;

    dosyadan_oku(dosya, depo);
    return dosya.good();
}

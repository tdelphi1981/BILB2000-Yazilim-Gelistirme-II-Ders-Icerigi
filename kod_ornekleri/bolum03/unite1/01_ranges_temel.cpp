/**
 * Ranges Temelleri - Geleneksel STL vs Ranges Karsilastirmasi
 *
 * Bu program, C++20 Ranges kutuphanesinin temel kullanimi ile
 * geleneksel STL algoritmalarini karsilastirir. Ogrenci Bilgi
 * Yonetim Sistemi (OBYS) baglaminda ornekler sunar.
 *
 * Ogrenilecek kavramlar:
 * - std::ranges::sort vs std::sort
 * - std::ranges::find_if vs std::find_if
 * - std::ranges::count_if
 * - Range kavramlari (concepts) ve static_assert kontrolleri
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 1 - Ranges ve Views (C++20)
 *
 * Derleme: g++ -std=c++20 -o 01_ranges_temel 01_ranges_temel.cpp
 * Calistirma: ./01_ranges_temel
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ranges>

struct Ogrenci {
    std::string isim;
    int numara;
    double ortalama;
};

// -- Range kavramlari ile derleme zamani kontrolleri --
static_assert(std::ranges::range<std::vector<int>>,
    "vector bir range olmalidir");
static_assert(std::ranges::sized_range<std::vector<int>>,
    "vector bir sized_range olmalidir");
static_assert(std::ranges::random_access_range<std::vector<int>>,
    "vector random_access_range olmalidir");

int main() {
    std::vector<Ogrenci> sinif = {
        {"Ayse Yilmaz",   1003, 85.5},
        {"Mehmet Demir",   1001, 72.0},
        {"Zeynep Kara",    1005, 91.3},
        {"Ali Ozturk",     1002, 68.4},
        {"Fatma Celik",    1004, 78.9}
    };

    // =============================================
    // 1) SIRALAMA: Geleneksel vs Ranges
    // =============================================
    std::cout << "=== Numaraya Gore Siralama ===\n";

    // Geleneksel yontem (iterator cifti + lambda):
    // std::sort(sinif.begin(), sinif.end(),
    //     [](const Ogrenci& a, const Ogrenci& b) {
    //         return a.numara < b.numara;
    //     });

    // Ranges yontemi (dogrudan container + lambda):
    std::ranges::sort(sinif, [](const Ogrenci& a, const Ogrenci& b) {
        return a.numara < b.numara;
    });

    for (const auto& ogr : sinif) {
        std::cout << "  " << ogr.numara << " - " << ogr.isim
                  << " (Ort: " << ogr.ortalama << ")\n";
    }

    // =============================================
    // 2) ARAMA: Geleneksel vs Ranges
    // =============================================
    std::cout << "\n=== Numara ile Ogrenci Arama ===\n";
    int aranan_numara = 1003;

    // Geleneksel yontem:
    // auto it = std::find_if(sinif.begin(), sinif.end(),
    //     [&](const Ogrenci& o) { return o.numara == aranan_numara; });

    // Ranges yontemi:
    auto it = std::ranges::find_if(sinif, [&](const Ogrenci& o) {
        return o.numara == aranan_numara;
    });

    if (it != sinif.end()) {
        std::cout << "  Bulundu: " << it->isim
                  << " (No: " << it->numara << ")\n";
    }

    // =============================================
    // 3) SAYMA: ranges::count_if
    // =============================================
    std::cout << "\n=== Basarili Ogrenci Sayisi (Ort >= 75) ===\n";

    auto basarili_sayi = std::ranges::count_if(sinif, [](const Ogrenci& o) {
        return o.ortalama >= 75.0;
    });

    std::cout << "  Basarili ogrenci sayisi: " << basarili_sayi
              << " / " << sinif.size() << "\n";

    // =============================================
    // 4) DIGER: ranges::min_element, ranges::max_element
    // =============================================
    std::cout << "\n=== En Yuksek ve En Dusuk Ortalama ===\n";

    auto en_yuksek = std::ranges::max_element(sinif,
        [](const Ogrenci& a, const Ogrenci& b) {
            return a.ortalama < b.ortalama;
        });

    auto en_dusuk = std::ranges::min_element(sinif,
        [](const Ogrenci& a, const Ogrenci& b) {
            return a.ortalama < b.ortalama;
        });

    std::cout << "  En yuksek: " << en_yuksek->isim
              << " (" << en_yuksek->ortalama << ")\n";
    std::cout << "  En dusuk:  " << en_dusuk->isim
              << " (" << en_dusuk->ortalama << ")\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Numaraya Gore Siralama ===
 *   1001 - Mehmet Demir (Ort: 72)
 *   1002 - Ali Ozturk (Ort: 68.4)
 *   1003 - Ayse Yilmaz (Ort: 85.5)
 *   1004 - Fatma Celik (Ort: 78.9)
 *   1005 - Zeynep Kara (Ort: 91.3)
 *
 * === Numara ile Ogrenci Arama ===
 *   Bulundu: Ayse Yilmaz (No: 1003)
 *
 * === Basarili Ogrenci Sayisi (Ort >= 75) ===
 *   Basarili ogrenci sayisi: 3 / 5
 *
 * === En Yuksek ve En Dusuk Ortalama ===
 *   En yuksek: Zeynep Kara (91.3)
 *   En dusuk:  Ali Ozturk (68.4)
 */

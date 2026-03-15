/**
 * OBYS Ranges Donusumu - Kapsamli Ogrenci Yonetim Sistemi
 *
 * Bu program, geleneksel C++ yaklasimi ile C++20 Ranges yaklasimini
 * yan yana karsilastirarak tam bir OBYS senaryosu uzerinde gosterir.
 * Sayfalama, coklu kriter siralama ve istatistiksel sorgular icerir.
 *
 * Ogrenilecek kavramlar:
 * - Eski yaklasim vs yeni yaklasim karsilastirmasi
 * - views::drop / views::take ile sayfalama
 * - Projeksiyonlarla coklu kriter siralama
 * - Ranges pipeline ile istatistiksel sorgular
 * - Kapsamli OBYS entegrasyonu
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 1 - Ranges ve Views (C++20)
 *
 * Derleme: g++ -std=c++20 -o 04_obys_ranges_donusum 04_obys_ranges_donusum.cpp
 * Calistirma: ./04_obys_ranges_donusum
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <iterator>
#include <iomanip>

struct Ogrenci {
    std::string isim;
    int numara;
    double ortalama;
    std::string bolum;
    int sinif_yili;   // 1, 2, 3 veya 4
};

// Sayfalama fonksiyonu: sayfa numarasi ve boyutuna gore goruntule
void sayfala(const std::vector<Ogrenci>& liste, int sayfa, int boyut) {
    int atlanan = (sayfa - 1) * boyut;
    std::cout << "  [Sayfa " << sayfa << "]\n";
    for (const auto& ogr : liste
             | std::views::drop(atlanan)
             | std::views::take(boyut)) {
        std::cout << "    " << ogr.numara << " - " << std::left
                  << std::setw(18) << ogr.isim
                  << " Ort: " << std::fixed << std::setprecision(1)
                  << ogr.ortalama << "\n";
    }
}

int main() {
    std::vector<Ogrenci> ogrenciler = {
        {"Ayse Yilmaz",    1001, 85.5, "Bilgisayar", 2},
        {"Mehmet Demir",    1002, 72.0, "Elektrik",   3},
        {"Zeynep Kara",     1003, 91.3, "Bilgisayar", 2},
        {"Ali Ozturk",      1004, 48.4, "Makine",     1},
        {"Fatma Celik",     1005, 78.9, "Bilgisayar", 3},
        {"Burak Sahin",     1006, 95.2, "Elektrik",   4},
        {"Elif Arslan",     1007, 96.1, "Bilgisayar", 4},
        {"Caner Yildiz",    1008, 37.8, "Bilgisayar", 1},
        {"Derya Aksoy",     1009, 82.3, "Elektrik",   2},
        {"Emre Tas",        1010, 58.6, "Makine",     1},
        {"Selin Aydin",     1011, 88.7, "Bilgisayar", 3},
        {"Kaan Polat",      1012, 41.2, "Elektrik",   2}
    };

    // =============================================================
    // 1) ESKI YAKLASIM vs YENI YAKLASIM: Basarili ogrenci listesi
    // =============================================================
    std::cout << "=== [ESKI] Basarili Bilgisayar Ogrencileri ===\n";
    {
        std::vector<Ogrenci> sonuc;
        for (const auto& o : ogrenciler) {
            if (o.bolum == "Bilgisayar" && o.ortalama >= 50.0) {
                sonuc.push_back(o);
            }
        }
        std::sort(sonuc.begin(), sonuc.end(),
            [](const Ogrenci& a, const Ogrenci& b) {
                return a.ortalama > b.ortalama;
            });
        for (const auto& o : sonuc) {
            std::cout << "  " << o.isim << " - " << o.ortalama << "\n";
        }
    }

    std::cout << "\n=== [YENI] Basarili Bilgisayar Ogrencileri ===\n";
    {
        // Filtreleme icin once bir kopya olustur, sonra sirala
        std::vector<Ogrenci> sonuc;
        auto filtre = ogrenciler
            | std::views::filter([](const Ogrenci& o) {
                return o.bolum == "Bilgisayar" && o.ortalama >= 50.0;
              });
        std::ranges::copy(filtre, std::back_inserter(sonuc));
        std::ranges::sort(sonuc, std::ranges::greater{},
                          &Ogrenci::ortalama);

        for (const auto& o : sonuc) {
            std::cout << "  " << o.isim << " - " << o.ortalama << "\n";
        }
    }

    // =============================================================
    // 2) SAYFALAMA: drop + take ile
    // =============================================================
    std::cout << "\n=== Sayfalama Ornegi (sayfa boyutu: 4) ===\n";
    std::ranges::sort(ogrenciler, {}, &Ogrenci::numara);

    sayfala(ogrenciler, 1, 4);
    sayfala(ogrenciler, 2, 4);
    sayfala(ogrenciler, 3, 4);

    // =============================================================
    // 3) ISTATISTIKSEL SORGULAR
    // =============================================================
    std::cout << "\n=== Bolum Bazinda Istatistikler ===\n";

    for (const auto& bolum : {"Bilgisayar", "Elektrik", "Makine"}) {
        // Bolum ogrencilerini filtrele
        std::vector<double> notlar;
        auto bolum_notlari = ogrenciler
            | std::views::filter([&](const Ogrenci& o) {
                return o.bolum == bolum;
              })
            | std::views::transform(&Ogrenci::ortalama);

        std::ranges::copy(bolum_notlari, std::back_inserter(notlar));

        if (notlar.empty()) continue;

        double toplam = std::accumulate(notlar.begin(), notlar.end(), 0.0);
        double ort = toplam / static_cast<double>(notlar.size());
        auto [min_it, max_it] = std::ranges::minmax_element(notlar);

        std::cout << "  " << std::left << std::setw(12) << bolum
                  << " | Kisi: " << notlar.size()
                  << " | Ort: " << std::fixed << std::setprecision(1) << ort
                  << " | Min: " << *min_it
                  << " | Max: " << *max_it << "\n";
    }

    // =============================================================
    // 4) COKLU KRITER SIRALAMA
    // =============================================================
    std::cout << "\n=== Bolum + Ortalama Siralamasi ===\n";

    std::ranges::sort(ogrenciler, [](const Ogrenci& a, const Ogrenci& b) {
        if (a.bolum != b.bolum) return a.bolum < b.bolum;
        return a.ortalama > b.ortalama;  // Ayni bolumde yuksekten dusuge
    });

    for (const auto& o : ogrenciler) {
        std::cout << "  " << std::left << std::setw(12) << o.bolum
                  << std::setw(18) << o.isim
                  << "Ort: " << std::fixed << std::setprecision(1)
                  << o.ortalama << "\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === [ESKI] Basarili Bilgisayar Ogrencileri ===
 *   Elif Arslan - 96.1
 *   Zeynep Kara - 91.3
 *   Selin Aydin - 88.7
 *   Ayse Yilmaz - 85.5
 *   Fatma Celik - 78.9
 *
 * === [YENI] Basarili Bilgisayar Ogrencileri ===
 *   Elif Arslan - 96.1
 *   Zeynep Kara - 91.3
 *   Selin Aydin - 88.7
 *   Ayse Yilmaz - 85.5
 *   Fatma Celik - 78.9
 *
 * === Sayfalama Ornegi (sayfa boyutu: 4) ===
 *   [Sayfa 1]
 *     1001 - Ayse Yilmaz      Ort: 85.5
 *     1002 - Mehmet Demir      Ort: 72.0
 *     1003 - Zeynep Kara       Ort: 91.3
 *     1004 - Ali Ozturk        Ort: 48.4
 *   [Sayfa 2]
 *     1005 - Fatma Celik       Ort: 78.9
 *     1006 - Burak Sahin       Ort: 95.2
 *     1007 - Elif Arslan       Ort: 96.1
 *     1008 - Caner Yildiz      Ort: 37.8
 *   [Sayfa 3]
 *     1009 - Derya Aksoy       Ort: 82.3
 *     1010 - Emre Tas          Ort: 58.6
 *     1011 - Selin Aydin       Ort: 88.7
 *     1012 - Kaan Polat        Ort: 41.2
 *
 * === Bolum Bazinda Istatistikler ===
 *   Bilgisayar   | Kisi: 6 | Ort: 79.7 | Min: 37.8 | Max: 96.1
 *   Elektrik     | Kisi: 4 | Ort: 72.7 | Min: 41.2 | Max: 95.2
 *   Makine       | Kisi: 2 | Ort: 53.5 | Min: 48.4 | Max: 58.6
 *
 * === Bolum + Ortalama Siralamasi ===
 *   Bilgisayar  Elif Arslan       Ort: 96.1
 *   Bilgisayar  Zeynep Kara       Ort: 91.3
 *   Bilgisayar  Selin Aydin       Ort: 88.7
 *   Bilgisayar  Ayse Yilmaz       Ort: 85.5
 *   Bilgisayar  Fatma Celik       Ort: 78.9
 *   Bilgisayar  Caner Yildiz      Ort: 37.8
 *   Elektrik    Burak Sahin       Ort: 95.2
 *   Elektrik    Derya Aksoy       Ort: 82.3
 *   Elektrik    Mehmet Demir      Ort: 72.0
 *   Elektrik    Kaan Polat        Ort: 41.2
 *   Makine      Emre Tas          Ort: 58.6
 *   Makine      Ali Ozturk        Ort: 48.4
 */

/**
 * OBYS Istatistik Hesaplama - Sayisal ve Kume Algoritmalari
 *
 * Bu program OBYS baglaminda accumulate, nth_element ve
 * set islemlerini bir arada kullanir.
 *
 * Ogrenilecek kavramlar:
 * - Sinif istatistiklerini tek geciste hesaplama
 * - Medyan ve yuzdelik dilimleri verimli bulma
 * - Ders kesisimlerini kume islemleriyle hesaplama
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 3 - Ileri STL Algoritmalari
 *
 * Derleme: g++ -std=c++20 -o 04_obys_istatistik 04_obys_istatistik.cpp
 * Calistirma: ./04_obys_istatistik
 */

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <iomanip>
#include <cmath>
#include <memory>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
    std::set<std::string> dersler;
};

// Istatistik raporu yapisi
struct Istatistik {
    double ortalama;
    double medyan;
    double std_sapma;
    double min_gpa;
    double max_gpa;
    int toplam;
    int basarili;    // >= 2.0
    int onur;        // >= 3.5
};

Istatistik hesapla(std::vector<double> gpalar) {
    Istatistik ist{};
    ist.toplam = static_cast<int>(gpalar.size());

    if (gpalar.empty()) return ist;

    // Ortalama
    double toplam = std::accumulate(gpalar.begin(), gpalar.end(), 0.0);
    ist.ortalama = toplam / gpalar.size();

    // Min/Max
    auto [min_it, max_it] = std::minmax_element(gpalar.begin(), gpalar.end());
    ist.min_gpa = *min_it;
    ist.max_gpa = *max_it;

    // Medyan (nth_element ile O(n))
    auto medyan_pos = gpalar.begin() + gpalar.size() / 2;
    std::nth_element(gpalar.begin(), medyan_pos, gpalar.end());
    ist.medyan = *medyan_pos;

    // Standart sapma
    double varyans = std::transform_reduce(
        gpalar.begin(), gpalar.end(), 0.0,
        std::plus<>(),
        [ort = ist.ortalama](double g) { return (g - ort) * (g - ort); }
    ) / gpalar.size();
    ist.std_sapma = std::sqrt(varyans);

    // Sayimlar
    ist.basarili = std::count_if(gpalar.begin(), gpalar.end(),
        [](double g) { return g >= 2.0; });
    ist.onur = std::count_if(gpalar.begin(), gpalar.end(),
        [](double g) { return g >= 3.5; });

    return ist;
}

int main() {
    std::cout << "=== OBYS Istatistik Raporu ===\n\n";

    // Ogrenci verileri
    std::vector<Ogrenci> sinif = {
        {1001, "Ali Yilmaz",   3.45, {"C++", "Fizik", "Matematik"}},
        {1002, "Ayse Kaya",    3.80, {"C++", "Kimya", "Matematik", "Istatistik"}},
        {1003, "Mehmet Demir", 2.90, {"C++", "Fizik", "Elektronik"}},
        {1004, "Zeynep Can",   3.65, {"C++", "Matematik", "Veritabani"}},
        {1005, "Can Arslan",   2.10, {"Fizik", "Kimya"}},
        {1006, "Elif Yildiz",  3.92, {"C++", "Matematik", "Yapay Zeka"}},
        {1007, "Burak Ozturk", 2.75, {"Fizik", "Elektronik"}},
        {1008, "Selin Aktas",  3.50, {"C++", "Istatistik", "Veritabani"}},
        {1009, "Emre Celik",   1.95, {"Fizik"}},
        {1010, "Deniz Sahin",  3.30, {"C++", "Matematik", "Kimya"}}
    };

    // GPA listesini cikar (transform)
    std::vector<double> gpalar;
    gpalar.reserve(sinif.size());
    std::transform(sinif.begin(), sinif.end(),
                   std::back_inserter(gpalar),
                   [](const Ogrenci& o) { return o.gpa; });

    // Istatistikleri hesapla
    auto ist = hesapla(gpalar);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "--- Sinif Istatistikleri ---\n";
    std::cout << "  Toplam ogrenci:    " << ist.toplam << "\n";
    std::cout << "  Ortalama GPA:      " << ist.ortalama << "\n";
    std::cout << "  Medyan GPA:        " << ist.medyan << "\n";
    std::cout << "  Standart sapma:    " << ist.std_sapma << "\n";
    std::cout << "  En yuksek GPA:     " << ist.max_gpa << "\n";
    std::cout << "  En dusuk GPA:      " << ist.min_gpa << "\n";
    std::cout << "  Basarili (>= 2.0): " << ist.basarili << " ("
              << 100.0 * ist.basarili / ist.toplam << "%)\n";
    std::cout << "  Onur (>= 3.5):     " << ist.onur << " ("
              << 100.0 * ist.onur / ist.toplam << "%)\n";

    // Ders analizi: En populer dersler
    std::cout << "\n--- Ders Analizi ---\n";
    std::map<std::string, int> ders_sayilari;
    for (const auto& o : sinif) {
        for (const auto& d : o.dersler) {
            ders_sayilari[d]++;
        }
    }

    // Ders-sayisini vector'e kopyala ve sirala
    std::vector<std::pair<std::string, int>> ders_vec(
        ders_sayilari.begin(), ders_sayilari.end());
    std::sort(ders_vec.begin(), ders_vec.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    std::cout << "Ders populerligi:\n";
    for (const auto& [ders, sayi] : ders_vec) {
        std::cout << "  " << std::left << std::setw(15) << ders
                  << ": " << sayi << " ogrenci\n";
    }

    // Ders kesisimleri: en cok ortak ders alan ikili
    std::cout << "\n--- En Cok Ortak Dersi Olan Ikililer ---\n";
    int max_ortak = 0;
    std::string ikili_isim;

    for (size_t i = 0; i < sinif.size(); ++i) {
        for (size_t j = i + 1; j < sinif.size(); ++j) {
            std::set<std::string> ortak;
            std::set_intersection(
                sinif[i].dersler.begin(), sinif[i].dersler.end(),
                sinif[j].dersler.begin(), sinif[j].dersler.end(),
                std::inserter(ortak, ortak.begin()));

            if (static_cast<int>(ortak.size()) > max_ortak) {
                max_ortak = static_cast<int>(ortak.size());
                ikili_isim = sinif[i].isim + " & " + sinif[j].isim;
            }
        }
    }
    std::cout << "  " << ikili_isim << " (" << max_ortak << " ortak ders)\n";

    // Herkese ortak ders (tum kesisin)
    std::cout << "\n--- Herkesin Aldigi Dersler ---\n";
    std::set<std::string> herkesin = sinif[0].dersler;
    for (size_t i = 1; i < sinif.size(); ++i) {
        std::set<std::string> gecici;
        std::set_intersection(
            herkesin.begin(), herkesin.end(),
            sinif[i].dersler.begin(), sinif[i].dersler.end(),
            std::inserter(gecici, gecici.begin()));
        herkesin = std::move(gecici);
    }

    if (herkesin.empty()) {
        std::cout << "  Herkesin ortak aldigi ders yok.\n";
    } else {
        for (const auto& d : herkesin) {
            std::cout << "  " << d << "\n";
        }
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === OBYS Istatistik Raporu ===
 *
 * --- Sinif Istatistikleri ---
 *   Toplam ogrenci:    10
 *   Ortalama GPA:      3.13
 *   Medyan GPA:        3.30
 *   Standart sapma:    0.60
 *   En yuksek GPA:     3.92
 *   En dusuk GPA:      1.95
 *   Basarili (>= 2.0): 9 (90.00%)
 *   Onur (>= 3.5):     4 (40.00%)
 *
 * --- Ders Analizi ---
 * ... (populerlik sirasina gore)
 *
 * --- En Cok Ortak Dersi Olan Ikililer ---
 * ... (en fazla ortak dersi olan ikili)
 *
 * --- Herkesin Aldigi Dersler ---
 *   Herkesin ortak aldigi ders yok.
 */

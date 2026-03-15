/**
 * OBYS Siralama ve Filtreleme - Tam Senaryo
 *
 * Bu program OBYS baglaminda siralama, arama ve filtreleme
 * algoritmalarinin birlikte kullanimini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - sort + find + copy_if + transform birlesmesi
 * - Ogrenci verisi uzerinde coklu islem zincirleme
 * - Performans karsilastirmasi
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 2 - Temel STL Algoritmalari
 *
 * Derleme: g++ -std=c++20 -o 04_obys_siralama_filtreleme 04_obys_siralama_filtreleme.cpp
 * Calistirma: ./04_obys_siralama_filtreleme
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <numeric>
#include <iterator>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
    std::string bolum;
};

std::ostream& operator<<(std::ostream& os, const Ogrenci& o) {
    os << std::setw(5) << o.numara << " | "
       << std::left << std::setw(18) << o.isim
       << " | GPA: " << std::fixed << std::setprecision(2) << o.gpa
       << " | " << o.bolum;
    return os;
}

int main() {
    std::cout << "=== OBYS: Siralama ve Filtreleme ===\n\n";

    std::vector<Ogrenci> sinif = {
        {1001, "Ali Yilmaz",     3.45, "Bilgisayar"},
        {1002, "Ayse Kaya",      3.80, "Bilgisayar"},
        {1003, "Mehmet Demir",   2.90, "Elektrik"},
        {1004, "Zeynep Can",     3.65, "Bilgisayar"},
        {1005, "Can Arslan",     2.10, "Makine"},
        {1006, "Elif Yildiz",    3.92, "Bilgisayar"},
        {1007, "Burak Ozturk",   2.75, "Elektrik"},
        {1008, "Selin Aktas",    3.50, "Bilgisayar"},
        {1009, "Emre Celik",     1.95, "Makine"},
        {1010, "Deniz Sahin",    3.30, "Elektrik"}
    };

    // 1. GPA'ya gore azalan siralama
    std::sort(sinif.begin(), sinif.end(),
        [](const Ogrenci& a, const Ogrenci& b) {
            return a.gpa > b.gpa;
        });

    std::cout << "--- Basari Siralamasi ---\n";
    int sira = 1;
    for (const auto& o : sinif) {
        std::cout << "  " << std::setw(2) << sira++ << ". " << o << "\n";
    }

    // 2. Bolume gore filtreleme
    std::cout << "\n--- Bilgisayar Bolumu Ogrencileri ---\n";
    std::vector<Ogrenci> bilgisayar;
    std::copy_if(sinif.begin(), sinif.end(),
                 std::back_inserter(bilgisayar),
                 [](const Ogrenci& o) { return o.bolum == "Bilgisayar"; });

    for (const auto& o : bilgisayar) {
        std::cout << "  " << o << "\n";
    }

    // 3. GPA istatistikleri
    std::cout << "\n--- GPA Istatistikleri ---\n";
    double toplam = std::accumulate(sinif.begin(), sinif.end(), 0.0,
        [](double sum, const Ogrenci& o) { return sum + o.gpa; });
    double ortalama = toplam / static_cast<double>(sinif.size());

    // En yuksek ve en dusuk
    auto [min_it, max_it] = std::minmax_element(sinif.begin(), sinif.end(),
        [](const Ogrenci& a, const Ogrenci& b) { return a.gpa < b.gpa; });

    std::cout << "  Sinif ortalamasi: " << std::fixed << std::setprecision(2)
              << ortalama << "\n";
    std::cout << "  En yuksek GPA: " << max_it->isim << " (" << max_it->gpa << ")\n";
    std::cout << "  En dusuk GPA:  " << min_it->isim << " (" << min_it->gpa << ")\n";

    // 4. Basarili / Basarisiz sayilari
    int basarili = std::count_if(sinif.begin(), sinif.end(),
        [](const Ogrenci& o) { return o.gpa >= 2.0; });

    std::cout << "\n--- Durum Ozeti ---\n";
    std::cout << "  Toplam ogrenci: " << sinif.size() << "\n";
    std::cout << "  Basarili (GPA >= 2.0): " << basarili << "\n";
    std::cout << "  Basarisiz (GPA < 2.0): "
              << static_cast<int>(sinif.size()) - basarili << "\n";

    // 5. Onur listesi (GPA >= 3.5)
    std::cout << "\n--- Onur Listesi (GPA >= 3.5) ---\n";
    std::vector<std::string> onur;
    std::for_each(sinif.begin(), sinif.end(),
        [&onur](const Ogrenci& o) {
            if (o.gpa >= 3.5) {
                onur.push_back(o.isim + " (" +
                    std::to_string(o.gpa).substr(0, 4) + ")");
            }
        });

    std::sort(onur.begin(), onur.end()); // Alfabetik sirala
    for (const auto& s : onur) {
        std::cout << "  " << s << "\n";
    }

    // 6. Isme gore arama
    std::cout << "\n--- Ogrenci Arama ---\n";
    std::string aranan = "Selin Aktas";
    auto bulunan = std::find_if(sinif.begin(), sinif.end(),
        [&aranan](const Ogrenci& o) { return o.isim == aranan; });

    if (bulunan != sinif.end()) {
        std::cout << "  Bulundu: " << *bulunan << "\n";
    } else {
        std::cout << "  \"" << aranan << "\" bulunamadi.\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === OBYS: Siralama ve Filtreleme ===
 *
 * --- Basari Siralamasi ---
 *    1. 1006: Elif Yildiz       | GPA: 3.92 | Bilgisayar
 *    2. 1002: Ayse Kaya         | GPA: 3.80 | Bilgisayar
 *    3. 1004: Zeynep Can        | GPA: 3.65 | Bilgisayar
 *    4. 1008: Selin Aktas       | GPA: 3.50 | Bilgisayar
 *    5. 1001: Ali Yilmaz        | GPA: 3.45 | Bilgisayar
 *    6. 1010: Deniz Sahin       | GPA: 3.30 | Elektrik
 *    7. 1003: Mehmet Demir      | GPA: 2.90 | Elektrik
 *    8. 1007: Burak Ozturk      | GPA: 2.75 | Elektrik
 *    9. 1005: Can Arslan        | GPA: 2.10 | Makine
 *   10. 1009: Emre Celik        | GPA: 1.95 | Makine
 *
 * ... (devam eder)
 */

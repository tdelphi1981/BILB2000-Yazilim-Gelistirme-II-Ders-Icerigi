/**
 * STL Dort Bilesen - Tam Ornek
 *
 * Bu program STL'in dort ana bilesenini (kapsayici, iterator,
 * algoritma, fonksiyon nesnesi) bir arada gosterir.
 * OBYS benzeri bir senaryo uzerinden STL kullanimi tanitilir.
 *
 * Ogrenilecek kavramlar:
 * - vector, map kapsayicilari
 * - Iterator ile dolasma
 * - sort, find_if, for_each algoritmalari
 * - Lambda ifadeleri
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 1 - STL'e Genel Bakis
 *
 * Derleme: g++ -std=c++20 -o 03_stl_dort_bilesen 03_stl_dort_bilesen.cpp
 * Calistirma: ./03_stl_dort_bilesen
 */

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <numeric>   // accumulate icin

// Basit ogrenci yapisi
struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

int main() {
    std::cout << "=== STL Dort Bilesen: OBYS Ornegi ===\n\n";

    // --- BILESEN 1: KAPSAYICI (vector) ---
    std::vector<Ogrenci> sinif = {
        {1001, "Ali Yilmaz",    3.2},
        {1002, "Zeynep Kaya",   3.8},
        {1003, "Mehmet Demir",  2.9},
        {1004, "Ayse Ozturk",   3.5},
        {1005, "Can Arslan",    2.7}
    };

    std::cout << "--- 1. Kapsayici: Ogrenci Listesi ---\n";
    for (const auto& ogr : sinif) {
        std::cout << "  " << ogr.numara << " - "
                  << ogr.isim << " (GPA: " << ogr.gpa << ")\n";
    }
    std::cout << "\n";

    // --- BILESEN 2: ITERATOR ---
    std::cout << "--- 2. Iterator: Ilk ve Son Eleman ---\n";
    auto ilk = sinif.begin();  // Ilk elemani gosteren iterator
    auto son = sinif.end() - 1; // Son elemani gosteren iterator
    std::cout << "  Ilk: " << ilk->isim << "\n";
    std::cout << "  Son: " << son->isim << "\n\n";

    // --- BILESEN 3: ALGORITMA (sort) ---
    // GPA'ya gore buyukten kucuge sirala
    std::sort(sinif.begin(), sinif.end(),
        [](const Ogrenci& a, const Ogrenci& b) {
            return a.gpa > b.gpa;  // Buyukten kucuge
        });

    std::cout << "--- 3. Algoritma: GPA Siralamasina Gore ---\n";
    int sira = 1;
    for (const auto& ogr : sinif) {
        std::cout << "  " << sira++ << ". " << ogr.isim
                  << " (GPA: " << ogr.gpa << ")\n";
    }
    std::cout << "\n";

    // --- BILESEN 4: FONKSIYON NESNESI (lambda) ---
    // GPA >= 3.0 olanlari bul
    std::cout << "--- 4. Lambda: Basarili Ogrenciler (GPA >= 3.0) ---\n";

    auto basarili_mi = [](const Ogrenci& ogr) {
        return ogr.gpa >= 3.0;
    };

    int basarili_sayisi = std::count_if(
        sinif.begin(), sinif.end(), basarili_mi);

    std::cout << "  Basarili ogrenci sayisi: " << basarili_sayisi << "\n";

    // Basarililari listele
    std::for_each(sinif.begin(), sinif.end(),
        [&basarili_mi](const Ogrenci& ogr) {
            if (basarili_mi(ogr)) {
                std::cout << "  [+] " << ogr.isim
                          << " - GPA: " << ogr.gpa << "\n";
            }
        });
    std::cout << "\n";

    // --- BONUS: map kapsayicisi ---
    std::cout << "--- Bonus: map ile Anahtar-Deger Esleme ---\n";
    std::map<int, std::string> numara_isim;
    for (const auto& ogr : sinif) {
        numara_isim[ogr.numara] = ogr.isim;
    }

    // Numaraya gore hizli erisim
    int aranan_no = 1003;
    auto it = numara_isim.find(aranan_no);
    if (it != numara_isim.end()) {
        std::cout << "  Numara " << aranan_no << " -> "
                  << it->second << "\n";
    }

    // GPA ortalamasi hesapla
    double toplam = std::accumulate(sinif.begin(), sinif.end(), 0.0,
        [](double sum, const Ogrenci& ogr) {
            return sum + ogr.gpa;
        });
    double ortalama = toplam / sinif.size();
    std::cout << "\n  Sinif GPA ortalamasi: " << ortalama << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === STL Dort Bilesen: OBYS Ornegi ===
 *
 * --- 1. Kapsayici: Ogrenci Listesi ---
 *   1001 - Ali Yilmaz (GPA: 3.2)
 *   1002 - Zeynep Kaya (GPA: 3.8)
 *   1003 - Mehmet Demir (GPA: 2.9)
 *   1004 - Ayse Ozturk (GPA: 3.5)
 *   1005 - Can Arslan (GPA: 2.7)
 *
 * --- 2. Iterator: Ilk ve Son Eleman ---
 *   Ilk: Ali Yilmaz
 *   Son: Can Arslan
 *
 * --- 3. Algoritma: GPA Siralamasina Gore ---
 *   1. Zeynep Kaya (GPA: 3.8)
 *   2. Ayse Ozturk (GPA: 3.5)
 *   3. Ali Yilmaz (GPA: 3.2)
 *   4. Mehmet Demir (GPA: 2.9)
 *   5. Can Arslan (GPA: 2.7)
 *
 * --- 4. Lambda: Basarili Ogrenciler (GPA >= 3.0) ---
 *   Basarili ogrenci sayisi: 3
 *   [+] Zeynep Kaya - GPA: 3.8
 *   [+] Ayse Ozturk - GPA: 3.5
 *   [+] Ali Yilmaz - GPA: 3.2
 *
 * --- Bonus: map ile Anahtar-Deger Esleme ---
 *   Numara 1003 -> Mehmet Demir
 *
 *   Sinif GPA ortalamasi: 3.22
 */

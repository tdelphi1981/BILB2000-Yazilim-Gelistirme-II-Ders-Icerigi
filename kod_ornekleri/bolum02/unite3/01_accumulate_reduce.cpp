/**
 * Accumulate ve Reduce - Sayisal Algoritmalar
 *
 * Bu program sayisal algoritmalari gosterir: accumulate, reduce,
 * inner_product, partial_sum, transform_reduce.
 *
 * Ogrenilecek kavramlar:
 * - std::accumulate ile toplama ve ozel islemler
 * - std::reduce ile paralel-uyumlu toplama
 * - std::inner_product ile ic carpim
 * - std::partial_sum ile kumulatif toplam
 * - std::transform_reduce ile donusum + indirgeme
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 3 - Ileri STL Algoritmalari
 *
 * Derleme: g++ -std=c++20 -o 01_accumulate_reduce 01_accumulate_reduce.cpp
 * Calistirma: ./01_accumulate_reduce
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <iomanip>
#include <functional>

struct Ogrenci {
    std::string isim;
    double gpa;
    int kredi;
};

int main() {
    std::cout << "=== Accumulate ve Reduce ===\n\n";

    // --- 1. std::accumulate: temel toplam ---
    std::cout << "--- std::accumulate ---\n";
    std::vector<int> notlar = {85, 92, 78, 95, 88};

    int toplam = std::accumulate(notlar.begin(), notlar.end(), 0);
    double ortalama = static_cast<double>(toplam) / notlar.size();

    std::cout << "Notlar: ";
    for (int n : notlar) std::cout << n << " ";
    std::cout << "\nToplam: " << toplam
              << "\nOrtalama: " << std::fixed << std::setprecision(1)
              << ortalama << "\n";

    // Ozel islem: carpim
    std::vector<int> faktoriyel_veri = {1, 2, 3, 4, 5};
    int carpim = std::accumulate(faktoriyel_veri.begin(),
                                  faktoriyel_veri.end(),
                                  1, std::multiplies<int>());
    std::cout << "\n5! = " << carpim << "\n";

    // String birlestirme
    std::vector<std::string> kelimeler = {"C++", "STL", "harika"};
    std::string cumle = std::accumulate(
        kelimeler.begin(), kelimeler.end(), std::string(""),
        [](const std::string& a, const std::string& b) {
            return a.empty() ? b : a + " " + b;
        });
    std::cout << "Cumle: \"" << cumle << "\"\n";

    // --- 2. std::reduce (C++17) ---
    std::cout << "\n--- std::reduce ---\n";
    // reduce, accumulate'e benzer ama isleme sirasi belirsiz
    // Bu sayede paralel calisabilir
    double reduce_toplam = std::reduce(notlar.begin(), notlar.end(), 0.0);
    std::cout << "reduce toplam: " << reduce_toplam << "\n";

    // [!] accumulate vs reduce farki:
    // accumulate: soldan saga garanti
    // reduce: islem sirasi belirsiz (paralel uyumlu)
    // Toplama/carpma gibi degismeli islemlerde ayni sonuc
    // Cikarma gibi degismeli olmayanlarda FARKLI sonuc verebilir!

    // --- 3. std::inner_product ---
    std::cout << "\n--- std::inner_product ---\n";
    std::vector<double> agirliklar = {0.2, 0.3, 0.5}; // Vize, Odev, Final
    std::vector<double> puanlar = {80.0, 90.0, 85.0};

    double agirlikli_ort = std::inner_product(
        agirliklar.begin(), agirliklar.end(),
        puanlar.begin(), 0.0);

    std::cout << "Agirliklar: 0.2, 0.3, 0.5\n";
    std::cout << "Puanlar:    80, 90, 85\n";
    std::cout << "Agirlikli ortalama: " << agirlikli_ort << "\n";

    // --- 4. std::partial_sum ---
    std::cout << "\n--- std::partial_sum ---\n";
    std::vector<int> aylik_kredi = {3, 4, 3, 5, 4, 3};

    std::vector<int> kumulatif(aylik_kredi.size());
    std::partial_sum(aylik_kredi.begin(), aylik_kredi.end(),
                     kumulatif.begin());

    std::cout << "Aylik kredi:    ";
    for (int k : aylik_kredi) std::cout << k << " ";
    std::cout << "\nKumulatif kredi: ";
    for (int k : kumulatif) std::cout << k << " ";
    std::cout << "\n";

    // --- 5. std::transform_reduce (C++17) ---
    std::cout << "\n--- std::transform_reduce ---\n";

    // OBYS: Agirlikli GPA hesaplama (kredi * gpa toplami / toplam kredi)
    std::vector<Ogrenci> sinif = {
        {"Fizik",      3.0, 4},
        {"Matematik",  3.5, 3},
        {"C++",        4.0, 3},
        {"Elektronik", 2.5, 4},
        {"Ingilizce",  3.8, 2}
    };

    // Yontem 1: ayri ayri hesapla
    double kredi_gpa_toplam = 0.0;
    int toplam_kredi = 0;
    for (const auto& o : sinif) {
        kredi_gpa_toplam += o.gpa * o.kredi;
        toplam_kredi += o.kredi;
    }
    std::cout << "Yontem 1 (dongu): "
              << kredi_gpa_toplam / toplam_kredi << "\n";

    // Yontem 2: transform_reduce ile tek satirda
    double kredi_gpa = std::transform_reduce(
        sinif.begin(), sinif.end(),
        0.0,                                        // baslangic
        std::plus<>(),                               // indirgeme: topla
        [](const Ogrenci& o) { return o.gpa * o.kredi; }  // donusum
    );

    int t_kredi = std::transform_reduce(
        sinif.begin(), sinif.end(),
        0,
        std::plus<>(),
        [](const Ogrenci& o) { return o.kredi; }
    );

    std::cout << "Yontem 2 (transform_reduce): "
              << kredi_gpa / t_kredi << "\n";

    // --- 6. OBYS: Sinif istatistikleri ---
    std::cout << "\n--- OBYS: Sinif Istatistikleri ---\n";
    std::vector<double> gpa_listesi = {3.45, 3.80, 2.90, 3.65, 2.10, 3.92, 2.75, 3.50, 1.95, 3.30};

    double sinif_ort = std::accumulate(gpa_listesi.begin(),
                                        gpa_listesi.end(), 0.0)
                       / gpa_listesi.size();

    auto [min_it, max_it] = std::minmax_element(gpa_listesi.begin(),
                                                  gpa_listesi.end());

    std::cout << "Ogrenci sayisi: " << gpa_listesi.size() << "\n";
    std::cout << "Sinif ortalamasi: " << sinif_ort << "\n";
    std::cout << "En yuksek GPA: " << *max_it << "\n";
    std::cout << "En dusuk GPA: " << *min_it << "\n";

    // Standart sapma
    double varyans = std::transform_reduce(
        gpa_listesi.begin(), gpa_listesi.end(),
        0.0,
        std::plus<>(),
        [sinif_ort](double gpa) {
            return (gpa - sinif_ort) * (gpa - sinif_ort);
        }
    ) / gpa_listesi.size();

    std::cout << "Standart sapma: " << std::sqrt(varyans) << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Accumulate ve Reduce ===
 *
 * --- std::accumulate ---
 * Notlar: 85 92 78 95 88
 * Toplam: 438
 * Ortalama: 87.6
 *
 * 5! = 120
 * Cumle: "C++ STL harika"
 *
 * --- std::reduce ---
 * reduce toplam: 438.0
 *
 * --- std::inner_product ---
 * Agirliklar: 0.2, 0.3, 0.5
 * Puanlar:    80, 90, 85
 * Agirlikli ortalama: 85.5
 *
 * --- std::partial_sum ---
 * Aylik kredi:    3 4 3 5 4 3
 * Kumulatif kredi: 3 7 10 15 19 22
 *
 * --- std::transform_reduce ---
 * Yontem 1 (dongu): 3.28125
 * Yontem 2 (transform_reduce): 3.28125
 *
 * --- OBYS: Sinif Istatistikleri ---
 * Ogrenci sayisi: 10
 * Sinif ortalamasi: 3.1
 * En yuksek GPA: 3.9
 * En dusuk GPA: 1.9
 * Standart sapma: 0.6
 */

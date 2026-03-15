/**
 * Transform ve Copy - Donusturme ve Kopyalama Algoritmalari
 *
 * Bu program transform, copy, fill, generate ve for_each kullanimini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - std::transform (tekli ve ikili form)
 * - std::copy, std::copy_if
 * - std::fill, std::fill_n
 * - std::generate, std::generate_n
 * - std::for_each
 * - std::replace, std::replace_if
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 2 - Temel STL Algoritmalari
 *
 * Derleme: g++ -std=c++20 -o 02_transform_copy 02_transform_copy.cpp
 * Calistirma: ./02_transform_copy
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <iomanip>
#include <cctype>
#include <random>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

int main() {
    std::cout << "=== Transform ve Copy ===\n\n";

    // --- 1. std::transform (tekli form) ---
    std::cout << "--- std::transform (tekli) ---\n";
    std::vector<int> notlar = {85, 92, 78, 95, 88};

    // Her notu 5 puan arttir (egri uygula)
    std::vector<int> egrili_notlar(notlar.size());
    std::transform(notlar.begin(), notlar.end(),
                   egrili_notlar.begin(),
                   [](int n) { return std::min(n + 5, 100); });

    std::cout << "Orijinal: ";
    for (int n : notlar) std::cout << n << " ";
    std::cout << "\nEgrili:   ";
    for (int n : egrili_notlar) std::cout << n << " ";
    std::cout << "\n";

    // String donusumu: isimleri buyuk harfe cevir
    std::vector<std::string> isimler = {"ali", "ayse", "mehmet", "zeynep"};
    std::vector<std::string> buyuk_isimler(isimler.size());

    std::transform(isimler.begin(), isimler.end(),
                   buyuk_isimler.begin(),
                   [](std::string s) {
                       std::transform(s.begin(), s.end(), s.begin(),
                           [](unsigned char c) {
                               return std::toupper(c);
                           });
                       return s;
                   });

    std::cout << "\nBuyuk harfler: ";
    for (const auto& s : buyuk_isimler) std::cout << s << " ";
    std::cout << "\n";

    // --- 2. std::transform (ikili form) ---
    std::cout << "\n--- std::transform (ikili) ---\n";
    std::vector<int> vize = {80, 70, 90, 85, 60};
    std::vector<int> final_notu = {90, 80, 85, 95, 70};
    std::vector<double> ortalama(vize.size());

    // Vize %40 + Final %60
    std::transform(vize.begin(), vize.end(),
                   final_notu.begin(),
                   ortalama.begin(),
                   [](int v, int f) { return v * 0.4 + f * 0.6; });

    std::cout << "Vize:     ";
    for (int n : vize) std::cout << std::setw(4) << n << " ";
    std::cout << "\nFinal:    ";
    for (int n : final_notu) std::cout << std::setw(4) << n << " ";
    std::cout << "\nOrtalama: ";
    for (double n : ortalama)
        std::cout << std::fixed << std::setprecision(0) << std::setw(4) << n << " ";
    std::cout << "\n";

    // --- 3. std::copy / std::copy_if ---
    std::cout << "\n--- std::copy_if ---\n";
    std::vector<Ogrenci> sinif = {
        {1001, "Ali Yilmaz",   3.45},
        {1002, "Ayse Kaya",    3.80},
        {1003, "Mehmet Demir", 2.90},
        {1004, "Zeynep Can",   3.65},
        {1005, "Can Arslan",   2.10}
    };

    // Onur listesi: GPA >= 3.5
    std::vector<Ogrenci> onur_listesi;
    std::copy_if(sinif.begin(), sinif.end(),
                 std::back_inserter(onur_listesi),
                 [](const Ogrenci& o) { return o.gpa >= 3.5; });

    std::cout << "Onur listesi:\n";
    for (const auto& o : onur_listesi) {
        std::cout << "  " << o.isim << " (GPA: "
                  << std::fixed << std::setprecision(2) << o.gpa << ")\n";
    }

    // --- 4. std::fill / std::fill_n ---
    std::cout << "\n--- std::fill ---\n";
    std::vector<int> puanlar(10);
    std::fill(puanlar.begin(), puanlar.end(), 0);  // Tumu sifirla

    // Ilk 5'ini 100 yap
    std::fill_n(puanlar.begin(), 5, 100);

    std::cout << "Puanlar: ";
    for (int p : puanlar) std::cout << p << " ";
    std::cout << "\n";

    // --- 5. std::generate ---
    std::cout << "\n--- std::generate ---\n";
    std::vector<int> ogrenci_numaralari(5);

    int sayac = 2001;
    std::generate(ogrenci_numaralari.begin(),
                  ogrenci_numaralari.end(),
                  [&sayac]() { return sayac++; });

    std::cout << "Ogrenci numaralari: ";
    for (int no : ogrenci_numaralari) std::cout << no << " ";
    std::cout << "\n";

    // Rastgele notlar olustur
    std::vector<int> rastgele_notlar(8);
    std::mt19937 gen(42);  // Sabit seed (tekrarlanabilir)
    std::uniform_int_distribution<int> dagitim(50, 100);

    std::generate(rastgele_notlar.begin(), rastgele_notlar.end(),
                  [&]() { return dagitim(gen); });

    std::cout << "Rastgele notlar: ";
    for (int n : rastgele_notlar) std::cout << n << " ";
    std::cout << "\n";

    // --- 6. std::for_each ---
    std::cout << "\n--- std::for_each ---\n";

    // Her ogrencinin bilgisini formatli yazdir
    std::cout << "Sinif listesi:\n";
    std::for_each(sinif.begin(), sinif.end(),
        [sira = 1](const Ogrenci& o) mutable {
            std::cout << "  " << sira++ << ". " << o.isim
                      << " [" << o.numara << "] - GPA: "
                      << std::fixed << std::setprecision(2) << o.gpa << "\n";
        });

    // --- 7. std::replace / std::replace_if ---
    std::cout << "\n--- std::replace_if ---\n";
    std::vector<int> degerler = {85, 45, 92, 38, 78, 55, 91};

    // 50 altini 50 yap (minimum not esigi)
    std::replace_if(degerler.begin(), degerler.end(),
                    [](int n) { return n < 50; }, 50);

    std::cout << "Esik uygulandiktan sonra: ";
    for (int n : degerler) std::cout << n << " ";
    std::cout << "\n";

    // --- 8. OBYS: Transform ile rapor ---
    std::cout << "\n--- OBYS: Transform ile Rapor ---\n";
    std::vector<std::string> rapor;
    rapor.reserve(sinif.size());

    std::transform(sinif.begin(), sinif.end(),
                   std::back_inserter(rapor),
                   [](const Ogrenci& o) -> std::string {
                       std::string durum = (o.gpa >= 3.0) ? "Basarili" : "Kosula bagli";
                       return o.isim + " -> " + durum;
                   });

    std::cout << "Durum raporu:\n";
    for (const auto& r : rapor) {
        std::cout << "  " << r << "\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Transform ve Copy ===
 *
 * --- std::transform (tekli) ---
 * Orijinal: 85 92 78 95 88
 * Egrili:   90 97 83 100 93
 *
 * Buyuk harfler: ALI AYSE MEHMET ZEYNEP
 *
 * --- std::transform (ikili) ---
 * Vize:       80   70   90   85   60
 * Final:      90   80   85   95   70
 * Ortalama:   86   76   87   91   66
 *
 * --- std::copy_if ---
 * Onur listesi:
 *   Ayse Kaya (GPA: 3.80)
 *   Zeynep Can (GPA: 3.65)
 *
 * --- std::fill ---
 * Puanlar: 100 100 100 100 100 0 0 0 0 0
 *
 * --- std::generate ---
 * Ogrenci numaralari: 2001 2002 2003 2004 2005
 * Rastgele notlar: ...
 *
 * --- std::for_each ---
 * Sinif listesi:
 *   1. Ali Yilmaz [1001] - GPA: 3.45
 *   2. Ayse Kaya [1002] - GPA: 3.80
 *   3. Mehmet Demir [1003] - GPA: 2.90
 *   4. Zeynep Can [1004] - GPA: 3.65
 *   5. Can Arslan [1005] - GPA: 2.10
 *
 * --- std::replace_if ---
 * Esik uygulandiktan sonra: 85 50 92 50 78 55 91
 *
 * --- OBYS: Transform ile Rapor ---
 * Durum raporu:
 *   Ali Yilmaz -> Basarili
 *   Ayse Kaya -> Basarili
 *   Mehmet Demir -> Kosula bagli
 *   Zeynep Can -> Basarili
 *   Can Arslan -> Kosula bagli
 */

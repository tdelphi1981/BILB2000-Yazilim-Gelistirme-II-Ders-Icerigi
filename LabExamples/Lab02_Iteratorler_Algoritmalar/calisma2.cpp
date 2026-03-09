/**
 * Lab 02 - Çalışma 2 Başlangıç Kodu
 * Sıralama ve Arama
 *
 * Bu dosya Lab 02 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - std::sort ile özel karşılaştırma (lambda)
 * - std::find_if ile koşullu arama
 * - std::binary_search ile hızlı arama
 * - std::lower_bound ile konum bulma
 * - std::partial_sort ve std::count_if
 *
 * Lab: 02 - İteratörler ve STL Algoritmaları
 * Çalışma: 2 - Sıralama ve Arama (20 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma2 calisma2.cpp
 * Çalıştırma: ./calisma2
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

int main() {
    std::vector<Ogrenci> liste = {
        {2001, "Ali Yilmaz", 3.45},
        {2002, "Ayse Kaya", 3.80},
        {2003, "Mehmet Demir", 2.90},
        {2004, "Zeynep Can", 3.60},
        {2005, "Can Ozturk", 2.10},
        {2006, "Elif Aksoy", 3.95}
    };

    std::cout << "=== Calisma 2: Siralama ve Arama ===\n\n";

    // GPA'ya göre sırala (büyükten küçüğe)
    std::sort(liste.begin(), liste.end(),
        [](const Ogrenci& a, const Ogrenci& b) {
            return a.gpa > b.gpa;
        });

    std::cout << "--- GPA Siralamasi (buyukten kucuge) ---\n";
    for (const auto& o : liste) {
        std::cout << o.isim << " - GPA: " << o.gpa << '\n';
    }

    // === GOREV 2.1 - find_if ile Arama (5 dk) ===
    std::cout << "\n[GOREV 2.1]\n";
    // TODO: std::find_if kullanarak GPA'sı 3.5'in üzerinde olan
    //       ilk öğrenciyi bulun ve yazdırın:
    //
    //       auto it = std::find_if(liste.begin(), liste.end(),
    //           [](const Ogrenci& o) { return o.gpa > 3.5; });
    //       if (it != liste.end()) {
    //           std::cout << "\nIlk basarili: " << it->isim
    //                     << " (GPA: " << it->gpa << ")\n";
    //       }
    // ============================================

    // === GOREV 2.2 - binary_search ile Hızlı Arama (10 dk) ===
    std::cout << "\n[GOREV 2.2]\n";
    // TODO: a) Listeyi numaraya göre küçükten büyüğe sıralayın:
    //          std::sort(liste.begin(), liste.end(),
    //              [](const Ogrenci& a, const Ogrenci& b) {
    //                  return a.numara < b.numara;
    //              });
    //
    // TODO: b) std::binary_search ile 2004 numaralı öğrencinin
    //          varlığını kontrol edin:
    //          bool var = std::binary_search(liste.begin(), liste.end(),
    //              Ogrenci{2004, "", 0.0},
    //              [](const Ogrenci& a, const Ogrenci& b) {
    //                  return a.numara < b.numara;
    //              });
    //
    // TODO: c) std::lower_bound ile 2004 numaralı öğrencinin
    //          iteratörünü bulun ve bilgilerini yazdırın
    //
    // Soru: binary_search neden sadece bool döner? Konumu bulmak
    //       için ne kullanırsınız? (lower_bound veya equal_range)
    // ============================================

    // === GOREV 2.3 - partial_sort ve count_if (5 dk) ===
    std::cout << "\n[GOREV 2.3]\n";
    // TODO: a) std::partial_sort ile sadece en yüksek GPA'lı
    //          3 öğrenciyi sıralayın:
    //          std::partial_sort(liste.begin(), liste.begin() + 3,
    //              liste.end(),
    //              [](const Ogrenci& a, const Ogrenci& b) {
    //                  return a.gpa > b.gpa;
    //              });
    //
    // TODO: b) std::count_if ile GPA'sı 3.0'ın üzerindeki
    //          öğrenci sayısını bulun:
    //          auto sayi = std::count_if(liste.begin(), liste.end(),
    //              [](const Ogrenci& o) { return o.gpa > 3.0; });
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 2: Siralama ve Arama ===

--- GPA Siralamasi (buyukten kucuge) ---
Elif Aksoy - GPA: 3.95
Ayse Kaya - GPA: 3.8
Zeynep Can - GPA: 3.6
Ali Yilmaz - GPA: 3.45
Mehmet Demir - GPA: 2.9
Can Ozturk - GPA: 2.1

[GOREV 2.1]

[GOREV 2.2]

[GOREV 2.3]
*/

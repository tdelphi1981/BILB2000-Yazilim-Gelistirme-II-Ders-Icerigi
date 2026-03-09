/**
 * Lab 03 - Çalışma 1 Başlangıç Kodu
 * Ranges ve Projeksiyonlar
 *
 * Bu dosya Lab 03 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - std::ranges::sort ile projeksiyon
 * - std::ranges::find_if ile projeksiyon
 * - views::filter ve views::transform pipeline
 * - views::take ve views::drop
 * - Pipe (|) operatörü ile view zincirleme
 *
 * Lab: 03 - Modern C++ ve STL Uygulamaları
 * Çalışma: 1 - Ranges ve Projeksiyonlar (25 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma1 calisma1.cpp
 * Çalıştırma: ./calisma1
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ranges>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

int main() {
    std::vector<Ogrenci> liste = {
        {2001, "Ali Yilmaz",    3.45},
        {2002, "Ayse Kaya",     3.80},
        {2003, "Mehmet Demir",  2.30},
        {2004, "Zeynep Can",    3.60},
        {2005, "Can Ozturk",    2.10},
        {2006, "Elif Aksoy",    3.95},
        {2007, "Burak Celik",   2.75},
        {2008, "Deniz Yildiz",  3.20}
    };

    // ranges::sort ile GPA'ya gore sirala (buyukten kucuge)
    std::ranges::sort(liste, std::greater{}, &Ogrenci::gpa);

    std::cout << "=== Calisma 1: Ranges ve Projeksiyonlar ===\n\n";
    std::cout << "--- GPA Siralamasi ---\n";
    for (const auto& o : liste) {
        std::cout << o.isim << " - GPA: " << o.gpa << '\n';
    }

    // ranges::find_if ile ilk dusuk GPA'li ogrenciyi bul
    auto it = std::ranges::find_if(liste,
        [](double g) { return g < 2.5; }, &Ogrenci::gpa);

    if (it != liste.end()) {
        std::cout << "\nIlk dusuk GPA: " << it->isim
                  << " (" << it->gpa << ")\n";
    }

    // === GOREV 1.1 - ranges::sort ile Projeksiyon (5 dk) ===
    std::cout << "\n[GOREV 1.1]\n";
    // TODO: a) Ogrenci listesini isme gore alfabetik siralayın.
    //          Ozel karsilastirma fonksiyonu yazmak yerine
    //          projeksiyon kullanin:
    //          std::ranges::sort(liste, std::less{}, &Ogrenci::isim);
    //
    // TODO: b) Sirali listeyi yazdir
    // ============================================

    // === GOREV 1.2 - views::filter ve views::transform Pipeline (10 dk) ===
    std::cout << "\n[GOREV 1.2]\n";
    // TODO: a) views::filter ile GPA > 2.5 kosulunu uygulayin
    //
    // TODO: b) views::transform ile her ogrencinin ismini buyuk harfe
    //          cevirin. Yardimci lambda:
    //          auto buyuk_harf = [](std::string s) {
    //              for (auto& c : s) c = std::toupper(c);
    //              return s;
    //          };
    //
    // TODO: c) Pipeline'i pipe (|) operatoru ile olusturun:
    //          auto sonuc = liste
    //              | std::views::filter([](const Ogrenci& o) {
    //                    return o.gpa > 2.5;
    //                })
    //              | std::views::transform([&](const Ogrenci& o) {
    //                    return buyuk_harf(o.isim);
    //                });
    //
    // TODO: d) Sonuclari yazdir
    // ============================================

    // === GOREV 1.3 - views::take ve views::drop (10 dk) ===
    std::cout << "\n[GOREV 1.3]\n";
    // TODO: a) Listeyi GPA'ya gore buyukten kucuge siralay
    //
    // TODO: b) views::take(3) ile en yuksek GPA'li ilk 3 ogrenciyi
    //          yazdir:
    //          for (const auto& o : liste | std::views::take(3)) {
    //              std::cout << o.isim << " - " << o.gpa << '\n';
    //          }
    //
    // TODO: c) views::drop(liste.size() - 2) ile en dusuk GPA'li
    //          son 2 ogrenciyi yazdir
    //
    // TODO: d) Her iki sonucu std::ranges::for_each ile yazdir
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 1: Ranges ve Projeksiyonlar ===

--- GPA Siralamasi ---
Elif Aksoy - GPA: 3.95
Ayse Kaya - GPA: 3.8
Zeynep Can - GPA: 3.6
Ali Yilmaz - GPA: 3.45
Deniz Yildiz - GPA: 3.2
Burak Celik - GPA: 2.75
Mehmet Demir - GPA: 2.3
Can Ozturk - GPA: 2.1

Ilk dusuk GPA: Mehmet Demir (2.3)

[GOREV 1.1]

[GOREV 1.2]

[GOREV 1.3]
*/

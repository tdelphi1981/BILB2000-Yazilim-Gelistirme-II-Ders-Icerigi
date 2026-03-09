/**
 * Lab 02 - Çalışma 1 Başlangıç Kodu
 * İteratör Kullanımı
 *
 * Bu dosya Lab 02 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - Structured bindings ile map dolaşma
 * - İteratör ile dolaşma (begin/end)
 * - Ters iteratör (rbegin/rend)
 * - std::distance ve std::advance
 * - const_iterator ve cbegin/cend
 * - ostream_iterator ile akış çıktısı
 *
 * Lab: 02 - İteratörler ve STL Algoritmaları
 * Çalışma: 1 - İteratör Kullanımı (25 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma1 calisma1.cpp
 * Çalıştırma: ./calisma1
 */

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

int main() {
    std::map<int, std::string> ogrenciler = {
        {2001, "Ali Yilmaz"},
        {2002, "Ayse Kaya"},
        {2003, "Mehmet Demir"},
        {2004, "Zeynep Can"},
        {2005, "Can Ozturk"}
    };

    std::cout << "=== Calisma 1: Iterator Kullanimi ===\n\n";

    // C++17 structured bindings ile dolaşma
    std::cout << "--- Ogrenci Listesi ---\n";
    for (const auto& [no, isim] : ogrenciler) {
        std::cout << no << ": " << isim << '\n';
    }

    // İteratör ile dolaşma
    std::cout << "\n--- Iterator ile ---\n";
    for (auto it = ogrenciler.begin();
         it != ogrenciler.end(); ++it) {
        std::cout << it->first << ": "
                  << it->second << '\n';
    }

    // === GOREV 1.1 - Ters İteratör ile Dolaşma (5 dk) ===
    std::cout << "\n[GOREV 1.1]\n";
    // TODO: rbegin() ve rend() kullanarak öğrenci listesini
    //       tersten (büyük numaradan küçüğe) yazdırın
    //
    //       std::cout << "\n--- Ters Sirada ---\n";
    //       for (auto rit = ogrenciler.rbegin();
    //            rit != ogrenciler.rend(); ++rit) {
    //           std::cout << rit->first << ": " << rit->second << '\n';
    //       }
    // ============================================

    // === GOREV 1.2 - distance ve advance (10 dk) ===
    std::cout << "\n[GOREV 1.2]\n";
    // TODO: a) std::distance ile eleman sayısını hesaplayın:
    //          auto n = std::distance(ogrenciler.begin(), ogrenciler.end());
    //          std::cout << "Eleman sayisi: " << n << '\n';
    //
    // TODO: b) std::advance ile begin() iteratörünü 2 adım ilerletin
    //          ve o öğrenciyi yazdırın:
    //          auto it = ogrenciler.begin();
    //          std::advance(it, 2);
    //          std::cout << it->first << ": " << it->second << '\n';
    //
    // TODO: c) std::next(it, n) ile std::advance(it, n) arasındaki
    //          farkı test edin:
    //          auto it2 = std::next(ogrenciler.begin(), 3);  // yeni iteratör döner
    //          // vs advance: mevcut iteratörü değiştirir
    // ============================================

    // === GOREV 1.3 - const_iterator ve Güvenlik (10 dk) ===
    std::cout << "\n[GOREV 1.3]\n";
    // TODO: a) cbegin() ve cend() kullanarak listeyi dolaşın
    //
    // TODO: b) const_iterator üzerinden bir öğrencinin ismini
    //          değiştirmeye çalışın. Ne olur? (Derleme hatası!)
    //          // auto cit = ogrenciler.cbegin();
    //          // cit->second = "Yeni Isim";  // [HATA] derlenmez!
    //
    // TODO: c) const auto& vs auto& kullanımının neden önemli
    //          olduğunu bir yorum satırı ile açıklayın
    // ============================================

    // === GOREV 1.4 - Akış İteratörleri (10 dk) ===
    std::cout << "\n[GOREV 1.4]\n";
    // TODO: a) map'ten numaraları vector'e aktarın:
    //          std::vector<int> numaralar;
    //          for (const auto& [no, isim] : ogrenciler) {
    //              numaralar.push_back(no);
    //          }
    //
    // TODO: b) ostream_iterator ile numaraları yazdırın:
    //          std::copy(numaralar.begin(), numaralar.end(),
    //              std::ostream_iterator<int>(std::cout, ", "));
    //
    // TODO: c) Ayırıcıyı ", " yerine " | " olarak değiştirin
    //
    // TODO: d) Aynı tekniği öğrenci isimlerini yazdırmak için uygulayın
    //          (ostream_iterator<std::string>)
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 1: Iterator Kullanimi ===

--- Ogrenci Listesi ---
2001: Ali Yilmaz
2002: Ayse Kaya
2003: Mehmet Demir
2004: Zeynep Can
2005: Can Ozturk

--- Iterator ile ---
2001: Ali Yilmaz
2002: Ayse Kaya
2003: Mehmet Demir
2004: Zeynep Can
2005: Can Ozturk

[GOREV 1.1]

[GOREV 1.2]

[GOREV 1.3]

[GOREV 1.4]
*/

/**
 * Lab 02 - Çalışma 3 Başlangıç Kodu
 * Transform ve Filtreleme
 *
 * Bu dosya Lab 02 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - std::transform ile dönüşüm
 * - std::copy_if ve back_inserter ile filtreleme
 * - Remove-erase idiomu (C++17)
 * - std::erase_if (C++20)
 * - std::fill, std::generate, std::for_each
 *
 * Lab: 02 - İteratörler ve STL Algoritmaları
 * Çalışma: 3 - Transform ve Filtreleme (25 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma3 calisma3.cpp
 * Çalıştırma: ./calisma3
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

int main() {
    std::vector<int> notlar = {85, 42, 95, 60, 78, 92, 55, 88};

    std::cout << "=== Calisma 3: Transform ve Filtreleme ===\n\n";

    // transform: not -> harf notu dönüşümü
    std::vector<std::string> harfler(notlar.size());
    std::transform(notlar.begin(), notlar.end(), harfler.begin(),
        [](int n) -> std::string {
            if (n >= 90) return "AA";
            if (n >= 80) return "BA";
            if (n >= 70) return "BB";
            if (n >= 60) return "CB";
            return "FF";
        });

    std::cout << "--- Not -> Harf Donusumu ---\n";
    for (size_t i = 0; i < notlar.size(); ++i) {
        std::cout << notlar[i] << " -> " << harfler[i] << '\n';
    }

    // === GOREV 3.1 - copy_if ile Filtreleme (5 dk) ===
    std::cout << "\n[GOREV 3.1]\n";
    // TODO: std::copy_if ve std::back_inserter kullanarak
    //       notları 70 ve üstü olanları yeni bir vector'e kopyalayın:
    //
    //       std::vector<int> basarili;
    //       std::copy_if(notlar.begin(), notlar.end(),
    //           std::back_inserter(basarili),
    //           [](int n) { return n >= 70; });
    //       std::cout << "\nBasarili not sayisi: " << basarili.size() << '\n';
    // ============================================

    // === GOREV 3.2 - Remove-Erase İdiomu (10 dk) ===
    std::cout << "\n[GOREV 3.2]\n";
    // TODO: a) C++17 yöntemi: Remove-erase idiomu ile 60'ın altındaki
    //          notları silin:
    //          std::vector<int> notlar_kopya = notlar;  // Kopya üzerinde çalış
    //          std::cout << "\nSilme oncesi size: " << notlar_kopya.size() << '\n';
    //          notlar_kopya.erase(
    //              std::remove_if(notlar_kopya.begin(), notlar_kopya.end(),
    //                  [](int n) { return n < 60; }),
    //              notlar_kopya.end());
    //          std::cout << "Silme sonrasi size: " << notlar_kopya.size() << '\n';
    //
    // TODO: b) Deney: Sadece std::remove_if çağırsanız ama erase
    //          çağırmasanız size() değişir mi? Neden?
    //          (İpucu: remove_if sadece elemanları taşır, boyut değişmez)
    // ============================================

    // === GOREV 3.3 - C++20 erase_if (5 dk) ===
    std::cout << "\n[GOREV 3.3]\n";
    // TODO: Aynı işlemi C++20 std::erase_if ile tek satırda yapın:
    //       std::vector<int> notlar_kopya2 = notlar;
    //       std::erase_if(notlar_kopya2, [](int n) { return n < 60; });
    //
    //       Soru: C++20 yaklaşımının avantajları nelerdir?
    //       (Daha okunaklı, tek satır, hata yapma riski düşük)
    // ============================================

    // === GOREV 3.4 - transform ile Eğri Uygulama (5 dk) ===
    std::cout << "\n[GOREV 3.4]\n";
    // TODO: Tüm notlara 10 puan ekleyen (ancak 100'ü geçmeyen)
    //       bir transform çağrısı yazın:
    //       std::vector<int> egrili_notlar = notlar;
    //       std::transform(egrili_notlar.begin(), egrili_notlar.end(),
    //           egrili_notlar.begin(),
    //           [](int n) { return std::min(n + 10, 100); });
    // ============================================

    // === GOREV 3.5 - fill, generate ve for_each (10 dk) ===
    std::cout << "\n[GOREV 3.5]\n";
    // TODO: a) std::fill ile 8 elemanlık vector<int>'i sıfırlayın:
    //          std::vector<int> v(8);
    //          std::fill(v.begin(), v.end(), 0);
    //
    // TODO: b) std::generate ile aynı vektörü öğrenci numaraları ile
    //          doldurun (2001'den başlayarak artan):
    //          int numara = 2000;
    //          std::generate(v.begin(), v.end(),
    //              [&numara]() { return ++numara; });
    //
    // TODO: c) std::for_each ile her elemana 5 puan ekleyin
    //          (referans yakalama ile):
    //          std::for_each(v.begin(), v.end(),
    //              [](int& n) { n += 5; });
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 3: Transform ve Filtreleme ===

--- Not -> Harf Donusumu ---
85 -> BA
42 -> FF
95 -> AA
60 -> CB
78 -> BB
92 -> AA
55 -> FF
88 -> BA

[GOREV 3.1]

[GOREV 3.2]

[GOREV 3.3]

[GOREV 3.4]

[GOREV 3.5]
*/

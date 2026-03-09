/**
 * Lab 01 - Çalışma 1 Başlangıç Kodu
 * vector ile Öğrenci Listesi
 *
 * Bu dosya Lab 01 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - std::vector temel kullanımı
 * - push_back() ve emplace_back()
 * - size() ve capacity() farkı
 * - reserve() ile optimizasyon
 * - std::find ve erase()
 * - at() ile güvenli erişim
 *
 * Lab: 01 - STL Kapsayıcılara Giriş
 * Çalışma: 1 - vector ile Öğrenci Listesi (25 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma1 calisma1.cpp
 * Çalıştırma: ./calisma1
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

int main() {
    std::vector<std::string> ogrenciler;

    // Öğrenci ekle
    ogrenciler.push_back("Ali Yilmaz");
    ogrenciler.push_back("Ayse Kaya");
    ogrenciler.push_back("Mehmet Demir");
    ogrenciler.emplace_back("Zeynep Can");

    std::cout << "=== Calisma 1: vector ile Ogrenci Listesi ===\n\n";
    std::cout << "Ogrenci sayisi: " << ogrenciler.size() << '\n';
    std::cout << "Kapasite: " << ogrenciler.capacity() << '\n';

    // Range-based for ile listele
    std::cout << "\nOgrenci listesi:\n";
    for (const auto& isim : ogrenciler) {
        std::cout << "  - " << isim << '\n';
    }

    // === GOREV 1.1 - Kapasite Gözlemi (5 dk) ===
    std::cout << "\n[GOREV 1.1]\n";
    // TODO: a) Döngü ile 20 öğrenci daha ekleyin ("Ogrenci_1", "Ogrenci_2", ...)
    // TODO: b) Her ekleme sonrası size() ve capacity() değerlerini yazdırın
    // TODO: c) Kapasitenin hangi anlarda arttığını ve artış oranını gözlemleyin
    // ============================================

    // === GOREV 1.2 - reserve() ile Optimizasyon (5 dk) ===
    std::cout << "\n[GOREV 1.2]\n";
    // TODO: a) Programın başına ogrenciler.reserve(30) ekleyin
    // TODO: b) Görev 1.1'i tekrarlayın
    // TODO: c) Yeniden tahsis (reallocation) sayısının nasıl değiştiğini gözlemleyin
    // ============================================

    // === GOREV 1.3 - Arama ve Silme (10 dk) ===
    std::cout << "\n[GOREV 1.3]\n";
    // TODO: a) std::find ile "Mehmet Demir" adlı öğrenciyi bulun
    // TODO: b) Bulduğunuz öğrenciyi erase() ile silin
    // TODO: c) at() ile güvenli erişim yapın ve out_of_range istisnasını yakalayın
    //
    // İpucu:
    // auto it = std::find(ogrenciler.begin(), ogrenciler.end(), "Mehmet Demir");
    // if (it != ogrenciler.end()) { ogrenciler.erase(it); }
    //
    // try { ogrenciler.at(100); }
    // catch (const std::out_of_range& e) { ... }
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 1: vector ile Ogrenci Listesi ===

Ogrenci sayisi: 4
Kapasite: 4

Ogrenci listesi:
  - Ali Yilmaz
  - Ayse Kaya
  - Mehmet Demir
  - Zeynep Can

[GOREV 1.1]

[GOREV 1.2]

[GOREV 1.3]
*/

/**
 * Lab 01 - Çalışma 2 Başlangıç Kodu
 * deque ve Diğer Sıralı Kapsayıcılar
 *
 * Bu dosya Lab 01 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - std::deque push_front performansı
 * - std::list push_front performansı
 * - std::vector insert ve push_back karşılaştırması
 * - Zamanlama ile performans ölçümü
 *
 * Lab: 01 - STL Kapsayıcılara Giriş
 * Çalışma: 2 - deque ve Diğer Sıralı Kapsayıcılar (20 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma2 calisma2.cpp
 * Çalıştırma: ./calisma2
 */

#include <iostream>
#include <deque>
#include <list>
#include <vector>
#include <string>
#include <chrono>

// Basit zamanlama fonksiyonu
template <typename Func>
double olc(Func f) {
    auto bas = std::chrono::high_resolution_clock::now();
    f();
    auto son = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> sure = son - bas;
    return sure.count();
}

int main() {
    const int N = 100000;

    std::cout << "=== Calisma 2: Kapsayici Performans Karsilastirmasi ===\n";
    std::cout << "N = " << N << " eleman\n\n";

    // deque: başa ekleme
    std::deque<int> d;
    double t1 = olc([&]() {
        for (int i = 0; i < N; ++i)
            d.push_front(i);
    });
    std::cout << "deque push_front: " << t1 << " ms\n";

    // list: başa ekleme
    std::list<int> l;
    double t2 = olc([&]() {
        for (int i = 0; i < N; ++i)
            l.push_front(i);
    });
    std::cout << "list  push_front: " << t2 << " ms\n";

    // === GOREV 2.1 - vector ile Karşılaştırma (10 dk) ===
    std::cout << "\n[GOREV 2.1]\n";
    // TODO: a) vector'e baştan ekleme testi ekleyin:
    //          std::vector<int> v1;
    //          v1.insert(v1.begin(), i);  // Her adımda başa ekle
    //
    // TODO: b) vector'e sondan ekleme testi ekleyin:
    //          std::vector<int> v2;
    //          v2.push_back(i);
    //
    // TODO: c) Dört testi karşılaştırın ve neden vector baştan ekleme
    //          yavaş olduğunu açıklayın (yorum olarak)
    // ============================================

    // === GOREV 2.2 - Ortadan Ekleme/Silme (10 dk) ===
    std::cout << "\n[GOREV 2.2]\n";
    // TODO: a) Her üç kapsayıcı (vector, deque, list) için ortadaki
    //          elemana erişim süresini ölçün
    //
    // TODO: b) Her üç kapsayıcı için ortadan silme süresini ölçün
    //
    // TODO: c) Sonuçları tablo olarak yazdırın:
    //          İşlem        | vector  | deque   | list
    //          Baştan ekle  | ...ms   | ...ms   | ...ms
    //          Sondan ekle  | ...ms   | ...ms   | ...ms
    //          Ortadan silme| ...ms   | ...ms   | ...ms
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 2: Kapsayici Performans Karsilastirmasi ===
N = 100000 eleman

deque push_front: X.XX ms
list  push_front: X.XX ms

[GOREV 2.1]

[GOREV 2.2]
*/

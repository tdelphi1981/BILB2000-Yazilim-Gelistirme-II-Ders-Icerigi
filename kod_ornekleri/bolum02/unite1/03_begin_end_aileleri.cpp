/**
 * begin/end Ailesi - Iterator Erisim Fonksiyonlari
 *
 * Bu program begin/end ailesinin farkli varyantlarini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - begin/end, cbegin/cend, rbegin/rend, crbegin/crend
 * - Serbest fonksiyonlar: std::begin, std::end
 * - C dizileri ile begin/end kullanimi
 * - C++20 std::ssize()
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 1 - Iterator Kavrami
 *
 * Derleme: g++ -std=c++20 -o 03_begin_end_aileleri 03_begin_end_aileleri.cpp
 * Calistirma: ./03_begin_end_aileleri
 */

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>

int main() {
    std::cout << "=== begin/end Ailesi ===\n\n";

    std::vector<int> notlar = {85, 92, 78, 95, 88, 72, 91};

    // --- 1. begin/end (degistirilebilir) ---
    std::cout << "--- begin/end ---\n";
    std::cout << "Notlar: ";
    for (auto it = notlar.begin(); it != notlar.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Degistirebiliriz
    *notlar.begin() = 100;
    std::cout << "Ilk not degistirildi: " << notlar.front() << "\n";
    *notlar.begin() = 85; // geri al

    // --- 2. cbegin/cend (const - salt okunur) ---
    std::cout << "\n--- cbegin/cend (const) ---\n";
    std::cout << "Salt okunur dolasim: ";
    for (auto it = notlar.cbegin(); it != notlar.cend(); ++it) {
        std::cout << *it << " ";
        // *it = 100; // DERLEME HATASI! const iterator
    }
    std::cout << "\n";

    // --- 3. rbegin/rend (ters) ---
    std::cout << "\n--- rbegin/rend (ters) ---\n";
    std::cout << "Ters sirada: ";
    for (auto it = notlar.rbegin(); it != notlar.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // --- 4. crbegin/crend (const ters) ---
    std::cout << "\n--- crbegin/crend (const ters) ---\n";
    std::cout << "Salt okunur ters: ";
    for (auto it = notlar.crbegin(); it != notlar.crend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // --- 5. Serbest fonksiyonlar: std::begin / std::end ---
    std::cout << "\n--- std::begin / std::end ---\n";

    // C-tarzı dizi ile calismak
    int c_dizi[] = {10, 20, 30, 40, 50};

    // C dizisinde .begin() uye fonksiyonu YOKTUR
    // Ama std::begin / std::end calışır!
    std::cout << "C dizisi: ";
    for (auto it = std::begin(c_dizi); it != std::end(c_dizi); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // C dizisini siralama
    std::sort(std::begin(c_dizi), std::end(c_dizi),
              std::greater<int>());
    std::cout << "Sirali (azalan): ";
    for (auto it = std::begin(c_dizi); it != std::end(c_dizi); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Eleman sayisi
    auto eleman_sayisi = std::end(c_dizi) - std::begin(c_dizi);
    std::cout << "C dizisi boyutu: " << eleman_sayisi << "\n";

    // --- 6. std::size ve std::ssize (C++17/20) ---
    std::cout << "\n--- std::size / std::ssize ---\n";

    // std::size (C++17) - isaretsiz (unsigned) dondurur
    std::cout << "vector size: " << std::size(notlar) << "\n";
    std::cout << "C dizi size: " << std::size(c_dizi) << "\n";

    // std::ssize (C++20) - isaretli (signed) dondurur
    // Negatif indeks hesaplamalarinda guvenli
    auto n = std::ssize(notlar);
    std::cout << "vector ssize: " << n << " (signed)\n";

    // ssize avantaji: geriye dogru dongu guvenli
    std::cout << "Geriye dongu: ";
    for (auto i = std::ssize(notlar) - 1; i >= 0; --i) {
        std::cout << notlar[static_cast<std::size_t>(i)] << " ";
    }
    std::cout << "\n";

    // --- 7. std::data (C++17) ---
    std::cout << "\n--- std::data ---\n";
    // Ham isarciye erisim (C API uyumlulugu icin)
    int* ham_ptr = std::data(notlar);
    std::cout << "Ilk eleman (ham isarci): " << *ham_ptr << "\n";

    // std::array ile de calisir
    std::array<double, 3> gpa_lar = {3.45, 3.80, 2.90};
    const double* gpa_ptr = std::data(gpa_lar);
    std::cout << "Ilk GPA (ham isarci): " << *gpa_ptr << "\n";

    // --- 8. std::empty (C++17) ---
    std::cout << "\n--- std::empty ---\n";
    std::vector<int> bos_vec;
    std::cout << "notlar bos mu? " << (std::empty(notlar) ? "Evet" : "Hayir") << "\n";
    std::cout << "bos_vec bos mu? " << (std::empty(bos_vec) ? "Evet" : "Hayir") << "\n";

    // --- 9. Neden serbest fonksiyonlar tercih edilmeli? ---
    std::cout << "\n--- Jenerik Kod Ornegi ---\n";

    // Bu fonksiyon hem STL kapsayicilari hem C dizileri ile calisir
    auto toplam_hesapla = [](auto& kapsayici) {
        return std::accumulate(std::begin(kapsayici),
                               std::end(kapsayici), 0);
    };

    std::vector<int> vec = {1, 2, 3, 4, 5};
    int arr[] = {1, 2, 3, 4, 5};

    std::cout << "vector toplami: " << toplam_hesapla(vec) << "\n";
    std::cout << "C dizi toplami: " << toplam_hesapla(arr) << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === begin/end Ailesi ===
 *
 * --- begin/end ---
 * Notlar: 85 92 78 95 88 72 91
 * Ilk not degistirildi: 100
 *
 * --- cbegin/cend (const) ---
 * Salt okunur dolasim: 85 92 78 95 88 72 91
 *
 * --- rbegin/rend (ters) ---
 * Ters sirada: 91 72 88 95 78 92 85
 *
 * --- crbegin/crend (const ters) ---
 * Salt okunur ters: 91 72 88 95 78 92 85
 *
 * --- std::begin / std::end ---
 * C dizisi: 10 20 30 40 50
 * Sirali (azalan): 50 40 30 20 10
 * C dizisi boyutu: 5
 *
 * --- std::size / std::ssize ---
 * vector size: 7
 * C dizi size: 5
 * vector ssize: 7 (signed)
 * Geriye dongu: 91 72 88 95 78 92 85
 *
 * --- std::data ---
 * Ilk eleman (ham isarci): 85
 * Ilk GPA (ham isarci): 3.45
 *
 * --- std::empty ---
 * notlar bos mu? Hayir
 * bos_vec bos mu? Evet
 *
 * --- Jenerik Kod Ornegi ---
 * vector toplami: 15
 * C dizi toplami: 15
 */

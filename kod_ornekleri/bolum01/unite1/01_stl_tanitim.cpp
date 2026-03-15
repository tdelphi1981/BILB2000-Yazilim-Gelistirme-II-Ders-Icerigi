/**
 * STL Tanitim - Ilk STL Programi
 *
 * Bu program STL'in temel kapsayicilarini ve algoritmalarini
 * tanitir. vector, sort ve find kullanimi gosterilir.
 *
 * Ogrenilecek kavramlar:
 * - std::vector temel kullanimi
 * - std::sort ile siralama
 * - std::find ile arama
 * - Range-based for dongusu
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 1 - STL'e Genel Bakis
 *
 * Derleme: g++ -std=c++20 -o 01_stl_tanitim 01_stl_tanitim.cpp
 * Calistirma: ./01_stl_tanitim
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main() {
    // --- KAPSAYICI: vector ---
    // Ogrenci notlarini bir vector'de saklayalim
    std::vector<int> notlar = {85, 92, 78, 95, 88, 72, 91};

    std::cout << "=== STL Tanitim Programi ===\n\n";

    // Orijinal sirayla yazdir
    std::cout << "Notlar (orijinal): ";
    for (const auto& not_degeri : notlar) {
        std::cout << not_degeri << " ";
    }
    std::cout << "\n";

    // --- ALGORITMA: sort ---
    // Notlari kucukten buyuge sirala
    std::sort(notlar.begin(), notlar.end());

    std::cout << "Notlar (sirali):   ";
    for (const auto& not_degeri : notlar) {
        std::cout << not_degeri << " ";
    }
    std::cout << "\n\n";

    // --- ALGORITMA: find ---
    // Belirli bir notu ara
    int aranan = 92;
    auto it = std::find(notlar.begin(), notlar.end(), aranan);
    if (it != notlar.end()) {
        std::cout << aranan << " notu bulundu!\n";
    } else {
        std::cout << aranan << " notu bulunamadi.\n";
    }

    // --- FONKSIYON NESNESI: lambda ---
    // 90 ve ustu notlari say
    int basarili = std::count_if(notlar.begin(), notlar.end(),
        [](int n) { return n >= 90; });

    std::cout << "90 ve ustu not alan: " << basarili << " ogrenci\n\n";

    // --- STRING KAPSAYICI ---
    std::vector<std::string> isimler = {
        "Zeynep Kaya", "Ali Yilmaz", "Mehmet Demir",
        "Ayse Ozturk", "Can Arslan"
    };

    // Isimleri alfabetik sirala
    std::sort(isimler.begin(), isimler.end());

    std::cout << "Ogrenci listesi (alfabetik):\n";
    int sira = 1;
    for (const auto& isim : isimler) {
        std::cout << "  " << sira++ << ". " << isim << "\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === STL Tanitim Programi ===
 *
 * Notlar (orijinal): 85 92 78 95 88 72 91
 * Notlar (sirali):   72 78 85 88 91 92 95
 *
 * 92 notu bulundu!
 * 90 ve ustu not alan: 3 ogrenci
 *
 * Ogrenci listesi (alfabetik):
 *   1. Ali Yilmaz
 *   2. Ayse Ozturk
 *   3. Can Arslan
 *   4. Mehmet Demir
 *   5. Zeynep Kaya
 */

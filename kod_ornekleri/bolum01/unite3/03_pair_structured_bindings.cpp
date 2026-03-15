/**
 * pair ve Structured Bindings - Modern C++ Veri Eslestirme
 *
 * Bu program std::pair, std::tuple ve C++17 structured bindings
 * kullanimini gosterir. map ile entegrasyon ornekleri icerir.
 *
 * Ogrenilecek kavramlar:
 * - std::pair olusturma ve elemanlara erisim
 * - std::tuple ile coklu deger gruplama
 * - C++17 structured bindings (auto [a, b] = ...)
 * - map uzerinde structured bindings ile dolasma
 * - Fonksiyonlardan coklu deger dondurme
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 3 - Iliskisel Kapsayicilar
 *
 * Derleme: g++ -std=c++20 -o 03_pair_sb 03_pair_structured_bindings.cpp
 * Calistirma: ./03_pair_sb
 */

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <array>

// Bir fonksiyondan iki deger dondur
std::pair<bool, std::string> ogrenci_bul(
    const std::map<int, std::string>& depo, int numara) {
    auto it = depo.find(numara);
    if (it != depo.end()) {
        return {true, it->second};
    }
    return {false, ""};
}

// Bir fonksiyondan uc deger dondur
std::tuple<double, double, int> not_istatistik(
    const std::map<int, double>& notlar) {
    if (notlar.empty()) return {0.0, 0.0, 0};

    double toplam = 0.0;
    double en_yuksek = 0.0;
    for (const auto& [no, puan] : notlar) {
        toplam += puan;
        if (puan > en_yuksek) en_yuksek = puan;
    }
    double ortalama = toplam / static_cast<double>(notlar.size());
    return {ortalama, en_yuksek, static_cast<int>(notlar.size())};
}

int main() {
    // --- 1. pair Temelleri ---
    std::cout << "=== pair Temelleri ===\n\n";

    std::pair<int, std::string> ogr1{2001, "Ali Yilmaz"};
    auto ogr2 = std::make_pair(2002, std::string("Ayse Kaya"));

    // CTAD ile (C++17)
    std::pair ogr3{2003, std::string("Mehmet Demir")};

    std::cout << "pair.first:  " << ogr1.first << "\n";
    std::cout << "pair.second: " << ogr1.second << "\n";

    // pair karsilastirma
    std::pair a{1, 5};
    std::pair b{1, 3};
    std::pair c{2, 1};
    std::cout << "\n(1,5) > (1,3): " << std::boolalpha << (a > b) << "\n";
    std::cout << "(1,5) > (2,1): " << std::boolalpha << (a > c) << "\n";

    // --- 2. Structured Bindings ---
    std::cout << "\n=== Structured Bindings ===\n\n";

    // pair ile
    auto [numara, isim] = ogr1;
    std::cout << "pair -> [numara, isim]: "
              << numara << ", " << isim << "\n";

    // tuple ile
    std::tuple<int, std::string, double> bilgi{2001, "Ali", 3.45};
    auto [no, ad, gpa] = bilgi;
    std::cout << "tuple -> [no, ad, gpa]: "
              << no << ", " << ad << ", " << gpa << "\n";

    // array ile
    std::array<int, 3> rgb = {255, 128, 0};
    auto [r, g, bv] = rgb;
    std::cout << "array -> [r, g, b]: "
              << r << ", " << g << ", " << bv << "\n";

    // struct ile
    struct Nokta { double x; double y; };
    Nokta p{3.14, 2.71};
    auto [px, py] = p;
    std::cout << "struct -> [x, y]: "
              << px << ", " << py << "\n";

    // --- 3. map ile Structured Bindings ---
    std::cout << "\n=== map ile Structured Bindings ===\n\n";

    std::map<int, std::string> ogrenciler = {
        {2001, "Ali Yilmaz"},
        {2002, "Ayse Kaya"},
        {2003, "Mehmet Demir"},
        {2004, "Zeynep Celik"}
    };

    // Eski yontem (C++11)
    std::cout << "C++11 stili:\n";
    for (const auto& cift : ogrenciler) {
        std::cout << "  " << cift.first << ": " << cift.second << "\n";
    }

    // Yeni yontem (C++17)
    std::cout << "\nC++17 stili:\n";
    for (const auto& [ogrNo, ogrAd] : ogrenciler) {
        std::cout << "  " << ogrNo << ": " << ogrAd << "\n";
    }

    // insert sonucu kontrolu
    auto [it, eklendi] = ogrenciler.insert({2005, "Fatma Sahin"});
    std::cout << "\n2005 ekleme: "
              << (eklendi ? "Basarili" : "Zaten mevcut")
              << " -> " << it->second << "\n";

    // --- 4. Fonksiyondan Coklu Deger Dondurme ---
    std::cout << "\n=== Fonksiyondan Coklu Deger ===\n\n";

    // pair donduren fonksiyon
    auto [bulundu, bulunanIsim] = ogrenci_bul(ogrenciler, 2003);
    if (bulundu) {
        std::cout << "Bulunan: " << bulunanIsim << "\n";
    }

    auto [bulundu2, bulunanIsim2] = ogrenci_bul(ogrenciler, 9999);
    std::cout << "9999 bulundu: " << bulundu2 << "\n";

    // tuple donduren fonksiyon
    std::map<int, double> sinav_notlari = {
        {2001, 85.5}, {2002, 92.0}, {2003, 78.5},
        {2004, 95.0}, {2005, 88.0}
    };

    auto [ort, enYuksek, sayi] = not_istatistik(sinav_notlari);
    std::cout << "\nNot istatistikleri:\n";
    std::cout << "  Ortalama:   " << ort << "\n";
    std::cout << "  En yuksek:  " << enYuksek << "\n";
    std::cout << "  Ogrenci:    " << sayi << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ----------------
 * === pair Temelleri ===
 *
 * pair.first:  2001
 * pair.second: Ali Yilmaz
 *
 * (1,5) > (1,3): true
 * (1,5) > (2,1): false
 *
 * === Structured Bindings ===
 *
 * pair -> [numara, isim]: 2001, Ali Yilmaz
 * tuple -> [no, ad, gpa]: 2001, Ali, 3.45
 * array -> [r, g, b]: 255, 128, 0
 * struct -> [x, y]: 3.14, 2.71
 *
 * === map ile Structured Bindings ===
 *
 * C++11 stili:
 *   2001: Ali Yilmaz
 *   2002: Ayse Kaya
 *   2003: Mehmet Demir
 *   2004: Zeynep Celik
 *
 * C++17 stili:
 *   2001: Ali Yilmaz
 *   2002: Ayse Kaya
 *   2003: Mehmet Demir
 *   2004: Zeynep Celik
 *
 * 2005 ekleme: Basarili -> Fatma Sahin
 *
 * === Fonksiyondan Coklu Deger ===
 *
 * Bulunan: Mehmet Demir
 * 9999 bulundu: false
 *
 * Not istatistikleri:
 *   Ortalama:   87.8
 *   En yuksek:  95
 *   Ogrenci:    5
 */

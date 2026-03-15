/**
 * Views Kullanimi - Tembel Degerlendirme ile Veri Donusumleri
 *
 * Bu program, C++20 Views yapisinin temel kullanimini gosterir.
 * Views, veriler uzerinde tembel (lazy) donusumler uygulayarak
 * bellek ve performans kazanimi saglar.
 *
 * Ogrenilecek kavramlar:
 * - views::filter (kosullu filtreleme)
 * - views::transform (donusum)
 * - views::take ve views::drop
 * - views::keys ve views::values (map uzerinde)
 * - views::iota (sayi ureteci)
 * - Tembel degerlendirme (lazy evaluation)
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 1 - Ranges ve Views (C++20)
 *
 * Derleme: g++ -std=c++20 -o 02_views_kullanimi 02_views_kullanimi.cpp
 * Calistirma: ./02_views_kullanimi
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ranges>

struct Ogrenci {
    std::string isim;
    int numara;
    double ortalama;
};

int main() {
    std::vector<Ogrenci> sinif = {
        {"Ayse Yilmaz",   1001, 85.5},
        {"Mehmet Demir",   1002, 42.0},
        {"Zeynep Kara",    1003, 91.3},
        {"Ali Ozturk",     1004, 38.4},
        {"Fatma Celik",    1005, 78.9},
        {"Burak Sahin",    1006, 55.2},
        {"Elif Arslan",    1007, 96.1},
        {"Caner Yildiz",   1008, 47.8}
    };

    // =============================================
    // 1) views::filter - Gecen ogrencileri filtrele
    // =============================================
    std::cout << "=== Gecen Ogrenciler (Ort >= 50) ===\n";

    auto gecenler = sinif | std::views::filter([](const Ogrenci& o) {
        return o.ortalama >= 50.0;
    });

    // Tembel: gecenler henuz hesaplanmadi, erisildikce degerlendirilir
    for (const auto& ogr : gecenler) {
        std::cout << "  " << ogr.isim << " - " << ogr.ortalama << "\n";
    }

    // =============================================
    // 2) views::transform - Sadece isimleri al
    // =============================================
    std::cout << "\n=== Tum Ogrenci Isimleri ===\n";

    auto isimler = sinif | std::views::transform([](const Ogrenci& o) {
        return o.isim;
    });

    for (const auto& isim : isimler) {
        std::cout << "  " << isim << "\n";
    }

    // =============================================
    // 3) views::take ve views::drop
    // =============================================
    std::cout << "\n=== Ilk 3 Ogrenci (take) ===\n";
    for (const auto& ogr : sinif | std::views::take(3)) {
        std::cout << "  " << ogr.numara << " - " << ogr.isim << "\n";
    }

    std::cout << "\n=== Ilk 5'i Atla, Kalanlar (drop) ===\n";
    for (const auto& ogr : sinif | std::views::drop(5)) {
        std::cout << "  " << ogr.numara << " - " << ogr.isim << "\n";
    }

    // =============================================
    // 4) views::keys ve views::values (map uzerinde)
    // =============================================
    std::cout << "\n=== Ders Notlari (map - keys/values) ===\n";

    std::map<std::string, int> ders_notlari = {
        {"Matematik",   85},
        {"Fizik",       72},
        {"Programlama", 95},
        {"Veritabani",  88}
    };

    std::cout << "  Dersler: ";
    for (const auto& ders : ders_notlari | std::views::keys) {
        std::cout << ders << " ";
    }

    std::cout << "\n  Notlar:  ";
    for (const auto& not_val : ders_notlari | std::views::values) {
        std::cout << not_val << " ";
    }
    std::cout << "\n";

    // =============================================
    // 5) views::iota - Sayi dizisi uretme
    // =============================================
    std::cout << "\n=== Sayi Dizileri (iota) ===\n";

    std::cout << "  1-10 arasi: ";
    for (int n : std::views::iota(1, 11)) {
        std::cout << n << " ";
    }

    std::cout << "\n  Cift sayilar (1-20): ";
    for (int n : std::views::iota(1, 21)
                 | std::views::filter([](int x) { return x % 2 == 0; })) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Gecen Ogrenciler (Ort >= 50) ===
 *   Ayse Yilmaz - 85.5
 *   Zeynep Kara - 91.3
 *   Fatma Celik - 78.9
 *   Burak Sahin - 55.2
 *   Elif Arslan - 96.1
 *
 * === Tum Ogrenci Isimleri ===
 *   Ayse Yilmaz
 *   Mehmet Demir
 *   Zeynep Kara
 *   Ali Ozturk
 *   Fatma Celik
 *   Burak Sahin
 *   Elif Arslan
 *   Caner Yildiz
 *
 * === Ilk 3 Ogrenci (take) ===
 *   1001 - Ayse Yilmaz
 *   1002 - Mehmet Demir
 *   1003 - Zeynep Kara
 *
 * === Ilk 5'i Atla, Kalanlar (drop) ===
 *   1006 - Burak Sahin
 *   1007 - Elif Arslan
 *   1008 - Caner Yildiz
 *
 * === Ders Notlari (map - keys/values) ===
 *   Dersler: Fizik Matematik Programlama Veritabani
 *   Notlar:  72 85 95 88
 *
 * === Sayi Dizileri (iota) ===
 *   1-10 arasi: 1 2 3 4 5 6 7 8 9 10
 *   Cift sayilar (1-20): 2 4 6 8 10 12 14 16 18 20
 */

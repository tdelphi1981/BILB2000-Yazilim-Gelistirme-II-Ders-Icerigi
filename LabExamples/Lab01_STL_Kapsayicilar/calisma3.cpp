/**
 * Lab 01 - Çalışma 3 Başlangıç Kodu
 * map ile OBYS Prototipi
 *
 * Bu dosya Lab 01 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - std::map temel kullanımı
 * - Structured bindings (C++17)
 * - operator[], insert(), emplace()
 * - find(), at(), contains() (C++20)
 * - std::set ve set_intersection
 *
 * Lab: 01 - STL Kapsayıcılara Giriş
 * Çalışma: 3 - map ile OBYS Prototipi (30 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma3 calisma3.cpp
 * Çalıştırma: ./calisma3
 */

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>
#include <stdexcept>

struct Ogrenci {
    std::string isim;
    double not_ort;
};

int main() {
    std::map<int, Ogrenci> kayit;

    // Öğrenci ekle (numara -> bilgi)
    kayit[2001] = {"Ali Yilmaz", 3.45};
    kayit.insert({2002, {"Ayse Kaya", 3.80}});
    kayit.emplace(2003, Ogrenci{"Mehmet Demir", 2.90});

    std::cout << "=== Calisma 3: map ile OBYS Prototipi ===\n\n";

    // C++17 structured bindings ile dolaşma
    std::cout << "--- Ogrenci Kayitlari ---\n";
    for (const auto& [no, ogr] : kayit) {
        std::cout << no << ": " << ogr.isim
                  << " (Ort: " << ogr.not_ort << ")\n";
    }

    // Arama
    if (auto it = kayit.find(2002); it != kayit.end()) {
        std::cout << "\nBulundu: " << it->second.isim << '\n';
    }

    // === GOREV 3.1 - Güvenli Erişim (5 dk) ===
    std::cout << "\n[GOREV 3.1]\n";
    // TODO: a) kayit[9999] yazın ve sonucu gözlemleyin. Ne oldu?
    //          (İpucu: map'e yeni bir eleman eklendi mi?)
    //
    // TODO: b) kayit.at(9999) ile deneyin ve istisnayı yakalayın:
    //          try {
    //              auto& ogr = kayit.at(9999);
    //          } catch (const std::out_of_range& e) {
    //              std::cout << "[HATA] " << e.what() << '\n';
    //          }
    //
    // TODO: c) C++20 contains() ile kontrol edin:
    //          if (kayit.contains(2001)) { ... }
    // ============================================

    // === GOREV 3.2 - Not Güncelleme ve Filtreleme (10 dk) ===
    std::cout << "\n[GOREV 3.2]\n";
    // TODO: a) 2001 numaralı öğrencinin not ortalamasını 3.60'a güncelleyin:
    //          kayit[2001].not_ort = 3.60;
    //
    // TODO: b) Not ortalaması 3.0'ın üzerindeki öğrencileri listeleyen
    //          bir fonksiyon yazın:
    //          void basarililar(const std::map<int, Ogrenci>& kayit)
    // ============================================

    // === GOREV 3.3 - set ile Ders Kaydı (15 dk) ===
    std::cout << "\n[GOREV 3.3]\n";
    // TODO: a) Her öğrenciye set<string> ile kayıtlı ders listesi ekleyin
    //          İpucu: Yeni bir map oluşturun:
    //          std::map<int, std::set<std::string>> dersler;
    //          dersler[2001] = {"Matematik", "Fizik", "Programlama"};
    //          dersler[2002] = {"Matematik", "Kimya", "Programlama"};
    //
    // TODO: b) Aynı dersi iki kez eklemeyi deneyin -- ne olur?
    //          dersler[2001].insert("Matematik");  // Zaten var
    //
    // TODO: c) İki öğrencinin ortak derslerini bulan fonksiyon yazın:
    //          std::set<std::string> ortak;
    //          std::set_intersection(
    //              a.begin(), a.end(),
    //              b.begin(), b.end(),
    //              std::inserter(ortak, ortak.begin()));
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 3: map ile OBYS Prototipi ===

--- Ogrenci Kayitlari ---
2001: Ali Yilmaz (Ort: 3.45)
2002: Ayse Kaya (Ort: 3.8)
2003: Mehmet Demir (Ort: 2.9)

Bulundu: Ayse Kaya

[GOREV 3.1]

[GOREV 3.2]

[GOREV 3.3]
*/

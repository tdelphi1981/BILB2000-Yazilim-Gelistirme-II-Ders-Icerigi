/**
 * Lab 03 - Çalışma 4 Başlangıç Kodu
 * OBYS Entegrasyonu
 *
 * Bu dosya Lab 03 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - ranges pipeline ile sorgulama (filter + transform + take)
 * - std::optional ile güvenli arama
 * - std::variant ile polimorfik not sistemi
 * - std::visit ve overloaded deseni
 * - Tüm modern C++ tekniklerinin entegrasyonu
 *
 * Lab: 03 - Modern C++ ve STL Uygulamaları
 * Çalışma: 4 - OBYS Entegrasyonu (25 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma4 calisma4.cpp
 * Çalıştırma: ./calisma4
 */

#include <iostream>
#include <string>
#include <string_view>
#include <format>
#include <vector>
#include <map>
#include <ranges>
#include <algorithm>
#include <optional>
#include <variant>

struct Ogrenci {
    int numara;
    std::string isim;
    std::string bolum;
    double gpa;
};

// Not turu: harf, sayi veya ozel durum
using Not = std::variant<char, double, std::string>;

// Overloaded yardimcisi
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

class OBYS {
    std::vector<Ogrenci> ogrenciler_;

public:
    void ekle(Ogrenci o) {
        ogrenciler_.push_back(std::move(o));
    }

    // Tum ogrencileri dondur (const referans)
    const std::vector<Ogrenci>& tumu() const {
        return ogrenciler_;
    }

    // Numara ile arama (optional)
    std::optional<Ogrenci> bul(int numara) const {
        auto it = std::ranges::find(ogrenciler_,
                                    numara, &Ogrenci::numara);
        if (it != ogrenciler_.end()) return *it;
        return std::nullopt;
    }
};

int main() {
    std::cout << "=== Calisma 4: OBYS Entegrasyonu ===\n\n";

    OBYS sistem;
    sistem.ekle({2001, "Ali Yilmaz",    "Bilgisayar", 3.45});
    sistem.ekle({2002, "Ayse Kaya",     "Bilgisayar", 3.80});
    sistem.ekle({2003, "Mehmet Demir",  "Elektrik",   2.30});
    sistem.ekle({2004, "Zeynep Can",    "Bilgisayar", 3.60});
    sistem.ekle({2005, "Can Ozturk",    "Makine",     2.10});
    sistem.ekle({2006, "Elif Aksoy",    "Bilgisayar", 3.95});
    sistem.ekle({2007, "Burak Celik",   "Elektrik",   2.75});
    sistem.ekle({2008, "Deniz Yildiz",  "Bilgisayar", 3.20});

    // Tum ogrencileri yazdir
    std::cout << "--- Tum Ogrenciler ---\n";
    for (const auto& o : sistem.tumu()) {
        std::cout << std::format("{} - {} ({})\n",
                                 o.numara, o.isim, o.gpa);
    }

    // === GOREV 4.1 - Ranges Pipeline ile Sorgulama (10 dk) ===
    std::cout << "\n[GOREV 4.1]\n";
    // TODO: a) "Bilgisayar" bolumundeki ogrencileri views::filter
    //          ile filtreleyin:
    //          auto bilgisayar = sistem.tumu()
    //              | std::views::filter([](const Ogrenci& o) {
    //                    return o.bolum == "Bilgisayar";
    //                });
    //
    // TODO: b) Filtrelenen ogrencileri bir vector'e kopyalayin ve
    //          GPA'ya gore siralayin (ranges::sort, projeksiyon):
    //          std::vector<Ogrenci> filtrelenmis(bilgisayar.begin(),
    //                                            bilgisayar.end());
    //          std::ranges::sort(filtrelenmis, std::greater{},
    //                            &Ogrenci::gpa);
    //
    // TODO: c) views::take(3) ile ilk 3 ogrenciyi yazdir
    //          std::format ile hizali cikti olustur:
    //          "{:06} {:<20} {:>6.2f}"
    //
    // Beklenen format:
    // --- Bilgisayar En Basarili 3 ---
    //  002006 Elif Aksoy           3.95
    //  002002 Ayse Kaya            3.80
    //  002004 Zeynep Can           3.60
    // ============================================

    // === GOREV 4.2 - optional ile Arama + variant ile Notlar (10 dk) ===
    std::cout << "\n[GOREV 4.2]\n";
    // TODO: a) sistem.bul(2004) ile Zeynep Can'i arayin.
    //          Sonucu optional ile kontrol edin:
    //          auto sonuc = sistem.bul(2004);
    //          if (sonuc.has_value()) {
    //              std::cout << "Bulundu: " << sonuc->isim << '\n';
    //          }
    //
    // TODO: b) sistem.bul(9999) ile mevcut olmayan bir ogrenciyi arayin.
    //          has_value kontrolu ile "Bulunamadi" mesaji yazdirin.
    //
    // TODO: c) Her ogrenciye bir Not atayan bir map olusturun:
    //          std::map<int, Not> not_tablosu;
    //          not_tablosu[2001] = 'A';
    //          not_tablosu[2002] = 3.80;
    //          not_tablosu[2003] = std::string("Devamsiz");
    //          // En az 3 farkli not turu kullani
    //
    // TODO: d) std::visit ve overloaded deseni ile not tablosunu yazdir:
    //          for (const auto& [no, not_degeri] : not_tablosu) {
    //              std::cout << no << ": ";
    //              std::visit(overloaded{
    //                  [](char c)   { std::cout << "Harf: " << c; },
    //                  [](double d) { std::cout << "GPA: " << d; },
    //                  [](const std::string& s) {
    //                      std::cout << "Durum: " << s;
    //                  }
    //              }, not_degeri);
    //              std::cout << '\n';
    //          }
    // ============================================

    // === BONUS - Ozel Istisna Sinifi ve Hata Yonetimi (10 dk) ===
    std::cout << "\n[BONUS]\n";
    // TODO: a) std::runtime_error'dan tureyen bir
    //          OgrenciBulunamadiHatasi sinifi yazin:
    //
    //          class OgrenciBulunamadiHatasi : public std::runtime_error {
    //              int numara_;
    //          public:
    //              OgrenciBulunamadiHatasi(int no)
    //                  : std::runtime_error(
    //                      std::format("Ogrenci bulunamadi: {}", no)),
    //                    numara_(no) {}
    //              int numara() const { return numara_; }
    //          };
    //
    // TODO: b) OBYS sinifina bul_veya_hata(int numara) metodu ekleyin.
    //          Ogrenci bulunamazsa bu istisnayi firlatsin.
    //
    // TODO: c) try/catch blogu ile hem mevcut hem mevcut olmayan
    //          ogrenciyi arayin:
    //          try {
    //              auto o = sistem.bul_veya_hata(2001);
    //              std::cout << "Bulundu: " << o.isim << '\n';
    //              sistem.bul_veya_hata(9999); // istisna firlat!
    //          } catch (const OgrenciBulunamadiHatasi& e) {
    //              std::cout << "Hata: " << e.what() << '\n';
    //          }
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 4: OBYS Entegrasyonu ===

--- Tum Ogrenciler ---
2001 - Ali Yilmaz (3.45)
2002 - Ayse Kaya (3.8)
2003 - Mehmet Demir (2.3)
2004 - Zeynep Can (3.6)
2005 - Can Ozturk (2.1)
2006 - Elif Aksoy (3.95)
2007 - Burak Celik (2.75)
2008 - Deniz Yildiz (3.2)

[GOREV 4.1]

[GOREV 4.2]

[BONUS]
*/

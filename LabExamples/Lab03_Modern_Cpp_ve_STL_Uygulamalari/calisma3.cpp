/**
 * Lab 03 - Çalışma 3 Başlangıç Kodu
 * optional ve variant
 *
 * Bu dosya Lab 03 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - std::optional ile değer varlığını ifade etme
 * - has_value(), value(), value_or()
 * - std::variant ile tip-güvenli birleşim
 * - std::holds_alternative ve std::get
 * - std::visit ve overloaded deseni
 *
 * Lab: 03 - Modern C++ ve STL Uygulamaları
 * Çalışma: 3 - optional ve variant (25 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma3 calisma3.cpp
 * Çalıştırma: ./calisma3
 */

#include <iostream>
#include <string>
#include <optional>
#include <variant>
#include <vector>
#include <map>

// optional donduren arama fonksiyonu
std::optional<std::string> ogrenci_bul(
    const std::map<int, std::string>& kayitlar, int numara)
{
    auto it = kayitlar.find(numara);
    if (it != kayitlar.end()) {
        return it->second; // deger var
    }
    return std::nullopt; // deger yok
}

int main() {
    std::cout << "=== Calisma 3: optional ve variant ===\n\n";

    std::map<int, std::string> kayitlar = {
        {2001, "Ali Yilmaz"},
        {2002, "Ayse Kaya"},
        {2003, "Mehmet Demir"}
    };

    // optional kullanimi
    auto sonuc = ogrenci_bul(kayitlar, 2002);
    if (sonuc.has_value()) {
        std::cout << "Bulundu: " << sonuc.value() << '\n';
    }

    // value_or ile varsayilan deger
    auto sonuc2 = ogrenci_bul(kayitlar, 9999);
    std::cout << "Arama: "
              << sonuc2.value_or("Bulunamadi") << '\n';

    // variant: Not turlerini temsil etme
    using Not = std::variant<int, double, std::string>;

    std::vector<Not> notlar;
    notlar.push_back(85);                      // sayi notu
    notlar.push_back(3.45);                    // GPA
    notlar.push_back(std::string("Devamsiz")); // metin

    std::cout << "\n--- Notlar ---\n";
    for (const auto& n : notlar) {
        if (std::holds_alternative<int>(n)) {
            std::cout << "Sayi notu: " << std::get<int>(n) << '\n';
        } else if (std::holds_alternative<double>(n)) {
            std::cout << "GPA: " << std::get<double>(n) << '\n';
        } else {
            std::cout << "Durum: " << std::get<std::string>(n) << '\n';
        }
    }

    // === GOREV 3.1 - optional ile Guvenli Arama (10 dk) ===
    std::cout << "\n[GOREV 3.1]\n";
    // TODO: a) Ogrenci struct'i tanimlayın:
    //          struct Ogrenci { int numara; std::string isim; double gpa; };
    //
    // TODO: b) std::optional<double> donduren gpa_bul fonksiyonunu yazin.
    //          Parametre: ogrenci vektoru ve numara.
    //          Ogrenci bulunursa GPA'sini, bulunamazsa nullopt dondur.
    //
    // TODO: c) Cagri tarafinda has_value() kontrolu yapin ve
    //          bulunamayan durumda value_or(0.0) ile varsayilan
    //          deger kullanin.
    //
    // TODO: d) 3 farkli numara ile test edin (2 mevcut, 1 mevcut olmayan)
    //
    // Ipucu:
    // std::optional<double> gpa_bul(
    //     const std::vector<Ogrenci>& liste, int numara) {
    //     for (const auto& o : liste) {
    //         if (o.numara == numara) return o.gpa;
    //     }
    //     return std::nullopt;
    // }
    // ============================================

    // === GOREV 3.2 - variant ile Not Sistemi (10 dk) ===
    std::cout << "\n[GOREV 3.2]\n";
    // TODO: a) using Not2 = std::variant<char, double, std::string>;
    //          tipini tanimlayın. char harf notu ('A', 'B'),
    //          double sayisal not (0.0-4.0), string durum bilgisi
    //
    // TODO: b) 5 farkli not degeri olusturun (en az bir harf, bir
    //          sayisal, bir string)
    //
    // TODO: c) std::holds_alternative ile her notun turunu kontrol
    //          edip uygun sekilde yazdir
    // ============================================

    // === GOREV 3.3 - std::visit ile Overloaded Deseni (5 dk) ===
    std::cout << "\n[GOREV 3.3]\n";
    // TODO: a) Asagidaki overloaded yardimci yapiyi kodunuza ekleyin:
    //          template<class... Ts>
    //          struct overloaded : Ts... { using Ts::operator()...; };
    //
    // TODO: b) std::visit ile her not turu icin farkli bir cikti uretin:
    //          harf notu -> "Harf: X"
    //          sayisal   -> "GPA: X.XX"
    //          durum     -> "Durum: ..."
    //
    //          std::visit(overloaded{
    //              [](char c)   { std::cout << "Harf: " << c << '\n'; },
    //              [](double d) { std::cout << "GPA: " << d << '\n'; },
    //              [](const std::string& s) {
    //                  std::cout << "Durum: " << s << '\n';
    //              }
    //          }, not_degeri);
    //
    // TODO: c) Tum notlar uzerinde bir for dongusu ile visit cagirin
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 3: optional ve variant ===

Bulundu: Ayse Kaya
Arama: Bulunamadi

--- Notlar ---
Sayi notu: 85
GPA: 3.45
Durum: Devamsiz

[GOREV 3.1]

[GOREV 3.2]

[GOREV 3.3]
*/

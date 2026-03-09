/**
 * Lab 03 - Çalışma 2 Başlangıç Kodu
 * string_view ve std::format
 *
 * Bu dosya Lab 03 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - std::string_view ile kopyalamayan referans
 * - std::format ile tip-güvenli biçimlendirme
 * - Format belirteçleri (hizalama, sıfır doldurma, ondalık)
 * - string_view yaşam süresi tuzağı (dangling reference)
 *
 * Lab: 03 - Modern C++ ve STL Uygulamaları
 * Çalışma: 2 - string_view ve std::format (25 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma2 calisma2.cpp
 * Çalıştırma: ./calisma2
 */

#include <iostream>
#include <string>
#include <string_view>
#include <format>
#include <vector>

// string_view alan fonksiyon -- kopyalama yapmaz
void bilgi_yazdir(std::string_view baslik, std::string_view icerik) {
    std::cout << std::format("[{}] {}\n", baslik, icerik);
}

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

int main() {
    std::cout << "=== Calisma 2: string_view ve std::format ===\n\n";

    // string_view temel kullanim
    std::string ad = "Ali Yilmaz";
    std::string_view sv = ad; // kopyalamaz, gosterir
    bilgi_yazdir("Ogrenci", sv);

    // std::format ile formatlama
    std::vector<Ogrenci> liste = {
        {2001, "Ali Yilmaz",    3.45},
        {2002, "Ayse Kaya",     3.80},
        {2003, "Mehmet Demir",  2.30}
    };

    // Tablo basligi
    std::cout << std::format("\n{:<6} {:<20} {:>6}\n",
                             "No", "Isim", "GPA");
    std::cout << std::string(34, '-') << '\n';

    for (const auto& o : liste) {
        std::cout << std::format("{:<6} {:<20} {:>6.2f}\n",
                                 o.numara, o.isim, o.gpa);
    }

    // === GOREV 2.1 - string_view ile Dogrulama Fonksiyonu (10 dk) ===
    std::cout << "\n[GOREV 2.1]\n";
    // TODO: a) std::string_view parametresi alan bir eposta_gecerli_mi
    //          fonksiyonu yazin. '@' karakteri icerip icermedigini
    //          find ile kontrol edin.
    //
    // TODO: b) starts_with("ogrenci.") ile ogrenci e-postasi olup
    //          olmadigini kontrol edin (C++20).
    //
    // TODO: c) ends_with(".edu.tr") ile gecerli alan adini dogrulayin
    //          (C++20).
    //
    // TODO: d) En az 3 farkli e-posta ile test edin:
    //          eposta_gecerli_mi("ogrenci.ali@ktu.edu.tr")  -> gecerli
    //          eposta_gecerli_mi("ali@gmail.com")           -> gecersiz
    //          eposta_gecerli_mi("ogrenci.ayse")            -> gecersiz
    //
    // Ipucu:
    // bool eposta_gecerli_mi(std::string_view eposta) {
    //     return eposta.find('@') != std::string_view::npos
    //         && eposta.starts_with("ogrenci.")
    //         && eposta.ends_with(".edu.tr");
    // }
    // ============================================

    // === GOREV 2.2 - std::format ile Tablo Olusturma (10 dk) ===
    std::cout << "\n[GOREV 2.2]\n";
    // TODO: a) Tablo sutunlari: Numara (6 hane, sifir dolgulu),
    //          Isim (25 karakter, sola hizali), GPA (6 karakter,
    //          2 ondalik, saga hizali), Durum (10 karakter, ortaya hizali)
    //
    // TODO: b) GPA >= 2.0 ise durum "Basarili", degilse "Kaldi" olsun
    //
    // TODO: c) Tablo basligi ve ayirici cizgi ekleyin
    //
    // Ipucu:
    // std::format("{:06} {:<25} {:>6.2f} {:^10}\n",
    //             o.numara, o.isim, o.gpa, durum);
    // ============================================

    // === GOREV 2.3 - Dangling Reference Duzeltme (5 dk) ===
    std::cout << "\n[GOREV 2.3]\n";
    // Asagidaki kodda bir yasam suresi hatasi (dangling reference) var.
    // Hatayi bulun ve duzeltin:
    //
    // std::string_view tehlikeli_fonksiyon() {
    //     std::string gecici = "Merhaba Dunya";
    //     return gecici; // [!] Tehlikeli! Neden?
    // }
    //
    // int main_test() {
    //     auto sv = tehlikeli_fonksiyon();
    //     std::cout << sv << '\n'; // Tanimsiz davranis!
    //     return 0;
    // }
    //
    // TODO: a) Hatanin nedenini bir yorum satiri ile aciklayin
    // TODO: b) Iki farkli duzeltme yontemi onerin ve uygulayin
    //          Yontem 1: std::string dondur
    //          Yontem 2: constexpr / static string kullan
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 2: string_view ve std::format ===

[Ogrenci] Ali Yilmaz

No     Isim                    GPA
----------------------------------
2001   Ali Yilmaz               3.45
2002   Ayse Kaya                3.80
2003   Mehmet Demir             2.30

[GOREV 2.1]

[GOREV 2.2]

[GOREV 2.3]
*/

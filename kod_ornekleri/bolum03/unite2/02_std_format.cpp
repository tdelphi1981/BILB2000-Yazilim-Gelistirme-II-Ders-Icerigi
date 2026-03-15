/**
 * std::format Kullanimi - Modern C++ Metin Bicimlendirme
 *
 * Bu program C++20 ile gelen std::format kutuphanesini gosterir.
 * printf tarz bicimlendirmeyi tip-guvenli ve modern bir sekilde
 * yapmayi saglar.
 *
 * Ogrenilecek kavramlar:
 * - std::format temel kullanim: {} yer tutucular
 * - Konumsal arguman: {0}, {1}
 * - Bicimlendirme belirteceleri: {:d}, {:.2f}, {:>20}
 * - Doldurma ve hizalama: {:*>10}, {:0>8}
 * - Sayi tabanlari: {:x}, {:o}, {:b}
 * - Hizali tablolar olusturma
 * - OBYS: Ogrenci transkript raporu
 * - Ozel formatter (std::formatter)
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 2 - String Islemleri
 *
 * Derleme: g++ -std=c++20 -o 02_std_format 02_std_format.cpp
 * Calistirma: ./02_std_format
 */

#include <iostream>
#include <string>
#include <format>
#include <vector>

// --- Ozel formatter icin struct ---
struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

// std::formatter ozellemesi - Ogrenci icin
template<>
struct std::formatter<Ogrenci> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Ogrenci& o, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "[{}: {} (GPA: {:.2f})]",
                              o.numara, o.isim, o.gpa);
    }
};

int main() {
    std::cout << "=== std::format Kullanimi ===\n\n";

    // --- 1. Temel kullanim ---
    std::cout << "--- 1. Temel Kullanim ---\n";
    std::string mesaj = std::format("Merhaba, {}!", "Dunya");
    std::cout << "  " << mesaj << "\n";

    std::string bilgi = std::format("{} numarali ogrenci: {}", 1001, "Ali Yilmaz");
    std::cout << "  " << bilgi << "\n";

    // --- 2. Konumsal argumanlar ---
    std::cout << "\n--- 2. Konumsal Argumanlar ---\n";
    std::cout << "  " << std::format("{0} ve {1}", "Birinci", "Ikinci") << "\n";
    std::cout << "  " << std::format("{1} ve {0}", "Birinci", "Ikinci") << "\n";
    std::cout << "  " << std::format("{0}-{0}-{1}", "Ah", "Be") << "\n";

    // --- 3. Bicimlendirme belirteceleri ---
    std::cout << "\n--- 3. Bicimlendirme Belirteceleri ---\n";
    double gpa = 3.14159;
    std::cout << "  " << std::format("GPA (2 basamak): {:.2f}", gpa) << "\n";
    std::cout << "  " << std::format("GPA (4 basamak): {:.4f}", gpa) << "\n";
    std::cout << "  " << std::format("Tamsayi: {:d}", 42) << "\n";

    // --- 4. Hizalama ---
    std::cout << "\n--- 4. Hizalama ---\n";
    std::cout << "  " << std::format("|{:<20}|", "Sola") << "\n";
    std::cout << "  " << std::format("|{:^20}|", "Ortaya") << "\n";
    std::cout << "  " << std::format("|{:>20}|", "Saga") << "\n";

    // --- 5. Doldurma karakterleri ---
    std::cout << "\n--- 5. Doldurma Karakterleri ---\n";
    std::cout << "  " << std::format("Yildiz: {:*>10}", 42) << "\n";
    std::cout << "  " << std::format("Sifir:  {:0>8}", 42) << "\n";
    std::cout << "  " << std::format("Tire:   {:-^20}", "BASLIK") << "\n";

    // --- 6. Sayi tabanlari ---
    std::cout << "\n--- 6. Sayi Tabanlari ---\n";
    int deger = 255;
    std::cout << "  " << std::format("Onluk:   {:d}", deger) << "\n";
    std::cout << "  " << std::format("Onalti:  {:x}", deger) << "\n";
    std::cout << "  " << std::format("Sekizli: {:o}", deger) << "\n";
    std::cout << "  " << std::format("Ikilik:  {:b}", deger) << "\n";
    std::cout << "  " << std::format("Onekli:  {:#x} / {:#o} / {:#b}", deger, deger, deger) << "\n";

    // --- 7. Hizali tablo ---
    std::cout << "\n--- 7. Hizali Tablo ---\n";
    std::cout << std::format("  {:>5} | {:<15} | {:>6}\n", "No", "Isim", "GPA");
    std::cout << std::format("  {:->5}-+-{:-<15}-+-{:->6}\n", "", "", "");
    std::cout << std::format("  {:>5} | {:<15} | {:>6.2f}\n", 1001, "Ali Yilmaz", 3.45);
    std::cout << std::format("  {:>5} | {:<15} | {:>6.2f}\n", 1002, "Ayse Kaya", 3.80);
    std::cout << std::format("  {:>5} | {:<15} | {:>6.2f}\n", 1003, "Mehmet Demir", 2.90);

    // --- 8. OBYS: Transkript raporu ---
    std::cout << "\n--- 8. OBYS: Transkript Raporu ---\n";
    struct Ders {
        std::string kod;
        std::string ad;
        int kredi;
        double not_degeri;
        std::string harf;
    };

    std::vector<Ders> dersler = {
        {"BILB1001", "Programlama I",    4, 3.50, "BA"},
        {"BILB1003", "Matematik I",      4, 2.75, "CB"},
        {"BILB1005", "Fizik I",          3, 3.00, "BB"},
        {"BILB2000", "Yazilim Gel. II",  3, 4.00, "AA"},
    };

    std::cout << std::format("  Ogrenci: {} - {}\n", 1001, "Ali Yilmaz");
    std::cout << std::format("  {:=^50}\n", " TRANSKRIPT ");
    std::cout << std::format("  {:<10} {:<18} {:>5} {:>5} {:>4}\n",
                             "Kod", "Ders Adi", "Kredi", "Not", "Harf");
    std::cout << std::format("  {:-<50}\n", "");

    double toplam_agirlik = 0.0, toplam_kredi = 0.0;
    for (const auto& d : dersler) {
        std::cout << std::format("  {:<10} {:<18} {:>5} {:>5.2f} {:>4}\n",
                                 d.kod, d.ad, d.kredi, d.not_degeri, d.harf);
        toplam_agirlik += d.not_degeri * d.kredi;
        toplam_kredi += d.kredi;
    }
    std::cout << std::format("  {:-<50}\n", "");
    std::cout << std::format("  {:>35} {:.2f}\n", "Donem GPA:",
                             toplam_agirlik / toplam_kredi);

    // --- 9. Ozel formatter ---
    std::cout << "\n--- 9. Ozel Formatter ---\n";
    Ogrenci o1{1001, "Ali Yilmaz", 3.45};
    Ogrenci o2{1002, "Ayse Kaya", 3.80};
    std::cout << "  " << std::format("Ogrenci: {}", o1) << "\n";
    std::cout << "  " << std::format("Karsilastir: {} vs {}", o1, o2) << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === std::format Kullanimi ===
 *
 * --- 1. Temel Kullanim ---
 *   Merhaba, Dunya!
 *   1001 numarali ogrenci: Ali Yilmaz
 *
 * --- 2. Konumsal Argumanlar ---
 *   Birinci ve Ikinci
 *   Ikinci ve Birinci
 *   Ah-Ah-Be
 *
 * --- 3. Bicimlendirme Belirteceleri ---
 *   GPA (2 basamak): 3.14
 *   GPA (4 basamak): 3.1416
 *   Tamsayi: 42
 *
 * --- 4. Hizalama ---
 *   |Sola                |
 *   |       Ortaya       |
 *   |                Saga|
 *
 * --- 5. Doldurma Karakterleri ---
 *   Yildiz: ********42
 *   Sifir:  00000042
 *   Tire:   -------BASLIK-------
 *
 * --- 6. Sayi Tabanlari ---
 *   Onluk:   255
 *   Onalti:  ff
 *   Sekizli: 377
 *   Ikilik:  11111111
 *   Onekli:  0xff / 0377 / 0b11111111
 *
 * --- 7. Hizali Tablo ---
 *      No | Isim            |    GPA
 *   ------+-----------------+-------
 *    1001 | Ali Yilmaz      |   3.45
 *    1002 | Ayse Kaya       |   3.80
 *    1003 | Mehmet Demir    |   2.90
 *
 * --- 8. OBYS: Transkript Raporu ---
 *   Ogrenci: 1001 - Ali Yilmaz
 *   =================== TRANSKRIPT ===================
 *   Kod        Ders Adi           Kredi   Not Harf
 *   --------------------------------------------------
 *   BILB1001   Programlama I          4  3.50   BA
 *   BILB1003   Matematik I            4  2.75   CB
 *   BILB1005   Fizik I                3  3.00   BB
 *   BILB2000   Yazilim Gel. II        3  4.00   AA
 *   --------------------------------------------------
 *                              Donem GPA: 3.29
 *
 * --- 9. Ozel Formatter ---
 *   Ogrenci: [1001: Ali Yilmaz (GPA: 3.45)]
 *   Karsilastir: [1001: Ali Yilmaz (GPA: 3.45)] vs [1002: Ayse Kaya (GPA: 3.80)]
 */

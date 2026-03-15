/**
 * Variant ve Any - Tip-Guvenli Birlesim Turleri
 *
 * Bu program, std::variant (tip-guvenli union) ve std::any (tip-silme
 * ile herhangi bir deger) turlerini karsilastirir. OBYS baglaminda
 * farkli not turlerini temsil eden DersNotu ornegi sunar.
 *
 * Ogrenilecek kavramlar:
 * - std::variant olusturma ve deger atama
 * - get<T>(), get<index>(), get_if<T>() erisim kaliplari
 * - std::visit ve lambda ziyaretciler
 * - overloaded kalip deseni
 * - std::any temelleri ve any_cast
 * - variant vs any vs void* karsilastirmasi
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 3 - Hata Yonetimi
 *
 * Derleme: g++ -std=c++20 -o 02_variant_ve_any 02_variant_ve_any.cpp
 * Calistirma: ./02_variant_ve_any
 */

#include <iostream>
#include <variant>
#include <any>
#include <string>
#include <vector>
#include <iomanip>

// -- overloaded kalip deseni --
// Birden fazla lambda'yi tek bir cagrilabilir nesnede birlestirir
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

// -- OBYS: Muaf durumu icin bos yapi --
struct Muaf {
    std::string sebep;
};

// -- OBYS: DersNotu - farkli not turleri --
// Bir ogrencinin notu: sayi (int), harf (string) veya muaf olabilir
using DersNotu = std::variant<int, std::string, Muaf>;

// DersNotu'nu okunabilir metne ceviren fonksiyon
std::string not_aciklama(const DersNotu& not_degeri) {
    return std::visit(overloaded{
        [](int sayi) -> std::string {
            return "Sayi notu: " + std::to_string(sayi);
        },
        [](const std::string& harf) -> std::string {
            return "Harf notu: " + harf;
        },
        [](const Muaf& m) -> std::string {
            return "Muaf (" + m.sebep + ")";
        }
    }, not_degeri);
}

int main() {
    // =============================================
    // 1) variant Olusturma ve Atama
    // =============================================
    std::cout << "=== variant Olusturma ===\n";

    std::variant<int, std::string, double> v1 = 42;
    std::cout << "  v1 (int): " << std::get<int>(v1) << "\n";

    v1 = "Merhaba";  // artik string tutuyor
    std::cout << "  v1 (string): " << std::get<std::string>(v1) << "\n";

    v1 = 3.14;       // artik double tutuyor
    std::cout << "  v1 (double): " << std::get<double>(v1) << "\n";

    // Aktif indeks
    std::cout << "  Aktif indeks: " << v1.index() << "\n";

    // =============================================
    // 2) Erisim Yollari
    // =============================================
    std::cout << "\n=== Erisim Yollari ===\n";

    std::variant<int, std::string> v2 = 100;

    // get<index>
    std::cout << "  get<0>: " << std::get<0>(v2) << "\n";

    // get_if<T> - basarisizsa nullptr doner
    if (auto p = std::get_if<int>(&v2)) {
        std::cout << "  get_if<int>: " << *p << "\n";
    }
    if (auto p = std::get_if<std::string>(&v2)) {
        std::cout << "  Bu satir calismaz\n";
    } else {
        std::cout << "  get_if<string>: nullptr (aktif tur degil)\n";
    }

    // bad_variant_access
    try {
        auto s = std::get<std::string>(v2);  // hatali erisim!
        std::cout << s << "\n";
    } catch (const std::bad_variant_access& e) {
        std::cout << "  Hatali erisim: " << e.what() << "\n";
    }

    // =============================================
    // 3) std::visit ve overloaded Kalip
    // =============================================
    std::cout << "\n=== OBYS: DersNotu Ornegi ===\n";

    std::vector<std::pair<std::string, DersNotu>> notlar = {
        {"Ayse Yilmaz",  85},
        {"Mehmet Demir",  std::string("BB")},
        {"Zeynep Kara",   Muaf{"Yatay gecis"}},
        {"Ali Ozturk",   72}
    };

    for (const auto& [isim, not_d] : notlar) {
        std::cout << "  " << std::setw(16) << std::left << isim
                  << " -> " << not_aciklama(not_d) << "\n";
    }

    // =============================================
    // 4) std::any Temelleri
    // =============================================
    std::cout << "\n=== std::any Temelleri ===\n";

    std::any a = 42;
    std::cout << "  Tur: " << a.type().name() << ", Deger: "
              << std::any_cast<int>(a) << "\n";

    a = std::string("STL");
    std::cout << "  Tur: " << a.type().name() << ", Deger: "
              << std::any_cast<std::string>(a) << "\n";

    a = 3.14;
    std::cout << "  Tur: " << a.type().name() << ", Deger: "
              << std::any_cast<double>(a) << "\n";

    // bad_any_cast
    try {
        auto val = std::any_cast<int>(a);  // a simdi double!
        std::cout << val << "\n";
    } catch (const std::bad_any_cast& e) {
        std::cout << "  Hatali any_cast: " << e.what() << "\n";
    }

    // has_value ve reset
    std::cout << "  Dolu mu? " << (a.has_value() ? "Evet" : "Hayir") << "\n";
    a.reset();
    std::cout << "  Reset sonrasi dolu mu? "
              << (a.has_value() ? "Evet" : "Hayir") << "\n";

    // =============================================
    // 5) Karsilastirma Tablosu
    // =============================================
    std::cout << "\n=== variant vs any vs void* ===\n";
    std::cout << std::setw(20) << std::left << "  Ozellik"
              << std::setw(14) << "variant"
              << std::setw(14) << "any"
              << "void*\n";
    std::cout << "  " << std::string(54, '-') << "\n";
    std::cout << std::setw(20) << "  Tip guvenli"
              << std::setw(14) << "Evet"
              << std::setw(14) << "Kismi"
              << "Hayir\n";
    std::cout << std::setw(20) << "  Heap tahsisi"
              << std::setw(14) << "Hayir"
              << std::setw(14) << "Olabilir"
              << "Manuel\n";
    std::cout << std::setw(20) << "  Tur listesi"
              << std::setw(14) << "Sabit"
              << std::setw(14) << "Herhangi"
              << "Herhangi\n";
    std::cout << std::setw(20) << "  visit destegi"
              << std::setw(14) << "Evet"
              << std::setw(14) << "Hayir"
              << "Hayir\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === variant Olusturma ===
 *   v1 (int): 42
 *   v1 (string): Merhaba
 *   v1 (double): 3.14
 *   Aktif indeks: 2
 *
 * === Erisim Yollari ===
 *   get<0>: 100
 *   get_if<int>: 100
 *   get_if<string>: nullptr (aktif tur degil)
 *   Hatali erisim: bad_variant_access
 *
 * === OBYS: DersNotu Ornegi ===
 *   Ayse Yilmaz      -> Sayi notu: 85
 *   Mehmet Demir      -> Harf notu: BB
 *   Zeynep Kara       -> Muaf (Yatay gecis)
 *   Ali Ozturk        -> Sayi notu: 72
 *
 * === std::any Temelleri ===
 *   Tur: i, Deger: 42           (not: tur isimleri derleyiciye baglidir)
 *   Tur: ..., Deger: STL
 *   Tur: d, Deger: 3.14
 *   Hatali any_cast: bad any cast
 *   Dolu mu? Evet
 *   Reset sonrasi dolu mu? Hayir
 *
 * === variant vs any vs void* ===
 *   Ozellik           variant       any           void*
 *   ------------------------------------------------------
 *   Tip guvenli       Evet          Kismi         Hayir
 *   Heap tahsisi      Hayir         Olabilir      Manuel
 *   Tur listesi       Sabit         Herhangi      Herhangi
 *   visit destegi     Evet          Hayir         Hayir
 */

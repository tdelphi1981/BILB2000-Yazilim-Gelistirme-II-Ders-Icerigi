/**
 * std::function ve Jenerik Lambda
 *
 * Bu program std::function, auto parametreli lambda ve
 * C++20 template lambda kullanimini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - std::function ile tur silinmis cagrilabilir sarma
 * - Fonksiyon gostericisi vs lambda vs std::function
 * - auto parametreli (jenerik) lambda (C++14)
 * - Template lambda (C++20)
 * - Bilesik (composable) predikatlar
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 4 - Lambda Ifadeleri
 *
 * Derleme: g++ -std=c++20 -o 03_std_function_jenerik 03_std_function_jenerik.cpp
 * Calistirma: ./03_std_function_jenerik
 */

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <string>
#include <map>
#include <iomanip>
#include <type_traits>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
    std::string bolum;
};

int main() {
    std::cout << "=== std::function ve Jenerik Lambda ===\n\n";

    // --- 1. std::function temelleri ---
    std::cout << "--- std::function ---\n";

    // Herhangi bir cagrilabilir nesneyi sarar
    std::function<int(int, int)> islem;

    // Lambda atama
    islem = [](int a, int b) { return a + b; };
    std::cout << "Toplama: " << islem(3, 5) << "\n";

    // Farkli lambda atama (ayni imza)
    islem = [](int a, int b) { return a * b; };
    std::cout << "Carpma:  " << islem(3, 5) << "\n";

    // Serbest fonksiyon da atanabilir
    islem = std::multiplies<int>();
    std::cout << "Functor: " << islem(3, 5) << "\n";

    // --- 2. std::function ile strateji deseni ---
    std::cout << "\n--- Strateji Deseni ---\n";

    using Karsilastirici = std::function<bool(const Ogrenci&, const Ogrenci&)>;

    // Farkli siralama stratejileri
    std::map<std::string, Karsilastirici> stratejiler;

    stratejiler["gpa_artan"] = [](const Ogrenci& a, const Ogrenci& b) {
        return a.gpa < b.gpa;
    };
    stratejiler["gpa_azalan"] = [](const Ogrenci& a, const Ogrenci& b) {
        return a.gpa > b.gpa;
    };
    stratejiler["isim"] = [](const Ogrenci& a, const Ogrenci& b) {
        return a.isim < b.isim;
    };
    stratejiler["numara"] = [](const Ogrenci& a, const Ogrenci& b) {
        return a.numara < b.numara;
    };

    std::vector<Ogrenci> sinif = {
        {1003, "Mehmet Demir", 2.90, "Elektrik"},
        {1001, "Ali Yilmaz",  3.45, "Bilgisayar"},
        {1002, "Ayse Kaya",   3.80, "Bilgisayar"},
        {1004, "Zeynep Can",  3.65, "Bilgisayar"}
    };

    // Kullanicinin sectigi stratejiyi uygula
    std::string secim = "gpa_azalan";
    std::sort(sinif.begin(), sinif.end(), stratejiler[secim]);

    std::cout << "Siralama: " << secim << "\n";
    for (const auto& o : sinif) {
        std::cout << "  " << o.numara << ": " << std::left
                  << std::setw(15) << o.isim
                  << " GPA: " << std::fixed << std::setprecision(2)
                  << o.gpa << "\n";
    }

    // --- 3. auto parametreli (jenerik) lambda ---
    std::cout << "\n--- Jenerik Lambda (C++14) ---\n";

    auto yazdir = [](const auto& kapsayici, const std::string& baslik) {
        std::cout << baslik << ": ";
        for (const auto& e : kapsayici) {
            std::cout << e << " ";
        }
        std::cout << "\n";
    };

    std::vector<int> sayilar = {1, 2, 3, 4, 5};
    std::vector<std::string> isimler = {"Ali", "Ayse", "Mehmet"};

    yazdir(sayilar, "Sayilar");   // auto = vector<int>
    yazdir(isimler, "Isimler");   // auto = vector<string>

    // Jenerik max fonksiyonu
    auto jenerik_max = [](const auto& a, const auto& b) {
        return (a > b) ? a : b;
    };

    std::cout << "max(3, 7) = " << jenerik_max(3, 7) << "\n";
    std::cout << "max(3.14, 2.71) = " << jenerik_max(3.14, 2.71) << "\n";
    std::cout << "max(\"abc\", \"xyz\") = "
              << jenerik_max(std::string("abc"), std::string("xyz")) << "\n";

    // --- 4. Template lambda (C++20) ---
    std::cout << "\n--- Template Lambda (C++20) ---\n";

    // auto yetersiz kaldiginda: tur bilgisine erisim gerektiginde
    auto boyut_yazdir = []<typename T>(const std::vector<T>& v) {
        std::cout << "Eleman sayisi: " << v.size()
                  << ", sizeof(T): " << sizeof(T) << " byte\n";
    };

    std::vector<int> vi = {1, 2, 3};
    std::vector<double> vd = {1.1, 2.2, 3.3};

    boyut_yazdir(vi);
    boyut_yazdir(vd);

    // Template lambda ile kisitlama (concepts)
    auto sayi_topla = []<typename T>(const std::vector<T>& v) -> T {
        static_assert(std::is_arithmetic_v<T>, "Aritmetik tur gerekli");
        T toplam = T{};
        for (const auto& e : v) toplam += e;
        return toplam;
    };

    std::cout << "int toplam: " << sayi_topla(vi) << "\n";
    std::cout << "double toplam: " << sayi_topla(vd) << "\n";
    // sayi_topla(isimler);  // DERLEME HATASI! string aritmetik degil

    // --- 5. Bilesik predikatlar ---
    std::cout << "\n--- Bilesik Predikatlar ---\n";

    using Predikat = std::function<bool(const Ogrenci&)>;

    // Temel predikatlar
    auto gpa_filtre = [](double esik) -> Predikat {
        return [esik](const Ogrenci& o) { return o.gpa >= esik; };
    };

    auto bolum_filtre = [](const std::string& bolum) -> Predikat {
        return [bolum](const Ogrenci& o) { return o.bolum == bolum; };
    };

    // VE bilesimi
    auto ve = [](Predikat p1, Predikat p2) -> Predikat {
        return [p1, p2](const Ogrenci& o) { return p1(o) && p2(o); };
    };

    // VEYA bilesimi
    auto veya = [](Predikat p1, Predikat p2) -> Predikat {
        return [p1, p2](const Ogrenci& o) { return p1(o) || p2(o); };
    };

    // DEGIL
    auto degil = [](Predikat p) -> Predikat {
        return [p](const Ogrenci& o) { return !p(o); };
    };

    // Bilesik sorgu: Bilgisayar bolumu VE GPA >= 3.5
    std::vector<Ogrenci> sinif2 = {
        {1001, "Ali Yilmaz",   3.45, "Bilgisayar"},
        {1002, "Ayse Kaya",    3.80, "Bilgisayar"},
        {1003, "Mehmet Demir", 2.90, "Elektrik"},
        {1004, "Zeynep Can",   3.65, "Bilgisayar"},
        {1005, "Can Arslan",   2.10, "Makine"},
        {1006, "Elif Yildiz",  3.92, "Bilgisayar"}
    };

    auto sorgu = ve(bolum_filtre("Bilgisayar"), gpa_filtre(3.5));

    std::cout << "Bilgisayar + GPA >= 3.5:\n";
    for (const auto& o : sinif2) {
        if (sorgu(o)) {
            std::cout << "  " << o.isim << " (GPA: " << o.gpa << ")\n";
        }
    }

    // Farkli sorgu: Bilgisayar DEGIL
    auto sorgu2 = degil(bolum_filtre("Bilgisayar"));
    std::cout << "\nBilgisayar DEGIL:\n";
    for (const auto& o : sinif2) {
        if (sorgu2(o)) {
            std::cout << "  " << o.isim << " (" << o.bolum << ")\n";
        }
    }

    // --- 6. Higher-order function ---
    std::cout << "\n--- Higher-Order Function ---\n";

    // Lambda donduren lambda (fabrika deseni)
    auto carpan_olustur = [](int faktor) {
        return [faktor](int x) { return x * faktor; };
    };

    auto ikiyle_carp = carpan_olustur(2);
    auto ucle_carp = carpan_olustur(3);

    std::cout << "2 * 5 = " << ikiyle_carp(5) << "\n";
    std::cout << "3 * 5 = " << ucle_carp(5) << "\n";

    // Transform ile kullan
    std::vector<int> degerler = {1, 2, 3, 4, 5};
    std::transform(degerler.begin(), degerler.end(),
                   degerler.begin(), ikiyle_carp);

    std::cout << "2 ile carpilmis: ";
    for (int v : degerler) std::cout << v << " ";
    std::cout << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === std::function ve Jenerik Lambda ===
 *
 * --- std::function ---
 * Toplama: 8
 * Carpma:  15
 * Functor: 15
 *
 * --- Strateji Deseni ---
 * Siralama: gpa_azalan
 *   1002: Ayse Kaya       GPA: 3.80
 *   1004: Zeynep Can      GPA: 3.65
 *   1001: Ali Yilmaz      GPA: 3.45
 *   1003: Mehmet Demir    GPA: 2.90
 *
 * --- Jenerik Lambda (C++14) ---
 * Sayilar: 1 2 3 4 5
 * Isimler: Ali Ayse Mehmet
 * max(3, 7) = 7
 * max(3.14, 2.71) = 3.14
 * max("abc", "xyz") = xyz
 *
 * --- Template Lambda (C++20) ---
 * Eleman sayisi: 3, sizeof(T): 4 byte
 * Eleman sayisi: 3, sizeof(T): 8 byte
 * int toplam: 6
 * double toplam: 6.6
 *
 * --- Bilesik Predikatlar ---
 * Bilgisayar + GPA >= 3.5:
 *   Ayse Kaya (GPA: 3.8)
 *   Zeynep Can (GPA: 3.65)
 *   Elif Yildiz (GPA: 3.92)
 *
 * Bilgisayar DEGIL:
 *   Mehmet Demir (Elektrik)
 *   Can Arslan (Makine)
 *
 * --- Higher-Order Function ---
 * 2 * 5 = 10
 * 3 * 5 = 15
 * 2 ile carpilmis: 2 4 6 8 10
 */

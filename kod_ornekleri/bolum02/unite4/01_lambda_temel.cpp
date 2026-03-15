/**
 * Lambda Temelleri - Sozdizimi ve Temel Kullanim
 *
 * Bu program lambda ifadelerinin temel sozdizimini ve kullanimini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - Lambda sozdizimi: [capture](params) -> return_type { body }
 * - Donus tipi cikarimi
 * - mutable lambda
 * - constexpr / consteval lambda
 * - IIFE (Immediately Invoked Function Expression)
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 4 - Lambda Ifadeleri
 *
 * Derleme: g++ -std=c++20 -o 01_lambda_temel 01_lambda_temel.cpp
 * Calistirma: ./01_lambda_temel
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>

int main() {
    std::cout << "=== Lambda Temelleri ===\n\n";

    // --- 1. En basit lambda ---
    std::cout << "--- Basit Lambda ---\n";
    auto merhaba = []() { std::cout << "Merhaba, Lambda!\n"; };
    merhaba();  // Lambda'yi cagir

    // --- 2. Parametreli lambda ---
    std::cout << "\n--- Parametreli Lambda ---\n";
    auto topla = [](int a, int b) { return a + b; };
    std::cout << "3 + 5 = " << topla(3, 5) << "\n";

    // Birden fazla parametre
    auto bilgi = [](const std::string& isim, double gpa) {
        std::cout << isim << " - GPA: " << std::fixed
                  << std::setprecision(2) << gpa << "\n";
    };
    bilgi("Ali Yilmaz", 3.45);

    // --- 3. Donus tipi belirtme ---
    std::cout << "\n--- Donus Tipi ---\n";

    // Otomatik cikarim
    auto kare = [](int x) { return x * x; };
    std::cout << "5^2 = " << kare(5) << "\n";

    // Acik donus tipi (gerektiginde)
    auto bol = [](int a, int b) -> double {
        return static_cast<double>(a) / b;
    };
    std::cout << "7 / 2 = " << bol(7, 2) << "\n";

    // --- 4. Lambda ile algoritmalar ---
    std::cout << "\n--- Algoritmalarla Kullanim ---\n";
    std::vector<int> notlar = {85, 92, 78, 95, 88, 72, 91};

    // sort ile ozel siralama
    std::sort(notlar.begin(), notlar.end(),
              [](int a, int b) { return a > b; });
    std::cout << "Azalan sira: ";
    for (int n : notlar) std::cout << n << " ";
    std::cout << "\n";

    // count_if ile sayma
    int yuksek = std::count_if(notlar.begin(), notlar.end(),
                                [](int n) { return n >= 90; });
    std::cout << "90 ve ustu: " << yuksek << " adet\n";

    // find_if ile arama
    auto it = std::find_if(notlar.begin(), notlar.end(),
                            [](int n) { return n < 80; });
    if (it != notlar.end()) {
        std::cout << "80'den kucuk ilk not: " << *it << "\n";
    }

    // for_each ile islem
    std::cout << "Notlar (+5 egri): ";
    std::for_each(notlar.begin(), notlar.end(),
                  [](int n) { std::cout << std::min(n + 5, 100) << " "; });
    std::cout << "\n";

    // --- 5. mutable lambda ---
    std::cout << "\n--- mutable Lambda ---\n";

    int sayac_degeri = 0;

    // Deger ile yakalanan degiskenler varsayilan olarak const'tur
    // mutable ile degistirilebilir (kopya uzerinde)
    auto sayac = [sayac_degeri]() mutable -> int {
        return ++sayac_degeri;
    };

    std::cout << "Sayac: " << sayac() << "\n";  // 1
    std::cout << "Sayac: " << sayac() << "\n";  // 2
    std::cout << "Sayac: " << sayac() << "\n";  // 3
    std::cout << "Orijinal deger: " << sayac_degeri << " (degismedi)\n";

    // --- 6. noexcept lambda ---
    std::cout << "\n--- noexcept Lambda ---\n";
    auto guvenli_topla = [](int a, int b) noexcept {
        return a + b;
    };
    std::cout << "noexcept toplam: " << guvenli_topla(10, 20) << "\n";

    // --- 7. constexpr lambda (C++17) ---
    std::cout << "\n--- constexpr Lambda ---\n";
    constexpr auto kare_ce = [](int x) constexpr { return x * x; };
    constexpr int sonuc = kare_ce(6);  // Derleme zamaninda hesaplanir
    std::cout << "constexpr 6^2 = " << sonuc << "\n";

    // --- 8. IIFE (Immediately Invoked Function Expression) ---
    std::cout << "\n--- IIFE ---\n";

    // Karmasik baslatma icin kullanisli
    const auto mesaj = [&]() -> std::string {
        int toplam_not = 0;
        for (int n : notlar) toplam_not += n;
        double ort = static_cast<double>(toplam_not) / notlar.size();
        if (ort >= 85.0) return "Harika sinif!";
        if (ort >= 70.0) return "Iyi sinif.";
        return "Gelistirme gerekli.";
    }();  // Hemen cagrilir!

    std::cout << "Sinif degerlendirmesi: " << mesaj << "\n";

    // --- 9. Lambda turu ---
    std::cout << "\n--- Lambda Turu ---\n";
    auto f1 = [](int x) { return x; };
    auto f2 = [](int x) { return x; };

    // Her lambda BENZERSIZ bir ture sahip
    // f1 ve f2 ayni isi yapar ama turleri FARKLI
    // f1 = f2;  // DERLEME HATASI!

    std::cout << "Her lambda benzersiz bir closure turune sahiptir.\n";
    std::cout << "sizeof(f1) = " << sizeof(f1) << " byte\n";
    std::cout << "sizeof(f2) = " << sizeof(f2) << " byte\n";

    // --- 10. OBYS: Lambda ile filtreleme ---
    std::cout << "\n--- OBYS: Lambda Filtreleme ---\n";

    struct Ogrenci {
        int numara;
        std::string isim;
        double gpa;
    };

    std::vector<Ogrenci> sinif = {
        {1001, "Ali Yilmaz",   3.45},
        {1002, "Ayse Kaya",    3.80},
        {1003, "Mehmet Demir", 2.90},
        {1004, "Zeynep Can",   3.65},
        {1005, "Can Arslan",   2.10}
    };

    // GPA esigini disardan al
    double esik = 3.0;

    // Lambda yakalama ile esik kullan
    auto basarili_mi = [esik](const Ogrenci& o) { return o.gpa >= esik; };

    std::cout << "Basarili ogrenciler (GPA >= " << esik << "):\n";
    for (const auto& o : sinif) {
        if (basarili_mi(o)) {
            std::cout << "  " << o.isim << " (" << o.gpa << ")\n";
        }
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Lambda Temelleri ===
 *
 * --- Basit Lambda ---
 * Merhaba, Lambda!
 *
 * --- Parametreli Lambda ---
 * 3 + 5 = 8
 * Ali Yilmaz - GPA: 3.45
 *
 * --- Donus Tipi ---
 * 5^2 = 25
 * 7 / 2 = 3.50
 *
 * --- Algoritmalarla Kullanim ---
 * Azalan sira: 95 92 91 88 85 78 72
 * 90 ve ustu: 3 adet
 * 80'den kucuk ilk not: 78
 * Notlar (+5 egri): 100 97 96 93 90 83 77
 *
 * --- mutable Lambda ---
 * Sayac: 1
 * Sayac: 2
 * Sayac: 3
 * Orijinal deger: 0 (degismedi)
 *
 * --- noexcept Lambda ---
 * noexcept toplam: 30
 *
 * --- constexpr Lambda ---
 * constexpr 6^2 = 36
 *
 * --- IIFE ---
 * Sinif degerlendirmesi: Harika sinif!
 *
 * --- Lambda Turu ---
 * Her lambda benzersiz bir closure turune sahiptir.
 * sizeof(f1) = 1 byte
 * sizeof(f2) = 1 byte
 *
 * --- OBYS: Lambda Filtreleme ---
 * Basarili ogrenciler (GPA >= 3.00):
 *   Ali Yilmaz (3.45)
 *   Ayse Kaya (3.80)
 *   Zeynep Can (3.65)
 */

/**
 * Lab 02 - Çalışma 4 Başlangıç Kodu
 * Lambda ve İleri Algoritmalar
 *
 * Bu dosya Lab 02 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - Lambda yakalama modları (değer, referans, mutable)
 * - std::count_if ile koşullu sayma
 * - std::accumulate ile toplama/ortalama
 * - std::inner_product ile ağırlıklı hesaplama
 * - std::partition ve stable_partition
 * - std::function ile birleştirilebilir filtreler
 * - IIFE (Immediately Invoked Function Expression)
 *
 * Lab: 02 - İteratörler ve STL Algoritmaları
 * Çalışma: 4 - Lambda ve İleri Algoritmalar (30 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma4 calisma4.cpp
 * Çalıştırma: ./calisma4
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>
#include <map>

struct Ogrenci {
    std::string isim;
    double gpa;
};

int main() {
    std::vector<Ogrenci> liste = {
        {"Ali", 3.45}, {"Ayse", 3.80}, {"Mehmet", 2.90},
        {"Zeynep", 3.60}, {"Can", 2.10}, {"Elif", 3.95}
    };

    std::cout << "=== Calisma 4: Lambda ve Ileri Algoritmalar ===\n\n";

    // Lambda ile GPA eşik değeri dışarıdan yakalama
    double esik = 3.0;
    auto basarili_mi = [esik](const Ogrenci& o) {
        return o.gpa >= esik;
    };

    int basarili_sayisi = std::count_if(
        liste.begin(), liste.end(), basarili_mi);
    std::cout << "GPA >= " << esik << ": "
              << basarili_sayisi << " ogrenci\n";

    // === GOREV 4.1 - Yakalama Modları (10 dk) ===
    std::cout << "\n[GOREV 4.1]\n";
    // TODO: a) Değerle yakalama: esik değişkenini lambda içinde
    //          değiştirmeye çalışın. Ne olur?
    //          auto test1 = [esik]() { esik = 3.5; };  // [HATA]!
    //          mutable ekleyerek tekrar deneyin:
    //          auto test2 = [esik]() mutable { esik = 3.5; };  // [OK]
    //
    // TODO: b) Referansla yakalama: [&esik] ile yakalayın ve lambda
    //          içinde esik = 3.5; yazın. Lambda çağrısından sonra
    //          esik değeri ne olur?
    //
    // TODO: c) Bir sayaç lambdası yazın: Her çağrısında değeri 1 artan:
    //          int sayac = 0;
    //          auto artir = [sayac]() mutable { return ++sayac; };
    //          std::cout << artir() << '\n';  // 1
    //          std::cout << artir() << '\n';  // 2
    // ============================================

    // === GOREV 4.2 - accumulate ile GPA Hesaplama (5 dk) ===
    std::cout << "\n[GOREV 4.2]\n";
    // TODO: std::accumulate kullanarak tüm öğrencilerin GPA
    //       ortalamasını hesaplayın:
    //
    //       double toplam = std::accumulate(liste.begin(), liste.end(),
    //           0.0, [](double t, const Ogrenci& o) {
    //               return t + o.gpa;
    //           });
    //       double ortalama = toplam / liste.size();
    //       std::cout << "\nGPA Ortalamasi: " << ortalama << '\n';
    //
    //       [!] Başlangıç değeri olarak 0.0 (double) yazın, 0 (int) değil!
    // ============================================

    // === GOREV 4.3 - inner_product ile Ağırlıklı GNO (10 dk) ===
    std::cout << "\n[GOREV 4.3]\n";
    // TODO: a) OBYS'de ağırlıklı GNO hesaplayın:
    //          std::vector<int> krediler = {3, 4, 3, 2};
    //          std::vector<double> ders_notlari = {3.5, 2.8, 3.9, 4.0};
    //
    //          double agirlikli = std::inner_product(
    //              krediler.begin(), krediler.end(),
    //              ders_notlari.begin(), 0.0);
    //          int toplam_kredi = std::accumulate(
    //              krediler.begin(), krediler.end(), 0);
    //          double gno = agirlikli / toplam_kredi;
    //          std::cout << "GNO: " << gno << '\n';
    //
    // TODO: b) Bir öğrencinin daha notlarını ekleyin ve GNO'yu
    //          yeniden hesaplayın
    //
    // TODO: c) std::partial_sum ile kümülatif kredi toplamını hesaplayın
    // ============================================

    // === GOREV 4.4 - partition ile Gruplandırma (10 dk) ===
    std::cout << "\n[GOREV 4.4]\n";
    // TODO: a) std::partition ile öğrencileri GPA >= 3.0 ve GPA < 3.0
    //          olarak ikiye ayırın:
    //          auto sinir = std::partition(liste.begin(), liste.end(),
    //              [](const Ogrenci& o) { return o.gpa >= 3.0; });
    //
    // TODO: b) Dönen iteratörü kullanarak her iki grubu ayrı ayrı yazdırın:
    //          std::cout << "\n--- Basarili ---\n";
    //          for (auto it = liste.begin(); it != sinir; ++it)
    //              std::cout << it->isim << " (" << it->gpa << ")\n";
    //          std::cout << "--- Basarisiz ---\n";
    //          for (auto it = sinir; it != liste.end(); ++it)
    //              std::cout << it->isim << " (" << it->gpa << ")\n";
    //
    // TODO: c) std::stable_partition ile tekrar yapın ve
    //          sıra farkını gözlemleyin
    //          (stable_partition orijinal sırayı korur)
    // ============================================

    // === GOREV 4.5 - Bonus: Birleştirilebilir Filtreler ve IIFE (10 dk) ===
    std::cout << "\n[GOREV 4.5]\n";
    // TODO: a) std::function kullanarak farklı filtreleri birleştirin:
    //          using Filtre = std::function<bool(const Ogrenci&)>;
    //
    //          Filtre gpa_filtre = [](const auto& o) { return o.gpa >= 3.0; };
    //          Filtre isim_filtre = [](const auto& o) { return o.isim[0] == 'A'; };
    //
    //          // VE birleşimi
    //          auto ve = [=](const Ogrenci& o) {
    //              return gpa_filtre(o) && isim_filtre(o);
    //          };
    //
    //          // copy_if ile kullan
    //          std::vector<Ogrenci> sonuc;
    //          std::copy_if(liste.begin(), liste.end(),
    //              std::back_inserter(sonuc), ve);
    //
    // TODO: b) Bonus 2 - IIFE: Hemen çağrılan lambda ile const not
    //          tablosu oluşturun:
    //          const auto harf_tablosu = []() {
    //              std::map<std::string, double> t;
    //              t["AA"] = 4.0; t["BA"] = 3.5; t["BB"] = 3.0;
    //              t["CB"] = 2.5; t["CC"] = 2.0; t["FF"] = 0.0;
    //              return t;
    //          }();  // () -> hemen çağır!
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 4: Lambda ve Ileri Algoritmalar ===

GPA >= 3: 4 ogrenci

[GOREV 4.1]

[GOREV 4.2]

[GOREV 4.3]

[GOREV 4.4]

[GOREV 4.5]
*/

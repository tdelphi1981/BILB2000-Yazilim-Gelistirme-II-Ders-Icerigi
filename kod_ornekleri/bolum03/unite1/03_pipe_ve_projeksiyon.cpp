/**
 * Pipe Operatoru ve Projeksiyonlar - Birlestirilebilir Islem Hatlari
 *
 * Bu program, C++20 Ranges kutuphanesindeki pipe (|) operatoru
 * ile cok asamali islem hatlarinin (pipeline) olusturulmasini
 * ve projeksiyonlarin kullanimini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - Pipe operatoru (|) ile cok asamali islem hatlari
 * - Projeksiyonlar: ranges::sort(sinif, {}, &Ogrenci::numara)
 * - auto ile birlestirilebilir pipeline tanimlama
 * - Pipeline sonucunu vector'e donusturme
 * - Karmasik OBYS sorgusu ornegi
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 1 - Ranges ve Views (C++20)
 *
 * Derleme: g++ -std=c++20 -o 03_pipe_ve_projeksiyon 03_pipe_ve_projeksiyon.cpp
 * Calistirma: ./03_pipe_ve_projeksiyon
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ranges>
#include <iterator>

struct Ogrenci {
    std::string isim;
    int numara;
    double ortalama;
    std::string bolum;
};

int main() {
    std::vector<Ogrenci> sinif = {
        {"Ayse Yilmaz",   1001, 85.5, "Bilgisayar"},
        {"Mehmet Demir",   1002, 72.0, "Elektrik"},
        {"Zeynep Kara",    1003, 91.3, "Bilgisayar"},
        {"Ali Ozturk",     1004, 68.4, "Makine"},
        {"Fatma Celik",    1005, 78.9, "Bilgisayar"},
        {"Burak Sahin",    1006, 95.2, "Elektrik"},
        {"Elif Arslan",    1007, 96.1, "Bilgisayar"},
        {"Caner Yildiz",   1008, 47.8, "Bilgisayar"},
        {"Derya Aksoy",    1009, 82.3, "Elektrik"},
        {"Emre Tas",       1010, 58.6, "Makine"}
    };

    // =============================================
    // 1) PROJEKSIYON: Dogrudan uye isimle siralama
    // =============================================
    std::cout << "=== Numaraya Gore Siralama (Projeksiyon) ===\n";

    // Projeksiyon ile: lambda yazmaya gerek yok!
    std::ranges::sort(sinif, {}, &Ogrenci::numara);

    for (const auto& ogr : sinif | std::views::take(5)) {
        std::cout << "  " << ogr.numara << " - " << ogr.isim << "\n";
    }

    // =============================================
    // 2) COK ASAMALI PIPE HATTI
    // =============================================
    std::cout << "\n=== Bilgisayar Bolumu - Basarili Ogrenci Isimleri ===\n";

    // Filtrele -> Donustur -> Goster
    auto bilgisayar_basarili = sinif
        | std::views::filter([](const Ogrenci& o) {
            return o.bolum == "Bilgisayar";
          })
        | std::views::filter([](const Ogrenci& o) {
            return o.ortalama >= 50.0;
          })
        | std::views::transform([](const Ogrenci& o) {
            return o.isim;
          });

    for (const auto& isim : bilgisayar_basarili) {
        std::cout << "  " << isim << "\n";
    }

    // =============================================
    // 3) PIPELINE SONUCUNU VECTOR'E KOPYALAMA
    // =============================================
    std::cout << "\n=== Pipeline -> Vector Donusumu ===\n";

    // Pipeline sonucunu bir vector'e kopyala
    std::vector<std::string> isim_listesi;
    auto yuksek_not_isimleri = sinif
        | std::views::filter([](const Ogrenci& o) {
            return o.ortalama >= 80.0;
          })
        | std::views::transform([](const Ogrenci& o) {
            return o.isim;
          });

    std::ranges::copy(yuksek_not_isimleri,
                      std::back_inserter(isim_listesi));

    std::cout << "  Yuksek notlu ogrenci sayisi: "
              << isim_listesi.size() << "\n";
    for (const auto& isim : isim_listesi) {
        std::cout << "  - " << isim << "\n";
    }

    // =============================================
    // 4) KARMASIK OBYS SORGUSU
    // =============================================
    std::cout << "\n=== Elektrik Bol. - Ortalamaya Gore Ilk 2 ===\n";

    // Once Elektrik bolumu ogrencilerini ayri bir vector'e al
    std::vector<Ogrenci> elektrik;
    std::ranges::copy_if(sinif, std::back_inserter(elektrik),
        [](const Ogrenci& o) { return o.bolum == "Elektrik"; });

    // Ortalamaya gore azalan sirada sirala (projeksiyon)
    std::ranges::sort(elektrik, std::ranges::greater{},
                      &Ogrenci::ortalama);

    // Ilk 2'yi goster
    for (const auto& ogr : elektrik | std::views::take(2)) {
        std::cout << "  " << ogr.isim << " - Ort: "
                  << ogr.ortalama << "\n";
    }

    // =============================================
    // 5) BIRLESTIRILEBILIR PIPELINE (auto ile)
    // =============================================
    std::cout << "\n=== Yeniden Kullanilabilir Pipeline ===\n";

    // Pipeline'i bir degiskende sakla
    auto gecen_ve_isim = std::views::filter([](const Ogrenci& o) {
                             return o.ortalama >= 50.0;
                         })
                       | std::views::transform([](const Ogrenci& o) {
                             return o.isim + " (" + o.bolum + ")";
                         });

    // Ayni pipeline'i farkli veri uzerinde kullanabilirsiniz
    for (const auto& bilgi : sinif | gecen_ve_isim) {
        std::cout << "  " << bilgi << "\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Numaraya Gore Siralama (Projeksiyon) ===
 *   1001 - Ayse Yilmaz
 *   1002 - Mehmet Demir
 *   1003 - Zeynep Kara
 *   1004 - Ali Ozturk
 *   1005 - Fatma Celik
 *
 * === Bilgisayar Bolumu - Basarili Ogrenci Isimleri ===
 *   Ayse Yilmaz
 *   Zeynep Kara
 *   Fatma Celik
 *   Elif Arslan
 *
 * === Pipeline -> Vector Donusumu ===
 *   Yuksek notlu ogrenci sayisi: 5
 *   - Ayse Yilmaz
 *   - Zeynep Kara
 *   - Burak Sahin
 *   - Elif Arslan
 *   - Derya Aksoy
 *
 * === Elektrik Bol. - Ortalamaya Gore Ilk 2 ===
 *   Burak Sahin - Ort: 95.2
 *   Derya Aksoy - Ort: 82.3
 *
 * === Yeniden Kullanilabilir Pipeline ===
 *   Ayse Yilmaz (Bilgisayar)
 *   Mehmet Demir (Elektrik)
 *   Zeynep Kara (Bilgisayar)
 *   Ali Ozturk (Makine)
 *   Fatma Celik (Bilgisayar)
 *   Burak Sahin (Elektrik)
 *   Elif Arslan (Bilgisayar)
 *   Derya Aksoy (Elektrik)
 *   Emre Tas (Makine)
 */

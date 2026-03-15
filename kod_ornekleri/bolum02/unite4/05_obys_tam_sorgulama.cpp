/**
 * OBYS: Lambda Tabanli Tam Sorgulama
 *
 * Bu program Bolum 1'deki kapsayicilar ile Bolum 2'deki
 * iteratorler, algoritmalar ve lambda'lari birlestirerek
 * OBYS icin kapsamli bir sorgulama sistemi olusturur.
 *
 * Ogrenilecek kavramlar:
 * - Lambda fabrika fonksiyonlari
 * - Birlestirici (combinator) deseni
 * - ranges::copy_if, ranges::sort, ranges::for_each
 * - std::accumulate ile istatistik hesaplama
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 4 - Lambda Ifadeleri
 *
 * Derleme: g++ -std=c++20 -o 05_obys_tam_sorgulama 05_obys_tam_sorgulama.cpp
 * Calistirma: ./05_obys_tam_sorgulama
 */

#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iostream>
#include <string>

struct Ogrenci {
    std::string isim;
    int numara;
    std::string bolum;
    double ortalama;
    bool aktif;
};

// Fabrika fonksiyonlari: yapilandirılabilir lambda'lar uretir
auto not_filtresi(double min, double max) {
    return [min, max](const Ogrenci& o) {
        return o.ortalama >= min && o.ortalama <= max;
    };
}

auto bolum_filtresi(const std::string& bolum) {
    return [bolum](const Ogrenci& o) { return o.bolum == bolum; };
}

// Birlestirici: iki filtreyi AND ile birlestirir
template<typename F1, typename F2>
auto ve(F1 f1, F2 f2) {
    return [f1, f2](const Ogrenci& o) { return f1(o) && f2(o); };
}

int main() {
    std::vector<Ogrenci> ogrenciler = {
        {"Ali Yilmaz", 1001, "Bilgisayar", 88.5, true},
        {"Ayse Kaya", 1002, "Bilgisayar", 92.0, true},
        {"Mehmet Demir", 1003, "Elektrik", 75.3, true},
        {"Fatma Celik", 1004, "Bilgisayar", 65.0, false},
        {"Can Ozturk", 1005, "Elektrik", 91.2, true}
    };

    // Sorgu: Bilgisayar bolumu VE 80 ustu
    auto sorgu = ve(bolum_filtresi("Bilgisayar"), not_filtresi(80, 100));

    // Filtrele
    std::vector<Ogrenci> sonuc;
    std::ranges::copy_if(ogrenciler, std::back_inserter(sonuc), sorgu);

    // Sirala (nota gore azalan)
    std::ranges::sort(sonuc, [](const Ogrenci& a, const Ogrenci& b) {
        return a.ortalama > b.ortalama;
    });

    // Yazdir
    std::ranges::for_each(sonuc, [](const Ogrenci& o) {
        std::cout << o.isim << " - " << o.ortalama << '\n';
    });

    // Ortalama hesapla
    if (!sonuc.empty()) {
        double ort = std::accumulate(sonuc.begin(), sonuc.end(), 0.0,
            [](double acc, const Ogrenci& o) {
                return acc + o.ortalama;
            }) / sonuc.size();
        std::cout << "Ortalama: " << ort << '\n';
    }

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
Ayse Kaya - 92
Ali Yilmaz - 88.5
Ortalama: 90.25
*/

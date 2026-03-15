/**
 * OBYS Lambda Sorgulama - Dinamik Sorgu Sistemi
 *
 * Bu program OBYS'de lambda tabanli dinamik bir sorgu ve
 * filtreleme sistemi uygular.
 *
 * Ogrenilecek kavramlar:
 * - Lambda ile dinamik filtre zincirleme
 * - std::function ile strateji deseni
 * - Composable predicate'ler
 * - Bolum 2'nin tum kavramlarinin entegrasyonu
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 4 - Lambda Ifadeleri
 *
 * Derleme: g++ -std=c++20 -o 04_obys_lambda_sorgulama 04_obys_lambda_sorgulama.cpp
 * Calistirma: ./04_obys_lambda_sorgulama
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>
#include <iomanip>
#include <memory>
#include <iterator>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
    std::string bolum;
    std::set<std::string> dersler;
    int sinif_yili;  // 1, 2, 3, 4
};

std::ostream& operator<<(std::ostream& os, const Ogrenci& o) {
    os << std::setw(5) << o.numara << " | "
       << std::left << std::setw(18) << o.isim
       << " | " << std::setw(12) << o.bolum
       << " | GPA: " << std::fixed << std::setprecision(2) << o.gpa
       << " | Sinif: " << o.sinif_yili;
    return os;
}

// Filtre turu
using Filtre = std::function<bool(const Ogrenci&)>;

// Filtre fabrikasi
class SorguMotoru {
public:
    // Temel filtreler
    static Filtre gpa_filtre(double esik) {
        return [esik](const Ogrenci& o) { return o.gpa >= esik; };
    }

    static Filtre bolum_filtre(const std::string& bolum) {
        return [bolum](const Ogrenci& o) { return o.bolum == bolum; };
    }

    static Filtre sinif_filtre(int yil) {
        return [yil](const Ogrenci& o) { return o.sinif_yili == yil; };
    }

    static Filtre ders_filtre(const std::string& ders) {
        return [ders](const Ogrenci& o) {
            return o.dersler.contains(ders);
        };
    }

    // Bilesik operatorler
    static Filtre ve(Filtre f1, Filtre f2) {
        return [f1, f2](const Ogrenci& o) { return f1(o) && f2(o); };
    }

    static Filtre veya(Filtre f1, Filtre f2) {
        return [f1, f2](const Ogrenci& o) { return f1(o) || f2(o); };
    }

    static Filtre degil(Filtre f) {
        return [f](const Ogrenci& o) { return !f(o); };
    }

    // Coklu VE bilesimi
    static Filtre tumunu_sagla(std::vector<Filtre> filtreler) {
        return [filtreler](const Ogrenci& o) {
            return std::all_of(filtreler.begin(), filtreler.end(),
                [&o](const Filtre& f) { return f(o); });
        };
    }

    // Sorgu calistir
    static std::vector<const Ogrenci*>
    calistir(const std::vector<Ogrenci>& sinif, Filtre filtre) {
        std::vector<const Ogrenci*> sonuc;
        for (const auto& o : sinif) {
            if (filtre(o)) {
                sonuc.push_back(&o);
            }
        }
        return sonuc;
    }
};

// Siralama stratejisi
using Siralayici = std::function<bool(const Ogrenci*, const Ogrenci*)>;

int main() {
    std::cout << "=== OBYS Lambda Sorgulama Sistemi ===\n\n";

    // Ogrenci veritabani
    std::vector<Ogrenci> veritabani = {
        {1001, "Ali Yilmaz",     3.45, "Bilgisayar", {"C++", "Fizik", "Matematik"}, 2},
        {1002, "Ayse Kaya",      3.80, "Bilgisayar", {"C++", "Veritabani", "Matematik"}, 3},
        {1003, "Mehmet Demir",   2.90, "Elektrik",   {"C++", "Elektronik", "Fizik"}, 2},
        {1004, "Zeynep Can",     3.65, "Bilgisayar", {"C++", "Yapay Zeka", "Matematik"}, 4},
        {1005, "Can Arslan",     2.10, "Makine",     {"Fizik", "Termodinamik"}, 1},
        {1006, "Elif Yildiz",    3.92, "Bilgisayar", {"C++", "Yapay Zeka", "Istatistik"}, 3},
        {1007, "Burak Ozturk",   2.75, "Elektrik",   {"Fizik", "Elektronik"}, 2},
        {1008, "Selin Aktas",    3.50, "Bilgisayar", {"C++", "Veritabani", "Istatistik"}, 3},
        {1009, "Emre Celik",     1.95, "Makine",     {"Fizik"}, 1},
        {1010, "Deniz Sahin",    3.30, "Elektrik",   {"C++", "Fizik", "Matematik"}, 4}
    };

    // --- Sorgu 1: Basit filtre ---
    std::cout << "--- Sorgu 1: GPA >= 3.5 ---\n";
    auto sonuc1 = SorguMotoru::calistir(veritabani,
        SorguMotoru::gpa_filtre(3.5));

    for (const auto* o : sonuc1) {
        std::cout << "  " << *o << "\n";
    }
    std::cout << "  Sonuc: " << sonuc1.size() << " ogrenci\n";

    // --- Sorgu 2: Bilesik filtre ---
    std::cout << "\n--- Sorgu 2: Bilgisayar + GPA >= 3.5 ---\n";
    auto sorgu2 = SorguMotoru::ve(
        SorguMotoru::bolum_filtre("Bilgisayar"),
        SorguMotoru::gpa_filtre(3.5));

    auto sonuc2 = SorguMotoru::calistir(veritabani, sorgu2);
    for (const auto* o : sonuc2) {
        std::cout << "  " << *o << "\n";
    }

    // --- Sorgu 3: Ders bazli filtre ---
    std::cout << "\n--- Sorgu 3: C++ dersi alan + 3. sinif ---\n";
    auto sorgu3 = SorguMotoru::ve(
        SorguMotoru::ders_filtre("C++"),
        SorguMotoru::sinif_filtre(3));

    auto sonuc3 = SorguMotoru::calistir(veritabani, sorgu3);
    for (const auto* o : sonuc3) {
        std::cout << "  " << *o << "\n";
    }

    // --- Sorgu 4: Karmasik sorgu ---
    std::cout << "\n--- Sorgu 4: (Bilgisayar VEYA Elektrik) VE GPA >= 3.0 ---\n";
    auto sorgu4 = SorguMotoru::ve(
        SorguMotoru::veya(
            SorguMotoru::bolum_filtre("Bilgisayar"),
            SorguMotoru::bolum_filtre("Elektrik")),
        SorguMotoru::gpa_filtre(3.0));

    auto sonuc4 = SorguMotoru::calistir(veritabani, sorgu4);

    // Sonuclari GPA'ya gore sirala
    std::sort(sonuc4.begin(), sonuc4.end(),
        [](const Ogrenci* a, const Ogrenci* b) { return a->gpa > b->gpa; });

    for (const auto* o : sonuc4) {
        std::cout << "  " << *o << "\n";
    }

    // --- Sorgu 5: Coklu filtre zincirleme ---
    std::cout << "\n--- Sorgu 5: Coklu filtre ---\n";
    auto sorgu5 = SorguMotoru::tumunu_sagla({
        SorguMotoru::bolum_filtre("Bilgisayar"),
        SorguMotoru::gpa_filtre(3.0),
        SorguMotoru::ders_filtre("C++"),
        SorguMotoru::degil(SorguMotoru::sinif_filtre(1))
    });

    auto sonuc5 = SorguMotoru::calistir(veritabani, sorgu5);
    for (const auto* o : sonuc5) {
        std::cout << "  " << *o << "\n";
    }

    // --- Istatistikler ---
    std::cout << "\n--- Sorgu Sonuc Istatistikleri ---\n";

    // Sonuc kumesinin GPA ortalamasi (accumulate + transform)
    if (!sonuc4.empty()) {
        double ort = std::transform_reduce(
            sonuc4.begin(), sonuc4.end(), 0.0,
            std::plus<>(),
            [](const Ogrenci* o) { return o->gpa; }
        ) / sonuc4.size();

        std::cout << "  Sorgu 4 GPA ortalamasi: " << std::fixed
                  << std::setprecision(2) << ort << "\n";
    }

    // --- Ders kesisimleri ---
    std::cout << "\n--- Sorgu 4 Ogrencilerinin Ortak Dersleri ---\n";
    if (sonuc4.size() >= 2) {
        std::set<std::string> ortak = sonuc4[0]->dersler;
        for (size_t i = 1; i < sonuc4.size(); ++i) {
            std::set<std::string> gecici;
            std::set_intersection(
                ortak.begin(), ortak.end(),
                sonuc4[i]->dersler.begin(), sonuc4[i]->dersler.end(),
                std::inserter(gecici, gecici.begin()));
            ortak = std::move(gecici);
        }

        if (ortak.empty()) {
            std::cout << "  Tum ogrencilerin ortak dersi yok.\n";
        } else {
            std::cout << "  Ortak dersler: ";
            for (const auto& d : ortak) std::cout << d << ", ";
            std::cout << "\n";
        }
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === OBYS Lambda Sorgulama Sistemi ===
 *
 * --- Sorgu 1: GPA >= 3.5 ---
 *   (GPA 3.5 ve ustu ogrenciler)
 *
 * --- Sorgu 2: Bilgisayar + GPA >= 3.5 ---
 *   (Bilgisayar bolumu + GPA 3.5+)
 *
 * --- Sorgu 3: C++ dersi alan + 3. sinif ---
 *   (C++ alan 3. sinif ogrenciler)
 *
 * --- Sorgu 4: (Bilgisayar VEYA Elektrik) VE GPA >= 3.0 ---
 *   (GPA sirasinda listelenir)
 *
 * --- Sorgu 5: Coklu filtre ---
 *   (tum kosullari saglayan ogrenciler)
 *
 * --- Sorgu Sonuc Istatistikleri ---
 *   (GPA ortalamasi)
 *
 * --- Sorgu 4 Ogrencilerinin Ortak Dersleri ---
 *   (ortak ders listesi)
 */

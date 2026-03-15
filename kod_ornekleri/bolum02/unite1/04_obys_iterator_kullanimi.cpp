/**
 * OBYS Iterator Kullanimi - Ogrenci Kayit Sistemi
 *
 * Bu program OBYS baglaminda iteratorlerin pratik kullanimini
 * gosterir: map uzerinde dolasim, ters yazdirma, filtreleme.
 *
 * Ogrenilecek kavramlar:
 * - map iteratorleri ile structured bindings
 * - reverse_iterator ile ters siralama
 * - const_iterator ile guvenli erisim
 * - Iterator adaptorlerinin OBYS'de kullanimi
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 1 - Iterator Kavrami
 *
 * Derleme: g++ -std=c++20 -o 04_obys_iterator_kullanimi 04_obys_iterator_kullanimi.cpp
 * Calistirma: ./04_obys_iterator_kullanimi
 */

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <iomanip>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
    std::vector<std::string> dersler;
};

// Ogrenci bilgisini yazdirma
std::ostream& operator<<(std::ostream& os, const Ogrenci& o) {
    os << o.numara << ": " << o.isim << " (GPA: "
       << std::fixed << std::setprecision(2) << o.gpa << ")";
    return os;
}

// OBYS deposu tipi
using OgrenciDepo = std::map<int, std::shared_ptr<Ogrenci>>;

// const_iterator ile guvenli dolasim
void listele(const OgrenciDepo& depo) {
    std::cout << "--- Ogrenci Listesi ---\n";
    // const referans aldigimiz icin sadece const_iterator kullanilir
    for (auto it = depo.cbegin(); it != depo.cend(); ++it) {
        std::cout << "  " << *(it->second) << "\n";
    }
}

// Ters sirada yazdirma (reverse_iterator)
void ters_listele(const OgrenciDepo& depo) {
    std::cout << "--- Ters Sirada Ogrenci Listesi ---\n";
    for (auto rit = depo.crbegin(); rit != depo.crend(); ++rit) {
        std::cout << "  " << *(rit->second) << "\n";
    }
}

// Basarili ogrencileri filtrele (back_inserter ile)
std::vector<std::shared_ptr<Ogrenci>>
basarili_filtrele(const OgrenciDepo& depo, double esik) {
    std::vector<std::shared_ptr<Ogrenci>> sonuc;

    for (auto it = depo.cbegin(); it != depo.cend(); ++it) {
        if (it->second->gpa >= esik) {
            sonuc.push_back(it->second);
        }
    }

    return sonuc;
}

// GPA ortalamasini hesapla (iterator + accumulate)
double ortalama_gpa(const OgrenciDepo& depo) {
    if (depo.empty()) return 0.0;

    double toplam = 0.0;
    for (auto it = depo.cbegin(); it != depo.cend(); ++it) {
        toplam += it->second->gpa;
    }
    return toplam / static_cast<double>(depo.size());
}

int main() {
    std::cout << "=== OBYS Iterator Kullanimi ===\n\n";

    // Ogrenci deposunu olustur
    OgrenciDepo depo;
    depo[1001] = std::make_shared<Ogrenci>(
        Ogrenci{1001, "Ali Yilmaz", 3.45, {"Fizik", "Matematik", "C++"}});
    depo[1002] = std::make_shared<Ogrenci>(
        Ogrenci{1002, "Ayse Kaya", 3.80, {"Kimya", "Matematik", "C++"}});
    depo[1003] = std::make_shared<Ogrenci>(
        Ogrenci{1003, "Mehmet Demir", 2.90, {"Fizik", "Biyoloji"}});
    depo[1004] = std::make_shared<Ogrenci>(
        Ogrenci{1004, "Zeynep Can", 3.65, {"Matematik", "C++", "Veritabani"}});
    depo[1005] = std::make_shared<Ogrenci>(
        Ogrenci{1005, "Can Arslan", 2.10, {"Fizik", "Kimya"}});

    // 1. Normal sirada listele
    listele(depo);

    // 2. Ters sirada listele
    std::cout << "\n";
    ters_listele(depo);

    // 3. Basarili ogrencileri filtrele
    std::cout << "\n--- GPA >= 3.0 Olanlar ---\n";
    auto basarililar = basarili_filtrele(depo, 3.0);
    for (const auto& ogr : basarililar) {
        std::cout << "  " << *ogr << "\n";
    }

    // 4. Sinif GPA ortalamasi
    std::cout << "\nSinif GPA ortalamasi: "
              << std::fixed << std::setprecision(2)
              << ortalama_gpa(depo) << "\n";

    // 5. Structured bindings ile modern dolasim (C++17)
    std::cout << "\n--- Structured Bindings ile Dolasim ---\n";
    for (const auto& [numara, ogr_ptr] : depo) {
        std::cout << "  No: " << numara
                  << " | " << ogr_ptr->isim
                  << " | Ders sayisi: " << ogr_ptr->dersler.size()
                  << "\n";
    }

    // 6. Iterator ile belirli aralikta islem
    std::cout << "\n--- Belirli Aralikta Islem ---\n";
    auto baslangic = depo.find(1002);
    auto bitis = depo.find(1005);

    if (baslangic != depo.end() && bitis != depo.end()) {
        std::cout << "1002-1004 arasi ogrenciler:\n";
        for (auto it = baslangic; it != bitis; ++it) {
            std::cout << "  " << *(it->second) << "\n";
        }
    }

    // 7. Ogrenci derslerini ostream_iterator ile yazdirma
    std::cout << "\n--- Ders Listeleri ---\n";
    for (const auto& [numara, ogr_ptr] : depo) {
        std::cout << ogr_ptr->isim << ": ";
        std::copy(ogr_ptr->dersler.cbegin(),
                  ogr_ptr->dersler.cend(),
                  std::ostream_iterator<std::string>(std::cout, ", "));
        std::cout << "\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === OBYS Iterator Kullanimi ===
 *
 * --- Ogrenci Listesi ---
 *   1001: Ali Yilmaz (GPA: 3.45)
 *   1002: Ayse Kaya (GPA: 3.80)
 *   1003: Mehmet Demir (GPA: 2.90)
 *   1004: Zeynep Can (GPA: 3.65)
 *   1005: Can Arslan (GPA: 2.10)
 *
 * --- Ters Sirada Ogrenci Listesi ---
 *   1005: Can Arslan (GPA: 2.10)
 *   1004: Zeynep Can (GPA: 3.65)
 *   1003: Mehmet Demir (GPA: 2.90)
 *   1002: Ayse Kaya (GPA: 3.80)
 *   1001: Ali Yilmaz (GPA: 3.45)
 *
 * --- GPA >= 3.0 Olanlar ---
 *   1001: Ali Yilmaz (GPA: 3.45)
 *   1002: Ayse Kaya (GPA: 3.80)
 *   1004: Zeynep Can (GPA: 3.65)
 *
 * Sinif GPA ortalamasi: 3.18
 *
 * --- Structured Bindings ile Dolasim ---
 *   No: 1001 | Ali Yilmaz | Ders sayisi: 3
 *   No: 1002 | Ayse Kaya | Ders sayisi: 3
 *   No: 1003 | Mehmet Demir | Ders sayisi: 2
 *   No: 1004 | Zeynep Can | Ders sayisi: 3
 *   No: 1005 | Can Arslan | Ders sayisi: 2
 *
 * --- Belirli Aralikta Islem ---
 * 1002-1004 arasi ogrenciler:
 *   1002: Ayse Kaya (GPA: 3.80)
 *   1003: Mehmet Demir (GPA: 2.90)
 *   1004: Zeynep Can (GPA: 3.65)
 *
 * --- Ders Listeleri ---
 * Ali Yilmaz: Fizik, Matematik, C++,
 * Ayse Kaya: Kimya, Matematik, C++,
 * Mehmet Demir: Fizik, Biyoloji,
 * Zeynep Can: Matematik, C++, Veritabani,
 * Can Arslan: Fizik, Kimya,
 */

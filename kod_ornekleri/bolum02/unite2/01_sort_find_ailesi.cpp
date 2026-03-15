/**
 * Sort ve Find Ailesi - Siralama ve Arama Algoritmalari
 *
 * Bu program STL siralama ve arama algoritmalarini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - std::sort, std::stable_sort, std::partial_sort
 * - std::find, std::find_if, std::binary_search
 * - std::lower_bound, std::upper_bound
 * - std::count, std::count_if
 * - Ozel karsilastirici (lambda) kullanimi
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 2 - Temel STL Algoritmalari
 *
 * Derleme: g++ -std=c++20 -o 01_sort_find_ailesi 01_sort_find_ailesi.cpp
 * Calistirma: ./01_sort_find_ailesi
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

std::ostream& operator<<(std::ostream& os, const Ogrenci& o) {
    os << o.numara << ": " << std::left << std::setw(15) << o.isim
       << " (GPA: " << std::fixed << std::setprecision(2) << o.gpa << ")";
    return os;
}

int main() {
    std::cout << "=== Sort ve Find Ailesi ===\n\n";

    // --- 1. std::sort ---
    std::cout << "--- std::sort ---\n";
    std::vector<int> notlar = {85, 92, 78, 95, 88, 72, 91, 68};

    std::sort(notlar.begin(), notlar.end());
    std::cout << "Artan sira: ";
    for (int n : notlar) std::cout << n << " ";
    std::cout << "\n";

    // Azalan sira (lambda karsilastirici)
    std::sort(notlar.begin(), notlar.end(),
              [](int a, int b) { return a > b; });
    std::cout << "Azalan sira: ";
    for (int n : notlar) std::cout << n << " ";
    std::cout << "\n";

    // --- 2. std::stable_sort ---
    std::cout << "\n--- std::stable_sort ---\n";
    std::vector<Ogrenci> sinif = {
        {1001, "Ali Yilmaz",   3.45},
        {1002, "Ayse Kaya",    3.80},
        {1003, "Mehmet Demir", 3.80},
        {1004, "Zeynep Can",   3.45},
        {1005, "Can Arslan",   2.90}
    };

    // stable_sort: ayni GPA'ya sahip ogrencilerin orijinal sirasi korunur
    std::stable_sort(sinif.begin(), sinif.end(),
        [](const Ogrenci& a, const Ogrenci& b) {
            return a.gpa > b.gpa;
        });

    std::cout << "GPA'ya gore (kararlı sıralama):\n";
    for (const auto& o : sinif) {
        std::cout << "  " << o << "\n";
    }
    std::cout << "  [!] Ayse(3.80) hala Mehmet(3.80)'ten once\n";

    // --- 3. std::partial_sort ---
    std::cout << "\n--- std::partial_sort ---\n";
    std::vector<int> puanlar = {45, 92, 78, 95, 88, 72, 91, 68, 55, 83};

    // Sadece en yuksek 3 puani sirala
    std::partial_sort(puanlar.begin(),
                      puanlar.begin() + 3,
                      puanlar.end(),
                      std::greater<int>());

    std::cout << "En yuksek 3: ";
    for (int i = 0; i < 3; ++i) {
        std::cout << puanlar[i] << " ";
    }
    std::cout << "\n";

    // --- 4. std::nth_element ---
    std::cout << "\n--- std::nth_element ---\n";
    std::vector<int> veri = {45, 92, 78, 95, 88, 72, 91, 68, 55, 83};

    // Medyan elemani bul (ortanca pozisyona yerlestir)
    auto medyan_pos = veri.begin() + veri.size() / 2;
    std::nth_element(veri.begin(), medyan_pos, veri.end());
    std::cout << "Medyan deger: " << *medyan_pos << "\n";

    // --- 5. std::find / std::find_if ---
    std::cout << "\n--- std::find / std::find_if ---\n";

    // Ogrenci listesini yeniden olustur
    std::vector<Ogrenci> ogrenciler = {
        {1001, "Ali Yilmaz",   3.45},
        {1002, "Ayse Kaya",    3.80},
        {1003, "Mehmet Demir", 2.90},
        {1004, "Zeynep Can",   3.65},
        {1005, "Can Arslan",   2.10}
    };

    // Isime gore arama
    auto it = std::find_if(ogrenciler.begin(), ogrenciler.end(),
        [](const Ogrenci& o) { return o.isim == "Zeynep Can"; });

    if (it != ogrenciler.end()) {
        std::cout << "Bulundu: " << *it << "\n";
    }

    // Numaraya gore arama
    int aranan_no = 1003;
    auto it2 = std::find_if(ogrenciler.begin(), ogrenciler.end(),
        [aranan_no](const Ogrenci& o) { return o.numara == aranan_no; });

    if (it2 != ogrenciler.end()) {
        std::cout << "No " << aranan_no << " bulundu: " << it2->isim << "\n";
    }

    // find_if_not: GPA >= 3.0 OLMAYAN ilk ogrenciyi bul
    auto basarisiz = std::find_if_not(ogrenciler.begin(), ogrenciler.end(),
        [](const Ogrenci& o) { return o.gpa >= 3.0; });

    if (basarisiz != ogrenciler.end()) {
        std::cout << "Ilk basarisiz: " << *basarisiz << "\n";
    }

    // --- 6. std::binary_search ---
    std::cout << "\n--- std::binary_search ---\n";
    std::vector<int> sirali_notlar = {68, 72, 78, 85, 88, 91, 92, 95};

    // binary_search ONCEDEN SIRALI veri gerektirir!
    bool var_mi = std::binary_search(sirali_notlar.begin(),
                                      sirali_notlar.end(), 88);
    std::cout << "88 var mi? " << (var_mi ? "Evet" : "Hayir") << "\n";

    var_mi = std::binary_search(sirali_notlar.begin(),
                                 sirali_notlar.end(), 90);
    std::cout << "90 var mi? " << (var_mi ? "Evet" : "Hayir") << "\n";

    // --- 7. std::lower_bound / upper_bound ---
    std::cout << "\n--- lower_bound / upper_bound ---\n";
    // 85 ve ustu notlari bul
    auto alt = std::lower_bound(sirali_notlar.begin(),
                                 sirali_notlar.end(), 85);
    auto ust = std::upper_bound(sirali_notlar.begin(),
                                 sirali_notlar.end(), 92);

    std::cout << "85-92 arasi notlar: ";
    for (auto it3 = alt; it3 != ust; ++it3) {
        std::cout << *it3 << " ";
    }
    std::cout << "\n";

    // --- 8. std::count / std::count_if ---
    std::cout << "\n--- count / count_if ---\n";
    int basarili_sayisi = std::count_if(ogrenciler.begin(), ogrenciler.end(),
        [](const Ogrenci& o) { return o.gpa >= 3.0; });

    std::cout << "GPA >= 3.0 olan: " << basarili_sayisi << " ogrenci\n";
    std::cout << "GPA < 3.0 olan: "
              << static_cast<int>(ogrenciler.size()) - basarili_sayisi
              << " ogrenci\n";

    // --- 9. OBYS: Tam siralama ve arama senaryosu ---
    std::cout << "\n--- OBYS: Siralama + Arama ---\n";

    // GPA'ya gore sirala
    std::sort(ogrenciler.begin(), ogrenciler.end(),
        [](const Ogrenci& a, const Ogrenci& b) {
            return a.gpa > b.gpa;
        });

    std::cout << "Basari siralaması:\n";
    int sira = 1;
    for (const auto& o : ogrenciler) {
        std::cout << "  " << sira++ << ". " << o << "\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Sort ve Find Ailesi ===
 *
 * --- std::sort ---
 * Artan sira: 68 72 78 85 88 91 92 95
 * Azalan sira: 95 92 91 88 85 78 72 68
 *
 * --- std::stable_sort ---
 * GPA'ya gore (kararlı sıralama):
 *   1002: Ayse Kaya       (GPA: 3.80)
 *   1003: Mehmet Demir    (GPA: 3.80)
 *   1001: Ali Yilmaz      (GPA: 3.45)
 *   1004: Zeynep Can      (GPA: 3.45)
 *   1005: Can Arslan       (GPA: 2.90)
 *   [!] Ayse(3.80) hala Mehmet(3.80)'ten once
 *
 * --- std::partial_sort ---
 * En yuksek 3: 95 92 91
 *
 * --- std::nth_element ---
 * Medyan deger: 78
 *
 * --- std::find / std::find_if ---
 * Bulundu: 1004: Zeynep Can      (GPA: 3.65)
 * No 1003 bulundu: Mehmet Demir
 * Ilk basarisiz: 1003: Mehmet Demir    (GPA: 2.90)
 *
 * --- std::binary_search ---
 * 88 var mi? Evet
 * 90 var mi? Hayir
 *
 * --- lower_bound / upper_bound ---
 * 85-92 arasi notlar: 85 88 91 92
 *
 * --- count / count_if ---
 * GPA >= 3.0 olan: 3 ogrenci
 * GPA < 3.0 olan: 2 ogrenci
 *
 * --- OBYS: Siralama + Arama ---
 * Basari siralaması:
 *   1. 1002: Ayse Kaya       (GPA: 3.80)
 *   2. 1004: Zeynep Can      (GPA: 3.65)
 *   3. 1001: Ali Yilmaz      (GPA: 3.45)
 *   4. 1003: Mehmet Demir    (GPA: 2.90)
 *   5. 1005: Can Arslan       (GPA: 2.10)
 */

/**
 * Partition ve nth_element - Bolumleme Algoritmalari
 *
 * Bu program partition, stable_partition ve nth_element kullanimini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - std::partition ile eleman bolumleme
 * - std::stable_partition ile kararli bolumleme
 * - std::partition_point ile bolum noktasi bulma
 * - std::nth_element ile medyan ve yuzdelik dilim
 * - OBYS'de basarili/basarisiz ayirma ve medyan hesabi
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 3 - Ileri STL Algoritmalari
 *
 * Derleme: g++ -std=c++20 -o 03_partition_nth_element 03_partition_nth_element.cpp
 * Calistirma: ./03_partition_nth_element
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
       << " GPA: " << std::fixed << std::setprecision(2) << o.gpa;
    return os;
}

int main() {
    std::cout << "=== Partition ve nth_element ===\n\n";

    // --- 1. std::partition ---
    std::cout << "--- std::partition ---\n";
    std::vector<int> notlar = {85, 45, 92, 38, 78, 55, 91, 42, 88, 60};

    std::cout << "Orijinal: ";
    for (int n : notlar) std::cout << n << " ";
    std::cout << "\n";

    // 60 ve ustu / 60 alti olarak bolumlele
    auto bolum = std::partition(notlar.begin(), notlar.end(),
                                [](int n) { return n >= 60; });

    std::cout << "Basarili (>= 60): ";
    for (auto it = notlar.begin(); it != bolum; ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\nBasarisiz (< 60): ";
    for (auto it = bolum; it != notlar.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // --- 2. std::stable_partition ---
    std::cout << "\n--- std::stable_partition ---\n";
    std::vector<Ogrenci> sinif = {
        {1001, "Ali Yilmaz",   3.45},
        {1002, "Ayse Kaya",    3.80},
        {1003, "Mehmet Demir", 2.90},
        {1004, "Zeynep Can",   3.65},
        {1005, "Can Arslan",   2.10},
        {1006, "Elif Yildiz",  3.92},
        {1007, "Burak Ozturk", 2.75}
    };

    std::cout << "Orijinal sira:\n";
    for (const auto& o : sinif) std::cout << "  " << o << "\n";

    // GPA >= 3.0 olanlar one, geri kalanlar arkaya
    // stable: her grup icinde orijinal sira korunur
    auto sinir = std::stable_partition(sinif.begin(), sinif.end(),
        [](const Ogrenci& o) { return o.gpa >= 3.0; });

    std::cout << "\nstable_partition (esik: 3.0):\n";
    std::cout << "  --- Basarili ---\n";
    for (auto it = sinif.begin(); it != sinir; ++it) {
        std::cout << "  " << *it << "\n";
    }
    std::cout << "  --- Kosullu ---\n";
    for (auto it = sinir; it != sinif.end(); ++it) {
        std::cout << "  " << *it << "\n";
    }

    // --- 3. std::is_partitioned & partition_point ---
    std::cout << "\n--- is_partitioned & partition_point ---\n";
    auto esik = [](const Ogrenci& o) { return o.gpa >= 3.0; };

    bool bolumlu_mu = std::is_partitioned(sinif.begin(), sinif.end(), esik);
    std::cout << "Bolumlu mu? " << (bolumlu_mu ? "Evet" : "Hayir") << "\n";

    if (bolumlu_mu) {
        auto nokta = std::partition_point(sinif.begin(), sinif.end(), esik);
        int basarili_sayisi = std::distance(sinif.begin(), nokta);
        std::cout << "Basarili ogrenci sayisi: " << basarili_sayisi << "\n";
    }

    // --- 4. std::nth_element ---
    std::cout << "\n--- std::nth_element ---\n";
    std::vector<double> gpa_listesi = {3.45, 3.80, 2.90, 3.65, 2.10, 3.92, 2.75, 3.50, 1.95, 3.30};

    std::cout << "GPA'lar: ";
    for (double g : gpa_listesi) std::cout << g << " ";
    std::cout << "\n";

    // Medyan (ortanca) bul - O(n) karmasiklik
    auto medyan_pos = gpa_listesi.begin() + gpa_listesi.size() / 2;
    std::nth_element(gpa_listesi.begin(), medyan_pos, gpa_listesi.end());

    std::cout << "Medyan GPA: " << *medyan_pos << "\n";

    // 25. yuzdelik dilim (Q1)
    auto q1_pos = gpa_listesi.begin() + gpa_listesi.size() / 4;
    std::nth_element(gpa_listesi.begin(), q1_pos, gpa_listesi.end());
    std::cout << "Q1 (25. yuzdelik): " << *q1_pos << "\n";

    // 75. yuzdelik dilim (Q3)
    auto q3_pos = gpa_listesi.begin() + 3 * gpa_listesi.size() / 4;
    std::nth_element(gpa_listesi.begin(), q3_pos, gpa_listesi.end());
    std::cout << "Q3 (75. yuzdelik): " << *q3_pos << "\n";

    // En buyuk 3 eleman
    std::cout << "\n--- En Buyuk 3 GPA ---\n";
    std::vector<double> gpa2 = {3.45, 3.80, 2.90, 3.65, 2.10, 3.92, 2.75, 3.50, 1.95, 3.30};

    // 3. en buyuk elemani dogru konuma yerlestir
    std::nth_element(gpa2.begin(),
                     gpa2.begin() + 3,
                     gpa2.end(),
                     std::greater<double>());

    std::cout << "En yuksek 3: ";
    for (int i = 0; i < 3; ++i) {
        std::cout << gpa2[i] << " ";
    }
    std::cout << "\n";

    // --- 5. OBYS: Ogrenci gruplama ---
    std::cout << "\n--- OBYS: Ogrenci Gruplama ---\n";
    std::vector<Ogrenci> tum_sinif = {
        {1001, "Ali Yilmaz",   3.45},
        {1002, "Ayse Kaya",    3.80},
        {1003, "Mehmet Demir", 2.90},
        {1004, "Zeynep Can",   3.65},
        {1005, "Can Arslan",   2.10},
        {1006, "Elif Yildiz",  3.92},
        {1007, "Burak Ozturk", 2.75},
        {1008, "Selin Aktas",  3.50}
    };

    // 3 gruba ayir: Onur (>= 3.5), Normal (2.0-3.5), Kosullu (< 2.0)
    // Once onur ogrencileri ayir
    auto onur_sinir = std::stable_partition(tum_sinif.begin(), tum_sinif.end(),
        [](const Ogrenci& o) { return o.gpa >= 3.5; });

    // Geri kalan icinde kosullu olanlari arkaya at
    auto kosullu_sinir = std::stable_partition(onur_sinir, tum_sinif.end(),
        [](const Ogrenci& o) { return o.gpa >= 2.0; });

    std::cout << "=== Onur Listesi (GPA >= 3.5) ===\n";
    for (auto it = tum_sinif.begin(); it != onur_sinir; ++it) {
        std::cout << "  " << *it << "\n";
    }

    std::cout << "\n=== Normal (2.0 - 3.5) ===\n";
    for (auto it = onur_sinir; it != kosullu_sinir; ++it) {
        std::cout << "  " << *it << "\n";
    }

    std::cout << "\n=== Kosullu (< 2.0) ===\n";
    for (auto it = kosullu_sinir; it != tum_sinif.end(); ++it) {
        std::cout << "  " << *it << "\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Partition ve nth_element ===
 *
 * --- std::partition ---
 * Orijinal: 85 45 92 38 78 55 91 42 88 60
 * Basarili (>= 60): 85 60 92 88 78 91
 * Basarisiz (< 60): 55 42 38 45
 *
 * --- std::stable_partition ---
 * (orijinal sira korunarak bolumlenir)
 *
 * --- nth_element ---
 * Medyan GPA: 3.30
 *
 * --- OBYS: Ogrenci Gruplama ---
 * (uc gruba ayrilmis ogrenciler)
 */

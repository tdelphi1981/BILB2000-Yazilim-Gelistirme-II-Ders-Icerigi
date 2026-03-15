/**
 * Remove-Erase Idiomu ve C++20 std::erase
 *
 * Bu program remove-erase idiomunu ve C++20 alternatiflerini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - std::remove neden gercekten silmez
 * - Klasik remove-erase idiomu
 * - C++20 std::erase ve std::erase_if
 * - Farkli kapsayicilarda silme stratejileri
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 2 - Temel STL Algoritmalari
 *
 * Derleme: g++ -std=c++20 -o 03_remove_erase_idiom 03_remove_erase_idiom.cpp
 * Calistirma: ./03_remove_erase_idiom
 */

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

// Yardimci: vector icerigini yazdir
template <typename T>
void yazdir(const std::string& baslik, const std::vector<T>& v) {
    std::cout << baslik << ": ";
    for (const auto& e : v) std::cout << e << " ";
    std::cout << "(boyut: " << v.size() << ")\n";
}

int main() {
    std::cout << "=== Remove-Erase Idiomu ===\n\n";

    // --- 1. std::remove gercekten silmez! ---
    std::cout << "--- std::remove Davranisi ---\n";
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};
    yazdir("Oncesi", v);

    // remove, 2'leri "siler" ama aslinda sadece tasir
    auto yeni_son = std::remove(v.begin(), v.end(), 2);

    std::cout << "remove sonrasi:\n";
    std::cout << "  Gecerli aralik [begin, yeni_son): ";
    for (auto it = v.begin(); it != yeni_son; ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "  Tum vector (kirli bolgeli): ";
    for (int x : v) std::cout << x << " ";
    std::cout << "(boyut HALA: " << v.size() << ")\n";

    // --- 2. Klasik Remove-Erase Idiomu ---
    std::cout << "\n--- Klasik Remove-Erase Idiomu ---\n";
    std::vector<int> v2 = {1, 2, 3, 2, 4, 2, 5};
    yazdir("Oncesi", v2);

    // Adim 1: remove tasir, Adim 2: erase gercekten siler
    v2.erase(std::remove(v2.begin(), v2.end(), 2), v2.end());

    yazdir("Sonrasi", v2);

    // remove_if ile kosullu silme
    std::vector<int> notlar = {85, 45, 92, 38, 78, 55, 91, 42};
    yazdir("\nNotlar", notlar);

    // 50 altini sil
    notlar.erase(
        std::remove_if(notlar.begin(), notlar.end(),
                       [](int n) { return n < 50; }),
        notlar.end());

    yazdir("50 alti silindi", notlar);

    // --- 3. C++20 std::erase / std::erase_if ---
    std::cout << "\n--- C++20: std::erase / std::erase_if ---\n";

    // vector icin
    std::vector<int> v3 = {1, 2, 3, 2, 4, 2, 5};
    yazdir("Oncesi", v3);

    auto silinen = std::erase(v3, 2);
    yazdir("erase(v3, 2)", v3);
    std::cout << "  Silinen eleman sayisi: " << silinen << "\n";

    // erase_if ile kosullu
    std::vector<int> v4 = {10, 25, 30, 45, 50, 65, 70};
    yazdir("\nOncesi", v4);

    auto silinen2 = std::erase_if(v4, [](int n) { return n % 2 != 0; });
    yazdir("Tekler silindi", v4);
    std::cout << "  Silinen: " << silinen2 << " eleman\n";

    // --- 4. Farkli kapsayicilarda erase_if ---
    std::cout << "\n--- Farkli Kapsayicilarda erase_if ---\n";

    // list icin
    std::list<int> liste = {1, 2, 3, 4, 5, 6, 7, 8};
    std::erase_if(liste, [](int n) { return n % 2 == 0; });
    std::cout << "list (ciftler silindi): ";
    for (int x : liste) std::cout << x << " ";
    std::cout << "\n";

    // map icin
    std::map<int, std::string> kayit = {
        {1001, "Ali"}, {1002, "Ayse"}, {1003, "Mehmet"},
        {1004, "Zeynep"}, {1005, "Can"}
    };
    // Numarasi 1003'ten buyuk olanlari sil
    std::erase_if(kayit, [](const auto& cift) {
        return cift.first > 1003;
    });
    std::cout << "map (>1003 silindi): ";
    for (const auto& [no, isim] : kayit) {
        std::cout << no << ":" << isim << " ";
    }
    std::cout << "\n";

    // set icin
    std::set<int> kume = {10, 20, 30, 40, 50, 60};
    std::erase_if(kume, [](int n) { return n > 40; });
    std::cout << "set (>40 silindi): ";
    for (int x : kume) std::cout << x << " ";
    std::cout << "\n";

    // --- 5. OBYS: Dersten cekilen ogrencileri sil ---
    std::cout << "\n--- OBYS: Dersten Cekilme ---\n";
    std::vector<Ogrenci> ders_listesi = {
        {1001, "Ali Yilmaz",   3.45},
        {1002, "Ayse Kaya",    3.80},
        {1003, "Mehmet Demir", 2.90},
        {1004, "Zeynep Can",   3.65},
        {1005, "Can Arslan",   2.10}
    };

    std::cout << "Ders listesi (" << ders_listesi.size() << " kisi):\n";
    for (const auto& o : ders_listesi) {
        std::cout << "  " << o.numara << ": " << o.isim << "\n";
    }

    // 1003 ve 1005 numarali ogrenciler cekildi
    std::set<int> cekilen = {1003, 1005};
    std::erase_if(ders_listesi, [&cekilen](const Ogrenci& o) {
        return cekilen.contains(o.numara);
    });

    std::cout << "\nCekilmeler sonrasi (" << ders_listesi.size() << " kisi):\n";
    for (const auto& o : ders_listesi) {
        std::cout << "  " << o.numara << ": " << o.isim << "\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Remove-Erase Idiomu ===
 *
 * --- std::remove Davranisi ---
 * Oncesi: 1 2 3 2 4 2 5 (boyut: 7)
 * remove sonrasi:
 *   Gecerli aralik [begin, yeni_son): 1 3 4 5
 *   Tum vector (kirli bolgeli): 1 3 4 5 4 2 5 (boyut HALA: 7)
 *
 * --- Klasik Remove-Erase Idiomu ---
 * Oncesi: 1 2 3 2 4 2 5 (boyut: 7)
 * Sonrasi: 1 3 4 5 (boyut: 4)
 *
 * Notlar: 85 45 92 38 78 55 91 42 (boyut: 8)
 * 50 alti silindi: 85 92 78 55 91 (boyut: 5)
 *
 * --- C++20: std::erase / std::erase_if ---
 * Oncesi: 1 2 3 2 4 2 5 (boyut: 7)
 * erase(v3, 2): 1 3 4 5 (boyut: 4)
 *   Silinen eleman sayisi: 3
 *
 * Oncesi: 10 25 30 45 50 65 70 (boyut: 7)
 * Tekler silindi: 10 30 50 70 (boyut: 4)
 *   Silinen: 3 eleman
 *
 * --- Farkli Kapsayicilarda erase_if ---
 * list (ciftler silindi): 1 3 5 7
 * map (>1003 silindi): 1001:Ali 1002:Ayse 1003:Mehmet
 * set (>40 silindi): 10 20 30 40
 *
 * --- OBYS: Dersten Cekilme ---
 * Ders listesi (5 kisi):
 *   1001: Ali Yilmaz
 *   1002: Ayse Kaya
 *   1003: Mehmet Demir
 *   1004: Zeynep Can
 *   1005: Can Arslan
 *
 * Cekilmeler sonrasi (3 kisi):
 *   1001: Ali Yilmaz
 *   1002: Ayse Kaya
 *   1004: Zeynep Can
 */

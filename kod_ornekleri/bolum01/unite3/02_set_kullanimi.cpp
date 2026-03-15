/**
 * set Kullanimi - Benzersiz Ders Kodlari Yonetimi
 *
 * Bu program std::set kullanarak benzersiz elemanlarin
 * yonetimini, kume islemlerini ve multiset kullanımini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - std::set olusturma ve eleman ekleme
 * - Benzersizlik kontrolu (insert donus degeri)
 * - find(), contains() ile arama
 * - Kume islemleri: birlesim, kesisim, fark
 * - std::multiset ile tekrarli elemanlar
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 3 - Iliskisel Kapsayicilar
 *
 * Derleme: g++ -std=c++20 -o 02_set_kullanimi 02_set_kullanimi.cpp
 * Calistirma: ./02_set_kullanimi
 */

#include <iostream>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>

int main() {
    // --- 1. set Olusturma ---
    std::cout << "=== set Olusturma ===\n\n";

    std::set<std::string> ders_kodlari = {
        "MAT101", "FIZ102", "BIL201", "KIM103"
    };

    // Sirali cikti
    std::cout << "Ders kodlari (sirali):\n";
    for (const auto& kod : ders_kodlari) {
        std::cout << "  " << kod << "\n";
    }

    // --- 2. Benzersizlik Kontrolu ---
    std::cout << "\n=== Benzersizlik Kontrolu ===\n\n";

    // Yeni ders ekle
    auto [it1, eklendi1] = ders_kodlari.insert("ELE104");
    std::cout << "ELE104 ekleme: "
              << (eklendi1 ? "Basarili" : "Zaten mevcut") << "\n";

    // Tekrar eklemeye calis
    auto [it2, eklendi2] = ders_kodlari.insert("MAT101");
    std::cout << "MAT101 ekleme: "
              << (eklendi2 ? "Basarili" : "Zaten mevcut") << "\n";

    std::cout << "Toplam ders: " << ders_kodlari.size() << "\n";

    // --- 3. Arama ---
    std::cout << "\n=== Arama ===\n\n";

    if (ders_kodlari.contains("BIL201")) {
        std::cout << "BIL201 kayitli.\n";
    }

    auto it = ders_kodlari.find("TRK105");
    if (it == ders_kodlari.end()) {
        std::cout << "TRK105 kayitli degil.\n";
    }

    // --- 4. Kume Islemleri ---
    std::cout << "\n=== Kume Islemleri ===\n\n";

    std::set<int> A = {1, 2, 3, 4, 5};
    std::set<int> B = {3, 4, 5, 6, 7};

    // Birlesim
    std::set<int> birlesim;
    std::set_union(A.begin(), A.end(),
                   B.begin(), B.end(),
                   std::inserter(birlesim, birlesim.begin()));

    std::cout << "A = {";
    for (auto it = A.begin(); it != A.end(); ++it) {
        if (it != A.begin()) std::cout << ", ";
        std::cout << *it;
    }
    std::cout << "}\n";

    std::cout << "B = {";
    for (auto it = B.begin(); it != B.end(); ++it) {
        if (it != B.begin()) std::cout << ", ";
        std::cout << *it;
    }
    std::cout << "}\n\n";

    std::cout << "Birlesim (A U B) = {";
    for (auto it = birlesim.begin(); it != birlesim.end(); ++it) {
        if (it != birlesim.begin()) std::cout << ", ";
        std::cout << *it;
    }
    std::cout << "}\n";

    // Kesisim
    std::set<int> kesisim;
    std::set_intersection(A.begin(), A.end(),
                          B.begin(), B.end(),
                          std::inserter(kesisim, kesisim.begin()));

    std::cout << "Kesisim (A n B) = {";
    for (auto it = kesisim.begin(); it != kesisim.end(); ++it) {
        if (it != kesisim.begin()) std::cout << ", ";
        std::cout << *it;
    }
    std::cout << "}\n";

    // Fark
    std::set<int> fark;
    std::set_difference(A.begin(), A.end(),
                        B.begin(), B.end(),
                        std::inserter(fark, fark.begin()));

    std::cout << "Fark (A - B)    = {";
    for (auto it = fark.begin(); it != fark.end(); ++it) {
        if (it != fark.begin()) std::cout << ", ";
        std::cout << *it;
    }
    std::cout << "}\n";

    // --- 5. multiset ---
    std::cout << "\n=== multiset ===\n\n";

    std::multiset<int> notlar = {85, 92, 78, 85, 95, 70, 85, 92, 88};

    std::cout << "Sinav notlari (sirali): ";
    for (const auto& n : notlar) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    std::cout << "Toplam not sayisi: " << notlar.size() << "\n";
    std::cout << "85 alan ogrenci: " << notlar.count(85) << "\n";
    std::cout << "En dusuk: " << *notlar.begin() << "\n";
    std::cout << "En yuksek: " << *notlar.rbegin() << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ----------------
 * === set Olusturma ===
 *
 * Ders kodlari (sirali):
 *   BIL201
 *   FIZ102
 *   KIM103
 *   MAT101
 *
 * === Benzersizlik Kontrolu ===
 *
 * ELE104 ekleme: Basarili
 * MAT101 ekleme: Zaten mevcut
 * Toplam ders: 5
 *
 * === Arama ===
 *
 * BIL201 kayitli.
 * TRK105 kayitli degil.
 *
 * === Kume Islemleri ===
 *
 * A = {1, 2, 3, 4, 5}
 * B = {3, 4, 5, 6, 7}
 *
 * Birlesim (A U B) = {1, 2, 3, 4, 5, 6, 7}
 * Kesisim (A n B) = {3, 4, 5}
 * Fark (A - B)    = {1, 2}
 *
 * === multiset ===
 *
 * Sinav notlari (sirali): 70 78 85 85 85 88 92 92 95
 * Toplam not sayisi: 9
 * 85 alan ogrenci: 3
 * En dusuk: 70
 * En yuksek: 95
 */

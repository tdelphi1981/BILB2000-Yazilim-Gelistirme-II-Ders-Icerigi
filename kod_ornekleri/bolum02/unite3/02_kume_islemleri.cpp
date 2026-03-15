/**
 * Kume Islemleri - set_union, intersection, difference
 *
 * Bu program STL kume algoritmalarini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - std::set_union, std::set_intersection
 * - std::set_difference, std::set_symmetric_difference
 * - std::includes, std::merge
 * - Sirali araliklar gerektirmesi
 * - OBYS'de ders kesisimleri
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 3 - Ileri STL Algoritmalari
 *
 * Derleme: g++ -std=c++20 -o 02_kume_islemleri 02_kume_islemleri.cpp
 * Calistirma: ./02_kume_islemleri
 */

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <string>

// Yardimci: set icerigini yazdir
template <typename T>
void yazdir_set(const std::string& baslik, const std::set<T>& s) {
    std::cout << baslik << ": {";
    bool ilk = true;
    for (const auto& e : s) {
        if (!ilk) std::cout << ", ";
        std::cout << e;
        ilk = false;
    }
    std::cout << "}\n";
}

int main() {
    std::cout << "=== Kume Islemleri ===\n\n";

    // --- 1. std::set_intersection (Kesisim) ---
    std::cout << "--- set_intersection (Kesisim) ---\n";
    std::set<int> A = {1, 2, 3, 4, 5};
    std::set<int> B = {3, 4, 5, 6, 7};

    std::set<int> kesisim;
    std::set_intersection(A.begin(), A.end(),
                          B.begin(), B.end(),
                          std::inserter(kesisim, kesisim.begin()));

    yazdir_set("A", A);
    yazdir_set("B", B);
    yazdir_set("A kesisim B", kesisim);

    // --- 2. std::set_union (Birlesim) ---
    std::cout << "\n--- set_union (Birlesim) ---\n";
    std::set<int> birlesim;
    std::set_union(A.begin(), A.end(),
                   B.begin(), B.end(),
                   std::inserter(birlesim, birlesim.begin()));

    yazdir_set("A birlesim B", birlesim);

    // --- 3. std::set_difference (Fark) ---
    std::cout << "\n--- set_difference (Fark) ---\n";
    std::set<int> fark_AB;
    std::set_difference(A.begin(), A.end(),
                        B.begin(), B.end(),
                        std::inserter(fark_AB, fark_AB.begin()));

    std::set<int> fark_BA;
    std::set_difference(B.begin(), B.end(),
                        A.begin(), A.end(),
                        std::inserter(fark_BA, fark_BA.begin()));

    yazdir_set("A - B (sadece A'da)", fark_AB);
    yazdir_set("B - A (sadece B'de)", fark_BA);

    // --- 4. std::set_symmetric_difference (Simetrik Fark) ---
    std::cout << "\n--- set_symmetric_difference ---\n";
    std::set<int> simetrik;
    std::set_symmetric_difference(A.begin(), A.end(),
                                   B.begin(), B.end(),
                                   std::inserter(simetrik, simetrik.begin()));

    yazdir_set("A simetrik fark B", simetrik);

    // --- 5. std::includes ---
    std::cout << "\n--- std::includes ---\n";
    std::set<int> alt_kume = {3, 4, 5};
    bool icerir_mi = std::includes(A.begin(), A.end(),
                                    alt_kume.begin(), alt_kume.end());
    yazdir_set("Alt kume", alt_kume);
    std::cout << "A, alt kumeyi icerir mi? "
              << (icerir_mi ? "Evet" : "Hayir") << "\n";

    std::set<int> baska = {3, 4, 8};
    bool icerir2 = std::includes(A.begin(), A.end(),
                                  baska.begin(), baska.end());
    yazdir_set("Baska kume", baska);
    std::cout << "A, baska kumeyi icerir mi? "
              << (icerir2 ? "Evet" : "Hayir") << "\n";

    // --- 6. std::merge (Sirali birlestirme) ---
    std::cout << "\n--- std::merge ---\n";
    std::vector<int> sirali1 = {1, 3, 5, 7};
    std::vector<int> sirali2 = {2, 4, 6, 8};
    std::vector<int> birlesmis;

    std::merge(sirali1.begin(), sirali1.end(),
               sirali2.begin(), sirali2.end(),
               std::back_inserter(birlesmis));

    std::cout << "Birlesmis sirali: ";
    for (int x : birlesmis) std::cout << x << " ";
    std::cout << "\n";

    // --- 7. OBYS: Ogrenci ders kesisimleri ---
    std::cout << "\n--- OBYS: Ders Kesisimleri ---\n";

    std::set<std::string> ali_dersleri = {
        "C++", "Fizik", "Matematik", "Veritabani"
    };
    std::set<std::string> ayse_dersleri = {
        "C++", "Kimya", "Matematik", "Istatistik"
    };
    std::set<std::string> mehmet_dersleri = {
        "C++", "Elektronik", "Fizik", "Matematik"
    };

    yazdir_set("Ali'nin dersleri", ali_dersleri);
    yazdir_set("Ayse'nin dersleri", ayse_dersleri);
    yazdir_set("Mehmet'in dersleri", mehmet_dersleri);

    // Ali ve Ayse'nin ortak dersleri
    std::set<std::string> ortak;
    std::set_intersection(ali_dersleri.begin(), ali_dersleri.end(),
                          ayse_dersleri.begin(), ayse_dersleri.end(),
                          std::inserter(ortak, ortak.begin()));
    yazdir_set("\nAli-Ayse ortak", ortak);

    // Uc ogrencinin ortak dersleri
    std::set<std::string> uc_ortak;
    std::set_intersection(ortak.begin(), ortak.end(),
                          mehmet_dersleri.begin(), mehmet_dersleri.end(),
                          std::inserter(uc_ortak, uc_ortak.begin()));
    yazdir_set("Uc ogrenci ortak", uc_ortak);

    // Tum acilan dersler (birlesim)
    std::set<std::string> tum_dersler;
    std::set_union(ali_dersleri.begin(), ali_dersleri.end(),
                   ayse_dersleri.begin(), ayse_dersleri.end(),
                   std::inserter(tum_dersler, tum_dersler.begin()));
    std::set<std::string> tum_dersler2;
    std::set_union(tum_dersler.begin(), tum_dersler.end(),
                   mehmet_dersleri.begin(), mehmet_dersleri.end(),
                   std::inserter(tum_dersler2, tum_dersler2.begin()));
    yazdir_set("Tum acilan dersler", tum_dersler2);

    // Sadece Ali'nin aldigi dersler (fark)
    std::set<std::string> sadece_ali;
    std::set<std::string> diger_dersler;
    std::set_union(ayse_dersleri.begin(), ayse_dersleri.end(),
                   mehmet_dersleri.begin(), mehmet_dersleri.end(),
                   std::inserter(diger_dersler, diger_dersler.begin()));
    std::set_difference(ali_dersleri.begin(), ali_dersleri.end(),
                        diger_dersler.begin(), diger_dersler.end(),
                        std::inserter(sadece_ali, sadece_ali.begin()));
    yazdir_set("Sadece Ali'de olan", sadece_ali);

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Kume Islemleri ===
 *
 * --- set_intersection (Kesisim) ---
 * A: {1, 2, 3, 4, 5}
 * B: {3, 4, 5, 6, 7}
 * A kesisim B: {3, 4, 5}
 *
 * --- set_union (Birlesim) ---
 * A birlesim B: {1, 2, 3, 4, 5, 6, 7}
 *
 * --- set_difference (Fark) ---
 * A - B (sadece A'da): {1, 2}
 * B - A (sadece B'de): {6, 7}
 *
 * --- set_symmetric_difference ---
 * A simetrik fark B: {1, 2, 6, 7}
 *
 * --- std::includes ---
 * Alt kume: {3, 4, 5}
 * A, alt kumeyi icerir mi? Evet
 * Baska kume: {3, 4, 8}
 * A, baska kumeyi icerir mi? Hayir
 *
 * --- std::merge ---
 * Birlesmis sirali: 1 2 3 4 5 6 7 8
 *
 * --- OBYS: Ders Kesisimleri ---
 * Ali'nin dersleri: {C++, Fizik, Matematik, Veritabani}
 * Ayse'nin dersleri: {C++, Istatistik, Kimya, Matematik}
 * Mehmet'in dersleri: {C++, Elektronik, Fizik, Matematik}
 *
 * Ali-Ayse ortak: {C++, Matematik}
 * Uc ogrenci ortak: {C++, Matematik}
 * Tum acilan dersler: {C++, Elektronik, Fizik, Istatistik, Kimya, Matematik, Veritabani}
 * Sadece Ali'de olan: {Veritabani}
 */

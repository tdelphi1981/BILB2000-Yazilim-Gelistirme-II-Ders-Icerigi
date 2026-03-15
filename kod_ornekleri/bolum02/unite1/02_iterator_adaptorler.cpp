/**
 * Iterator Adaptorleri - Ters, Tasima ve Ekleme Iteratorleri
 *
 * Bu program iterator adaptorlerini ve stream iteratorlerini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - reverse_iterator ile ters dolasim
 * - move_iterator ile tasima semantigi
 * - back_inserter, front_inserter, inserter
 * - istream_iterator, ostream_iterator
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 1 - Iterator Kavrami
 *
 * Derleme: g++ -std=c++20 -o 02_iterator_adaptorler 02_iterator_adaptorler.cpp
 * Calistirma: ./02_iterator_adaptorler
 */

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <numeric>
#include <memory>

struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

int main() {
    std::cout << "=== Iterator Adaptorleri ===\n\n";

    // --- 1. reverse_iterator ---
    std::cout << "--- reverse_iterator ---\n";
    std::vector<int> notlar = {85, 92, 78, 95, 88};

    std::cout << "Normal sirada: ";
    for (auto it = notlar.begin(); it != notlar.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "Ters sirada:   ";
    for (auto rit = notlar.rbegin(); rit != notlar.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n";

    // reverse_iterator ile algoritma kullanimi
    // Son 3 notu buyukten kucuge sirala
    std::sort(notlar.rbegin(), notlar.rend());
    std::cout << "Ters siralama: ";
    for (const auto& n : notlar) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // --- 2. back_inserter ---
    std::cout << "\n--- back_inserter ---\n";
    std::vector<int> kaynak = {10, 20, 30, 40, 50};
    std::vector<int> hedef;

    // Sadece cift sayilari hedefe kopyala
    std::copy_if(kaynak.begin(), kaynak.end(),
                 std::back_inserter(hedef),
                 [](int n) { return n % 20 == 0; });

    std::cout << "Filtrelenmis: ";
    for (const auto& n : hedef) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // --- 3. front_inserter ---
    std::cout << "\n--- front_inserter ---\n";
    std::deque<std::string> isimler;

    std::vector<std::string> yeni_isimler = {"Ali", "Ayse", "Mehmet"};
    std::copy(yeni_isimler.begin(), yeni_isimler.end(),
              std::front_inserter(isimler));

    // front_inserter ters sirayla ekler!
    std::cout << "front_inserter sonucu: ";
    for (const auto& isim : isimler) {
        std::cout << isim << " ";
    }
    std::cout << "\n";

    // --- 4. inserter ---
    std::cout << "\n--- inserter ---\n";
    std::set<int> kume1 = {1, 3, 5, 7, 9};
    std::set<int> kume2 = {2, 3, 4, 5, 6};
    std::set<int> kesisim;

    // set_intersection sonucu bir set'e ekle
    std::set_intersection(kume1.begin(), kume1.end(),
                          kume2.begin(), kume2.end(),
                          std::inserter(kesisim, kesisim.begin()));

    std::cout << "Kesisim: ";
    for (const auto& n : kesisim) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // --- 5. move_iterator ---
    std::cout << "\n--- move_iterator ---\n";
    std::vector<std::string> kaynakStr = {"Fizik", "Kimya", "Matematik"};
    std::cout << "Tasinmadan once kaynak boyutu: " << kaynakStr.size() << "\n";

    std::vector<std::string> hedefStr(
        std::make_move_iterator(kaynakStr.begin()),
        std::make_move_iterator(kaynakStr.end()));

    std::cout << "Tasinan hedef: ";
    for (const auto& s : hedefStr) {
        std::cout << s << " ";
    }
    std::cout << "\n";

    // Kaynak stringleri tasindi (ici bos olabilir)
    std::cout << "Kaynak boyutu hala: " << kaynakStr.size()
              << " (ama ici bos stringler)\n";

    // --- 6. ostream_iterator ---
    std::cout << "\n--- ostream_iterator ---\n";
    std::vector<double> gpa_listesi = {3.45, 3.80, 2.90, 3.65, 3.20};

    std::cout << "GPA'lar: ";
    std::copy(gpa_listesi.begin(), gpa_listesi.end(),
              std::ostream_iterator<double>(std::cout, ", "));
    std::cout << "\n";

    // --- 7. istream_iterator ---
    std::cout << "\n--- istream_iterator ---\n";
    // String stream ile simule edelim
    std::istringstream veri("10 20 30 40 50");
    std::vector<int> okunan(
        (std::istream_iterator<int>(veri)),
        std::istream_iterator<int>());  // bos = EOF

    std::cout << "Stream'den okunan: ";
    for (const auto& n : okunan) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // --- 8. OBYS: Iterator adaptorlerinin birlesmesi ---
    std::cout << "\n--- OBYS: Adaptor Ornegi ---\n";
    std::vector<Ogrenci> sinif = {
        {1001, "Ali Yilmaz", 3.45},
        {1002, "Ayse Kaya", 3.80},
        {1003, "Mehmet Demir", 2.90},
        {1004, "Zeynep Can", 3.65},
        {1005, "Can Arslan", 2.10}
    };

    // Basarili ogrencileri ayri listeye kopyala (back_inserter ile)
    std::vector<Ogrenci> basarililar;
    std::copy_if(sinif.begin(), sinif.end(),
                 std::back_inserter(basarililar),
                 [](const Ogrenci& o) { return o.gpa >= 3.0; });

    // Basarililari GPA'ya gore ters sirala
    std::sort(basarililar.begin(), basarililar.end(),
              [](const Ogrenci& a, const Ogrenci& b) {
                  return a.gpa > b.gpa;
              });

    std::cout << "Basarili ogrenciler (GPA sirasinda):\n";
    for (const auto& o : basarililar) {
        std::cout << "  " << o.numara << ": " << o.isim
                  << " (GPA: " << o.gpa << ")\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Iterator Adaptorleri ===
 *
 * --- reverse_iterator ---
 * Normal sirada: 85 92 78 95 88
 * Ters sirada:   88 95 78 92 85
 * Ters siralama: 95 92 88 85 78
 *
 * --- back_inserter ---
 * Filtrelenmis: 20 40
 *
 * --- front_inserter ---
 * front_inserter sonucu: Mehmet Ayse Ali
 *
 * --- inserter ---
 * Kesisim: 3 5
 *
 * --- move_iterator ---
 * Tasinmadan once kaynak boyutu: 3
 * Tasinan hedef: Fizik Kimya Matematik
 * Kaynak boyutu hala: 3 (ama ici bos stringler)
 *
 * --- ostream_iterator ---
 * GPA'lar: 3.45, 3.8, 2.9, 3.65, 3.2,
 *
 * --- istream_iterator ---
 * Stream'den okunan: 10 20 30 40 50
 *
 * --- OBYS: Adaptor Ornegi ---
 * Basarili ogrenciler (GPA sirasinda):
 *   1002: Ayse Kaya (GPA: 3.8)
 *   1004: Zeynep Can (GPA: 3.65)
 *   1001: Ali Yilmaz (GPA: 3.45)
 */

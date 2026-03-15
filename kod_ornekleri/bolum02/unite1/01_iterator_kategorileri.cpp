/**
 * Iterator Kategorileri - Alti Kategori ve Yetenekleri
 *
 * Bu program farkli iterator kategorilerini ve her birinin
 * destekledigi islemleri gosterir.
 *
 * Ogrenilecek kavramlar:
 * - Input, Output, Forward, Bidirectional, RandomAccess, Contiguous iterator
 * - Her kategorinin destekledigi islemler
 * - Kapsayicilar ve iterator kategorileri iliskisi
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 1 - Iterator Kavrami
 *
 * Derleme: g++ -std=c++20 -o 01_iterator_kategorileri 01_iterator_kategorileri.cpp
 * Calistirma: ./01_iterator_kategorileri
 */

#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <map>
#include <set>
#include <deque>
#include <array>
#include <iterator>
#include <algorithm>
#include <string>
#include <type_traits>
#include <memory>

// Iterator kategorisini string olarak donduren yardimci fonksiyon
template <typename Iter>
std::string iterator_kategorisi() {
    using category = typename std::iterator_traits<Iter>::iterator_category;

    if constexpr (std::is_same_v<category, std::input_iterator_tag>)
        return "Input Iterator";
    else if constexpr (std::is_same_v<category, std::output_iterator_tag>)
        return "Output Iterator";
    else if constexpr (std::is_same_v<category, std::forward_iterator_tag>)
        return "Forward Iterator";
    else if constexpr (std::is_same_v<category, std::bidirectional_iterator_tag>)
        return "Bidirectional Iterator";
    else if constexpr (std::is_same_v<category, std::random_access_iterator_tag>)
        return "Random Access Iterator";
    else
        return "Bilinmeyen Kategori";
}

// OBYS icin basit Ogrenci yapisi
struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;
};

int main() {
    std::cout << "=== Iterator Kategorileri ===\n\n";

    // --- Kapsayicilarin iterator kategorileri ---
    std::cout << "--- Kapsayici Iterator Kategorileri ---\n";
    std::cout << "vector<int>      : "
              << iterator_kategorisi<std::vector<int>::iterator>() << "\n";
    std::cout << "deque<int>       : "
              << iterator_kategorisi<std::deque<int>::iterator>() << "\n";
    std::cout << "list<int>        : "
              << iterator_kategorisi<std::list<int>::iterator>() << "\n";
    std::cout << "forward_list<int>: "
              << iterator_kategorisi<std::forward_list<int>::iterator>() << "\n";
    std::cout << "map<int,string>  : "
              << iterator_kategorisi<std::map<int,std::string>::iterator>() << "\n";
    std::cout << "set<int>         : "
              << iterator_kategorisi<std::set<int>::iterator>() << "\n";

    // --- Random Access Iterator islemleri ---
    std::cout << "\n--- Random Access Iterator Islemleri ---\n";
    std::vector<int> notlar = {85, 92, 78, 95, 88, 72, 91};

    auto it = notlar.begin();
    std::cout << "*it (ilk eleman)   : " << *it << "\n";

    ++it;
    std::cout << "++it (ikinci eleman): " << *it << "\n";

    it += 3;
    std::cout << "it += 3             : " << *it << "\n";

    std::cout << "it[1] (sonraki)     : " << it[1] << "\n";

    auto mesafe = notlar.end() - notlar.begin();
    std::cout << "end - begin         : " << mesafe << " eleman\n";

    // --- Bidirectional Iterator islemleri ---
    std::cout << "\n--- Bidirectional Iterator (list) ---\n";
    std::list<std::string> isimler = {"Ali", "Ayse", "Mehmet", "Zeynep"};

    auto lit = isimler.end();
    --lit; // Son elemana git
    std::cout << "Son eleman: " << *lit << "\n";
    --lit; // Bir oncekine
    std::cout << "Sondan bir onceki: " << *lit << "\n";

    // list iterator'u ile rastgele erisim YAPILAMAZ:
    // lit += 2; // DERLEME HATASI!
    // lit[0];   // DERLEME HATASI!

    // --- Forward Iterator islemleri ---
    std::cout << "\n--- Forward Iterator (forward_list) ---\n";
    std::forward_list<int> flist = {10, 20, 30, 40, 50};

    auto fit = flist.begin();
    std::cout << "Ileri dolasim: ";
    while (fit != flist.end()) {
        std::cout << *fit << " ";
        ++fit;
    }
    std::cout << "\n";
    // --fit; // DERLEME HATASI! Geri gidemez

    // --- OBYS: map iterator kullanimi ---
    std::cout << "\n--- OBYS: map Iterator Kullanimi ---\n";
    std::map<int, std::shared_ptr<Ogrenci>> ogrenciler;
    ogrenciler[1001] = std::make_shared<Ogrenci>(Ogrenci{1001, "Ali Yilmaz", 3.45});
    ogrenciler[1002] = std::make_shared<Ogrenci>(Ogrenci{1002, "Ayse Kaya", 3.80});
    ogrenciler[1003] = std::make_shared<Ogrenci>(Ogrenci{1003, "Mehmet Demir", 2.90});
    ogrenciler[1004] = std::make_shared<Ogrenci>(Ogrenci{1004, "Zeynep Can", 3.65});

    // map bidirectional iterator sunar
    std::cout << "Ogrenciler (sirali dolasim):\n";
    for (auto mit = ogrenciler.begin(); mit != ogrenciler.end(); ++mit) {
        std::cout << "  " << mit->first << ": "
                  << mit->second->isim << " (GPA: "
                  << mit->second->gpa << ")\n";
    }

    // Ters dolasim (bidirectional oldugu icin mumkun)
    std::cout << "\nOgrenciler (ters dolasim):\n";
    for (auto rit = ogrenciler.rbegin(); rit != ogrenciler.rend(); ++rit) {
        std::cout << "  " << rit->first << ": "
                  << rit->second->isim << "\n";
    }

    // --- std::distance ve std::advance ---
    std::cout << "\n--- distance ve advance ---\n";
    std::list<int> liste = {10, 20, 30, 40, 50};
    auto basla = liste.begin();
    auto bitis = liste.end();

    // distance: iki iterator arasi mesafe
    std::cout << "Eleman sayisi: " << std::distance(basla, bitis) << "\n";

    // advance: iteratoru n adim ilerlet
    std::advance(basla, 2);
    std::cout << "2 adim ilerletildi: " << *basla << "\n";

    // C++20: std::next ve std::prev
    auto ucuncu = std::next(liste.begin(), 2);
    std::cout << "next(begin, 2): " << *ucuncu << "\n";

    auto sondan_ikinci = std::prev(liste.end(), 2);
    std::cout << "prev(end, 2): " << *sondan_ikinci << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Iterator Kategorileri ===
 *
 * --- Kapsayici Iterator Kategorileri ---
 * vector<int>      : Random Access Iterator
 * deque<int>       : Random Access Iterator
 * list<int>        : Bidirectional Iterator
 * forward_list<int>: Forward Iterator
 * map<int,string>  : Bidirectional Iterator
 * set<int>         : Bidirectional Iterator
 *
 * --- Random Access Iterator Islemleri ---
 * *it (ilk eleman)   : 85
 * ++it (ikinci eleman): 92
 * it += 3             : 95
 * it[1] (sonraki)     : 88
 * end - begin         : 7 eleman
 *
 * --- Bidirectional Iterator (list) ---
 * Son eleman: Zeynep
 * Sondan bir onceki: Mehmet
 *
 * --- Forward Iterator (forward_list) ---
 * Ileri dolasim: 10 20 30 40 50
 *
 * --- OBYS: map Iterator Kullanimi ---
 * Ogrenciler (sirali dolasim):
 *   1001: Ali Yilmaz (GPA: 3.45)
 *   1002: Ayse Kaya (GPA: 3.8)
 *   1003: Mehmet Demir (GPA: 2.9)
 *   1004: Zeynep Can (GPA: 3.65)
 *
 * Ogrenciler (ters dolasim):
 *   1004: Zeynep Can
 *   1003: Mehmet Demir
 *   1002: Ayse Kaya
 *   1001: Ali Yilmaz
 *
 * --- distance ve advance ---
 * Eleman sayisi: 5
 * 2 adim ilerletildi: 30
 * next(begin, 2): 30
 * prev(end, 2): 40
 */

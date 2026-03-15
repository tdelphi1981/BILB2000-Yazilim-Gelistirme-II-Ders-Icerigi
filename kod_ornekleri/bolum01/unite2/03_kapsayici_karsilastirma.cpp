/**
 * Kapsayici Karsilastirma - Performans Olcumu
 *
 * Bu program farkli sirali kapsayicilarin ekleme,
 * silme ve gezinme performanslarini karsilastirir.
 *
 * Ogrenilecek kavramlar:
 * - Farkli kapsayicilarin pratik performans farklari
 * - Onbellek (cache) etkisinin onemi
 * - Dogru kapsayici seciminin performansa etkisi
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 2 - Sirali Kapsayicilar
 *
 * Derleme: g++ -std=c++20 -O2 -o 03_kapsayici_karsilastirma 03_kapsayici_karsilastirma.cpp
 * Calistirma: ./03_kapsayici_karsilastirma
 */

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <string>

// Zaman olcumu icin yardimci sinif
class Zamanlayici {
    std::chrono::high_resolution_clock::time_point baslangic;
public:
    Zamanlayici() : baslangic(std::chrono::high_resolution_clock::now()) {}

    long long mikrosaniye() const {
        auto bitis = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(bitis - baslangic).count();
    }
};

constexpr int N = 100000;

void sona_ekleme_testi() {
    std::cout << "=== Sona Ekleme Testi (" << N << " eleman) ===" << '\n';

    // vector
    {
        Zamanlayici z;
        std::vector<int> v;
        for (int i = 0; i < N; ++i) v.push_back(i);
        std::cout << "  vector:  " << std::setw(8) << z.mikrosaniye() << " us" << '\n';
    }

    // deque
    {
        Zamanlayici z;
        std::deque<int> d;
        for (int i = 0; i < N; ++i) d.push_back(i);
        std::cout << "  deque:   " << std::setw(8) << z.mikrosaniye() << " us" << '\n';
    }

    // list
    {
        Zamanlayici z;
        std::list<int> l;
        for (int i = 0; i < N; ++i) l.push_back(i);
        std::cout << "  list:    " << std::setw(8) << z.mikrosaniye() << " us" << '\n';
    }

    std::cout << '\n';
}

void basa_ekleme_testi() {
    std::cout << "=== Basa Ekleme Testi (" << N << " eleman) ===" << '\n';

    // vector (O(n) her ekleme - cok yavas!)
    // Kucuk N ile test edelim
    constexpr int KUCUK_N = 10000;

    {
        Zamanlayici z;
        std::vector<int> v;
        for (int i = 0; i < KUCUK_N; ++i) v.insert(v.begin(), i);
        std::cout << "  vector (" << KUCUK_N << "):  "
                  << std::setw(8) << z.mikrosaniye() << " us" << '\n';
    }

    // deque
    {
        Zamanlayici z;
        std::deque<int> d;
        for (int i = 0; i < N; ++i) d.push_front(i);
        std::cout << "  deque  (" << N << "): "
                  << std::setw(8) << z.mikrosaniye() << " us" << '\n';
    }

    // list
    {
        Zamanlayici z;
        std::list<int> l;
        for (int i = 0; i < N; ++i) l.push_front(i);
        std::cout << "  list   (" << N << "): "
                  << std::setw(8) << z.mikrosaniye() << " us" << '\n';
    }

    // forward_list
    {
        Zamanlayici z;
        std::forward_list<int> fl;
        for (int i = 0; i < N; ++i) fl.push_front(i);
        std::cout << "  fwd_list(" << N << "): "
                  << std::setw(8) << z.mikrosaniye() << " us" << '\n';
    }

    std::cout << '\n';
}

void gezinme_testi() {
    std::cout << "=== Gezinme ve Toplama Testi (" << N << " eleman) ===" << '\n';

    // Kapsayicilari hazirla
    std::vector<int> v(N);
    std::iota(v.begin(), v.end(), 1);

    std::deque<int> d(v.begin(), v.end());
    std::list<int> l(v.begin(), v.end());

    // vector gezinme
    {
        Zamanlayici z;
        long long toplam = 0;
        for (const auto& e : v) toplam += e;
        std::cout << "  vector:  " << std::setw(8) << z.mikrosaniye()
                  << " us (toplam=" << toplam << ")" << '\n';
    }

    // deque gezinme
    {
        Zamanlayici z;
        long long toplam = 0;
        for (const auto& e : d) toplam += e;
        std::cout << "  deque:   " << std::setw(8) << z.mikrosaniye()
                  << " us (toplam=" << toplam << ")" << '\n';
    }

    // list gezinme
    {
        Zamanlayici z;
        long long toplam = 0;
        for (const auto& e : l) toplam += e;
        std::cout << "  list:    " << std::setw(8) << z.mikrosaniye()
                  << " us (toplam=" << toplam << ")" << '\n';
    }

    std::cout << '\n';
}

void reserve_etkisi() {
    std::cout << "=== reserve() Etkisi (" << N << " eleman) ===" << '\n';

    // reserve olmadan
    {
        Zamanlayici z;
        std::vector<int> v;
        for (int i = 0; i < N; ++i) v.push_back(i);
        std::cout << "  reserve() yok: " << std::setw(8) << z.mikrosaniye() << " us" << '\n';
    }

    // reserve ile
    {
        Zamanlayici z;
        std::vector<int> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) v.push_back(i);
        std::cout << "  reserve(" << N << "): " << std::setw(8) << z.mikrosaniye() << " us" << '\n';
    }

    std::cout << '\n';
}

int main() {
    std::cout << "Sirali Kapsayici Performans Karsilastirmasi" << '\n';
    std::cout << std::string(45, '=') << '\n' << '\n';

    sona_ekleme_testi();
    basa_ekleme_testi();
    gezinme_testi();
    reserve_etkisi();

    std::cout << "Not: Sonuclar derleyici, isletim sistemi ve donanim" << '\n';
    std::cout << "     ozelliklerine gore degisebilir." << '\n';

    return 0;
}

/*
BEKLENEN CIKTI (yaklasik, sisteme bagli):
------------------------------------------
Sirali Kapsayici Performans Karsilastirmasi
=============================================

=== Sona Ekleme Testi (100000 eleman) ===
  vector:       850 us
  deque:       1200 us
  list:        3500 us

=== Basa Ekleme Testi (100000 eleman) ===
  vector (10000):   45000 us
  deque  (100000):   1100 us
  list   (100000):   3200 us
  fwd_list(100000):  2800 us

=== Gezinme ve Toplama Testi (100000 eleman) ===
  vector:       120 us (toplam=5000050000)
  deque:        180 us (toplam=5000050000)
  list:         450 us (toplam=5000050000)

=== reserve() Etkisi (100000 eleman) ===
  reserve() yok:      850 us
  reserve(100000):    550 us

Not: Sonuclar derleyici, isletim sistemi ve donanim
     ozelliklerine gore degisebilir.
*/

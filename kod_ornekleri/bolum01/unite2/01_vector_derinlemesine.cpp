/**
 * vector Derinlemesine - Kapasite, Buyume Stratejisi ve Performans
 *
 * Bu program vector'un ic yapisini, kapasite yonetimini ve
 * reserve/shrink_to_fit kullanımini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - size() vs capacity() farki
 * - Buyume stratejisi (geometrik artis)
 * - reserve() ile onceden kapasite ayirma
 * - shrink_to_fit() ile fazla kapasitenin iadesi
 * - emplace_back vs push_back performans farki
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 2 - Sirali Kapsayicilar
 *
 * Derleme: g++ -std=c++20 -o 01_vector_derinlemesine 01_vector_derinlemesine.cpp
 * Calistirma: ./01_vector_derinlemesine
 */

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

// Buyume stratejisini gozlemle
void buyume_stratejisi() {
    std::cout << "=== Buyume Stratejisi ===" << '\n';
    std::vector<int> v;
    size_t onceki_kapasite = 0;

    for (int i = 0; i < 20; ++i) {
        v.push_back(i);
        if (v.capacity() != onceki_kapasite) {
            std::cout << "size=" << v.size()
                      << " capacity=" << v.capacity();
            if (onceki_kapasite > 0) {
                std::cout << " (x" << static_cast<double>(v.capacity()) / onceki_kapasite << ")";
            }
            std::cout << '\n';
            onceki_kapasite = v.capacity();
        }
    }
    std::cout << '\n';
}

// reserve() kullanimi
void reserve_ornegi() {
    std::cout << "=== reserve() Kullanimi ===" << '\n';

    // reserve olmadan
    std::vector<int> v1;
    int yeniden_tahsis_1 = 0;
    size_t onceki = 0;
    for (int i = 0; i < 1000; ++i) {
        v1.push_back(i);
        if (v1.capacity() != onceki) {
            ++yeniden_tahsis_1;
            onceki = v1.capacity();
        }
    }
    std::cout << "reserve() olmadan: " << yeniden_tahsis_1
              << " yeniden tahsis" << '\n';

    // reserve ile
    std::vector<int> v2;
    v2.reserve(1000);
    int yeniden_tahsis_2 = 0;
    onceki = v2.capacity();
    for (int i = 0; i < 1000; ++i) {
        v2.push_back(i);
        if (v2.capacity() != onceki) {
            ++yeniden_tahsis_2;
            onceki = v2.capacity();
        }
    }
    std::cout << "reserve(1000) ile: " << yeniden_tahsis_2
              << " yeniden tahsis" << '\n';
    std::cout << '\n';
}

// shrink_to_fit ornegi
void shrink_ornegi() {
    std::cout << "=== shrink_to_fit() ===" << '\n';
    std::vector<int> v(1000);
    std::cout << "Baslangic: size=" << v.size()
              << " capacity=" << v.capacity() << '\n';

    v.resize(10);
    std::cout << "resize(10): size=" << v.size()
              << " capacity=" << v.capacity() << '\n';

    v.shrink_to_fit();
    std::cout << "shrink_to_fit: size=" << v.size()
              << " capacity=" << v.capacity() << '\n';
    std::cout << '\n';
}

// emplace_back vs push_back
struct Ogrenci {
    std::string isim;
    int numara;
    double ortalama;

    Ogrenci(std::string i, int n, double o)
        : isim(std::move(i)), numara(n), ortalama(o) {}
};

void emplace_ornegi() {
    std::cout << "=== emplace_back vs push_back ===" << '\n';
    constexpr int N = 100000;

    // push_back ile
    auto t1 = std::chrono::high_resolution_clock::now();
    std::vector<Ogrenci> v1;
    v1.reserve(N);
    for (int i = 0; i < N; ++i) {
        v1.push_back(Ogrenci{"Ogrenci_" + std::to_string(i), i, 3.5});
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    // emplace_back ile
    auto t3 = std::chrono::high_resolution_clock::now();
    std::vector<Ogrenci> v2;
    v2.reserve(N);
    for (int i = 0; i < N; ++i) {
        v2.emplace_back("Ogrenci_" + std::to_string(i), i, 3.5);
    }
    auto t4 = std::chrono::high_resolution_clock::now();

    auto sure1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto sure2 = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

    std::cout << "push_back:    " << sure1 << " mikrosaniye" << '\n';
    std::cout << "emplace_back: " << sure2 << " mikrosaniye" << '\n';
    std::cout << '\n';
}

int main() {
    buyume_stratejisi();
    reserve_ornegi();
    shrink_ornegi();
    emplace_ornegi();

    return 0;
}

/*
BEKLENEN CIKTI (yaklasik):
--------------------------
=== Buyume Stratejisi ===
size=1 capacity=1
size=2 capacity=2 (x2)
size=3 capacity=4 (x2)
size=5 capacity=8 (x2)
size=9 capacity=16 (x2)
size=17 capacity=32 (x2)

=== reserve() Kullanimi ===
reserve() olmadan: 11 yeniden tahsis
reserve(1000) ile: 0 yeniden tahsis

=== shrink_to_fit() ===
Baslangic: size=1000 capacity=1000
resize(10): size=10 capacity=1000
shrink_to_fit: size=10 capacity=10

=== emplace_back vs push_back ===
push_back:    12500 mikrosaniye
emplace_back: 10800 mikrosaniye
*/

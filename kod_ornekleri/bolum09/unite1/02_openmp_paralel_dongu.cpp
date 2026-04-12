/**
 * OpenMP ile Paralel Dongu
 *
 * Bu program, OpenMP kullanarak basit dongu paralellistirme ve
 * reduction ile guvenli toplama islemini gosterir.
 * OBYS not ortalamasi hesaplama senaryosunu icerir.
 *
 * Ogrenilecek kavramlar:
 * - #pragma omp parallel
 * - #pragma omp parallel for
 * - #pragma omp critical
 * - reduction operasyonu
 * - omp_get_thread_num() ve omp_get_num_threads()
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 1 - Eszamanlilik Kavramlari ve OpenMP
 *
 * Derleme: g++ -std=c++20 -fopenmp -o 02_openmp 02_openmp_paralel_dongu.cpp
 * Calistirma: ./02_openmp
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

// Zaman olcumu icin yardimci fonksiyon
template<typename Func>
double olc(Func f) {
    auto baslangic = std::chrono::high_resolution_clock::now();
    f();
    auto bitis = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> sure = bitis - baslangic;
    return sure.count();
}

int main() {
    // --- Bolum 1: Basit paralel bolge ---
    std::cout << "=== 1. Basit Paralel Bolge ===" << '\n';

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int toplam_izlek = omp_get_num_threads();
        // [NOT] Cikti sirasi her calistirmada farkli olabilir
        #pragma omp critical
        {
            std::cout << "  Izlek " << id
                      << "/" << toplam_izlek << '\n';
        }
    }

    // --- Bolum 2: Paralel dongu ---
    std::cout << "\n=== 2. Paralel Dongu ===" << '\n';

    const int N = 12;
    std::vector<int> kareler(N);

    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        kareler[i] = i * i;
    }

    std::cout << "  Kareler: ";
    for (int x : kareler)
        std::cout << x << ' ';
    std::cout << '\n';

    // --- Bolum 3: OBYS not ortalamasi (reduction ile) ---
    std::cout << "\n=== 3. OBYS Not Ortalamasi ===" << '\n';

    const int OGRENCI_SAYISI = 100000;
    std::vector<double> notlar(OGRENCI_SAYISI);

    // Notlari doldur
    for (int i = 0; i < OGRENCI_SAYISI; ++i)
        notlar[i] = 40.0 + (i % 61); // 40-100 arasi

    // Sirali hesaplama
    double sira_toplam = 0.0;
    double sira_sure = olc([&]() {
        for (int i = 0; i < OGRENCI_SAYISI; ++i)
            sira_toplam += notlar[i];
    });
    std::cout << "  Sirali ortalama : "
              << sira_toplam / OGRENCI_SAYISI
              << " (" << sira_sure << " ms)" << '\n';

    // Paralel hesaplama (reduction ile)
    double par_toplam = 0.0;
    double par_sure = olc([&]() {
        #pragma omp parallel for reduction(+:par_toplam)
        for (int i = 0; i < OGRENCI_SAYISI; ++i)
            par_toplam += notlar[i];
    });
    std::cout << "  Paralel ortalama: "
              << par_toplam / OGRENCI_SAYISI
              << " (" << par_sure << " ms)" << '\n';

    // --- Bolum 4: Kritik bolge ornegi ---
    std::cout << "\n=== 4. Kritik Bolge ===" << '\n';

    std::vector<int> basarili;

    #pragma omp parallel for
    for (int i = 0; i < OGRENCI_SAYISI; ++i) {
        if (notlar[i] >= 60.0) {
            #pragma omp critical
            {
                basarili.push_back(i);
            }
        }
    }

    std::cout << "  Basarili ogrenci sayisi: "
              << basarili.size() << '\n';

    return 0;
}

/*
 * BEKLENEN CIKTI (yaklasik):
 * --------------------------
 * === 1. Basit Paralel Bolge ===
 *   Izlek 0/8
 *   Izlek 3/8
 *   Izlek 1/8
 *   ...
 *
 * === 2. Paralel Dongu ===
 *   Kareler: 0 1 4 9 16 25 36 49 64 81 100 121
 *
 * === 3. OBYS Not Ortalamasi ===
 *   Sirali ortalama : 70.0 (0.15 ms)
 *   Paralel ortalama: 70.0 (0.08 ms)
 *
 * === 4. Kritik Bolge ===
 *   Basarili ogrenci sayisi: 67213
 */

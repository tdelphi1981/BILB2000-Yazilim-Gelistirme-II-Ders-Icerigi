/**
 * OpenMP Reduction Ornegi
 *
 * Bu program, OpenMP reduction islemiyle ogrenci GPA ortalamasi
 * hesaplamayi gosterir. Reduction, her izlegin kendi yerel toplam
 * degiskeniyle calismasini ve sonunda guvenli birlestirme yapmasini saglar.
 *
 * Ogrenilecek kavramlar:
 * - #pragma omp parallel for reduction(+:degisken)
 * - Farkli reduction operatorleri (+, *, min, max)
 * - Paralel ve sirali sonuc karsilastirmasi
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 1 - Eszamanlilik Kavramlari ve OpenMP
 *
 * Derleme: g++ -std=c++20 -fopenmp -o reduction 03_openmp_reduction.cpp
 * Calistirma: ./reduction
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include <cstdlib>
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
    // OBYS senaryosu: 500.000 ogrencinin GPA verileri
    constexpr int OGRENCI_SAYISI = 500'000;
    std::vector<double> gpa_listesi(OGRENCI_SAYISI);

    // GPA degerlerini doldur (0.00 - 4.00 arasi)
    std::srand(42);
    for (int i = 0; i < OGRENCI_SAYISI; ++i) {
        gpa_listesi[i] = (std::rand() % 401) / 100.0; // 0.00 - 4.00
    }

    // ================================================================
    // 1. TOPLAM REDUCTION: Genel GPA ortalamasi
    // ================================================================
    std::cout << "=== 1. Toplam Reduction (+) ===" << '\n';

    // Sirali hesaplama
    double sira_toplam = 0.0;
    double sira_sure = olc([&]() {
        for (int i = 0; i < OGRENCI_SAYISI; ++i) {
            sira_toplam += gpa_listesi[i];
        }
    });
    double sira_ort = sira_toplam / OGRENCI_SAYISI;
    std::cout << "  Sirali  GPA ortalamasi: " << sira_ort
              << " (" << sira_sure << " ms)" << '\n';

    // Paralel hesaplama (reduction ile)
    double par_toplam = 0.0;
    double par_sure = olc([&]() {
        #pragma omp parallel for reduction(+:par_toplam)
        for (int i = 0; i < OGRENCI_SAYISI; ++i) {
            par_toplam += gpa_listesi[i];
        }
    });
    double par_ort = par_toplam / OGRENCI_SAYISI;
    std::cout << "  Paralel GPA ortalamasi: " << par_ort
              << " (" << par_sure << " ms)" << '\n';

    // Dogruluk kontrolu
    double fark = std::abs(sira_ort - par_ort);
    if (fark < 1e-6) {
        std::cout << "  [OK] Sonuclar tutarli!" << '\n';
    } else {
        std::cout << "  [!] Kucuk kayan nokta farki: " << fark << '\n';
    }

    // ================================================================
    // 2. MAKSIMUM REDUCTION: En yuksek GPA
    // ================================================================
    std::cout << "\n=== 2. Maksimum Reduction (max) ===" << '\n';

    double en_yuksek = 0.0;

    #pragma omp parallel for reduction(max:en_yuksek)
    for (int i = 0; i < OGRENCI_SAYISI; ++i) {
        if (gpa_listesi[i] > en_yuksek) {
            en_yuksek = gpa_listesi[i];
        }
    }
    std::cout << "  En yuksek GPA: " << en_yuksek << '\n';

    // ================================================================
    // 3. MINIMUM REDUCTION: En dusuk GPA
    // ================================================================
    std::cout << "\n=== 3. Minimum Reduction (min) ===" << '\n';

    double en_dusuk = 4.0;

    #pragma omp parallel for reduction(min:en_dusuk)
    for (int i = 0; i < OGRENCI_SAYISI; ++i) {
        if (gpa_listesi[i] < en_dusuk) {
            en_dusuk = gpa_listesi[i];
        }
    }
    std::cout << "  En dusuk GPA: " << en_dusuk << '\n';

    // ================================================================
    // 4. SAYMA: Basarili ogrenci sayisi (GPA >= 2.0)
    // ================================================================
    std::cout << "\n=== 4. Sayma Reduction ===" << '\n';

    int basarili_sayisi = 0;

    #pragma omp parallel for reduction(+:basarili_sayisi)
    for (int i = 0; i < OGRENCI_SAYISI; ++i) {
        if (gpa_listesi[i] >= 2.0) {
            ++basarili_sayisi;
        }
    }

    double basari_orani = 100.0 * basarili_sayisi / OGRENCI_SAYISI;
    std::cout << "  Basarili ogrenci: " << basarili_sayisi
              << " / " << OGRENCI_SAYISI << '\n';
    std::cout << "  Basari orani: %" << basari_orani << '\n';

    // Izlek sayisi bilgisi
    std::cout << "\n--- Izlek bilgisi ---" << '\n';
    #pragma omp parallel
    {
        #pragma omp single
        {
            std::cout << "  Kullanilan izlek sayisi: "
                      << omp_get_num_threads() << '\n';
        }
    }

    return 0;
}

/*
 * BEKLENEN CIKTI (yaklasik):
 * --------------------------
 * === 1. Toplam Reduction (+) ===
 *   Sirali  GPA ortalamasi: 2.003 (0.8 ms)
 *   Paralel GPA ortalamasi: 2.003 (0.3 ms)
 *   [OK] Sonuclar tutarli!
 *
 * === 2. Maksimum Reduction (max) ===
 *   En yuksek GPA: 4.0
 *
 * === 3. Minimum Reduction (min) ===
 *   En dusuk GPA: 0.0
 *
 * === 4. Sayma Reduction ===
 *   Basarili ogrenci: ~250000 / 500000
 *   Basari orani: %50.0
 *
 * --- Izlek bilgisi ---
 *   Kullanilan izlek sayisi: 8
 */

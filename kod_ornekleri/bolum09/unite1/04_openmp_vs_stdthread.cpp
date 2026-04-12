/**
 * OpenMP ile std::thread Karsilastirmasi
 *
 * Ayni hesaplamayi (ogrenci not ortalamasi) hem OpenMP hem std::thread
 * ile yapar ve sonuclari/sureleri karsilastirir. Her iki yaklasimin
 * ayni sonucu vermesi beklenir.
 *
 * Ogrenilecek kavramlar:
 * - OpenMP parallel for ile dongu paralellistirme
 * - std::thread ile manuel is bolumu
 * - Zamanlama karsilastirmasi
 * - Her iki yaklasimin avantaj/dezavantajlari
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 1 - Eszamanlilik Kavramlari ve OpenMP
 *
 * Derleme: g++ -std=c++20 -fopenmp -pthread -o 04_karsilastir 04_openmp_vs_stdthread.cpp
 * Calistirma: ./04_karsilastir
 */

#include <iostream>
#include <vector>
#include <thread>
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
    constexpr int OGRENCI_SAYISI = 1'000'000;
    std::vector<double> notlar(OGRENCI_SAYISI);

    // Not verilerini olustur (0-100 arasi)
    std::srand(42);
    for (int i = 0; i < OGRENCI_SAYISI; ++i) {
        notlar[i] = (std::rand() % 101);
    }

    // ================================================================
    // 1. SIRALI HESAPLAMA
    // ================================================================
    std::cout << "=== 1. Sirali Hesaplama ===" << '\n';
    double sira_toplam = 0.0;
    double sira_sure = olc([&]() {
        for (int i = 0; i < OGRENCI_SAYISI; ++i) {
            sira_toplam += notlar[i];
        }
    });
    std::cout << "  Ortalama: " << sira_toplam / OGRENCI_SAYISI
              << "  Sure: " << sira_sure << " ms" << '\n';

    // ================================================================
    // 2. OPENMP ILE PARALEL HESAPLAMA
    // ================================================================
    std::cout << "\n=== 2. OpenMP Paralel Hesaplama ===" << '\n';
    double omp_toplam = 0.0;
    int omp_izlek_sayisi = 0;

    double omp_sure = olc([&]() {
        #pragma omp parallel for reduction(+:omp_toplam)
        for (int i = 0; i < OGRENCI_SAYISI; ++i) {
            omp_toplam += notlar[i];
        }
        omp_izlek_sayisi = omp_get_max_threads();
    });
    std::cout << "  Ortalama: " << omp_toplam / OGRENCI_SAYISI
              << "  Sure: " << omp_sure << " ms"
              << "  (" << omp_izlek_sayisi << " izlek)" << '\n';

    // ================================================================
    // 3. STD::THREAD ILE PARALEL HESAPLAMA
    // ================================================================
    std::cout << "\n=== 3. std::thread Paralel Hesaplama ===" << '\n';

    unsigned int izlek_sayisi = std::thread::hardware_concurrency();
    if (izlek_sayisi == 0) izlek_sayisi = 4;

    // Her izlegin kendi yerel toplami
    std::vector<double> yerel_toplamlar(izlek_sayisi, 0.0);

    double thread_sure = olc([&]() {
        std::vector<std::thread> izlekler;
        izlekler.reserve(izlek_sayisi);

        int parca_boyutu = OGRENCI_SAYISI / izlek_sayisi;

        for (unsigned int t = 0; t < izlek_sayisi; ++t) {
            int baslangic = t * parca_boyutu;
            int bitis = (t == izlek_sayisi - 1) ? OGRENCI_SAYISI
                                                 : (t + 1) * parca_boyutu;

            izlekler.emplace_back([&notlar, &yerel_toplamlar, t,
                                   baslangic, bitis]() {
                double yerel = 0.0;
                for (int i = baslangic; i < bitis; ++i) {
                    yerel += notlar[i];
                }
                yerel_toplamlar[t] = yerel;
            });
        }

        // Tum izlekleri bekle
        for (auto& izlek : izlekler) {
            izlek.join();
        }
    });

    double thread_toplam = 0.0;
    for (double t : yerel_toplamlar) {
        thread_toplam += t;
    }

    std::cout << "  Ortalama: " << thread_toplam / OGRENCI_SAYISI
              << "  Sure: " << thread_sure << " ms"
              << "  (" << izlek_sayisi << " izlek)" << '\n';

    // ================================================================
    // 4. SONUC KARSILASTIRMASI
    // ================================================================
    std::cout << "\n=== 4. Sonuc Karsilastirmasi ===" << '\n';
    std::cout << "  Sirali toplam      : " << sira_toplam << '\n';
    std::cout << "  OpenMP toplam      : " << omp_toplam << '\n';
    std::cout << "  std::thread toplam : " << thread_toplam << '\n';

    bool tutarli = (std::abs(sira_toplam - omp_toplam) < 1e-3) &&
                   (std::abs(sira_toplam - thread_toplam) < 1e-3);

    if (tutarli) {
        std::cout << "  [OK] Her uc yontem ayni sonucu verdi!" << '\n';
    } else {
        std::cout << "  [!] Kucuk kayan nokta farklari mevcut." << '\n';
    }

    std::cout << "\n--- Hiz Karsilastirmasi ---" << '\n';
    std::cout << "  Sirali     : " << sira_sure << " ms" << '\n';
    std::cout << "  OpenMP     : " << omp_sure << " ms"
              << " (hizlanma: " << sira_sure / omp_sure << "x)" << '\n';
    std::cout << "  std::thread: " << thread_sure << " ms"
              << " (hizlanma: " << sira_sure / thread_sure << "x)" << '\n';

    return 0;
}

/*
 * BEKLENEN CIKTI (yaklasik):
 * --------------------------
 * === 1. Sirali Hesaplama ===
 *   Ortalama: 50.0  Sure: 1.5 ms
 *
 * === 2. OpenMP Paralel Hesaplama ===
 *   Ortalama: 50.0  Sure: 0.4 ms  (8 izlek)
 *
 * === 3. std::thread Paralel Hesaplama ===
 *   Ortalama: 50.0  Sure: 0.5 ms  (8 izlek)
 *
 * === 4. Sonuc Karsilastirmasi ===
 *   Sirali toplam      : 50000000
 *   OpenMP toplam      : 50000000
 *   std::thread toplam : 50000000
 *   [OK] Her uc yontem ayni sonucu verdi!
 *
 * --- Hiz Karsilastirmasi ---
 *   Sirali     : 1.5 ms
 *   OpenMP     : 0.4 ms (hizlanma: 3.75x)
 *   std::thread: 0.5 ms (hizlanma: 3.0x)
 */

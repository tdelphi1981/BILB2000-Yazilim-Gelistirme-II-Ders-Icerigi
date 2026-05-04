/**
 * Konsol İlerleme Çubuğu - Çok Worker'lı
 *
 * Bölüm 11 Ünite 3 Konu 3 (Qt'siz versiyon)
 *
 * Bu örnekte:
 *  - 4 worker paralel çalışır, ortak atomic sayaca yazar
 *  - Ana thread sayacı izler, ASCII progress bar çizer
 *  - jthread + stop_token ile Ctrl+C destekli (basit gösterim)
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 03_pbar 03_progress_bar_console.cpp
 */

#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <stop_token>
#include <string>
#include <thread>
#include <vector>

void cizProgressBar(std::size_t islenen, std::size_t toplam, int genislik = 40) {
    double yuzde = (toplam == 0) ? 0.0 : 100.0 * islenen / toplam;
    int dolu = static_cast<int>(genislik * yuzde / 100.0);

    std::cout << "\r[";
    for (int i = 0; i < genislik; ++i) {
        std::cout << (i < dolu ? '#' : '.');
    }
    std::cout << "] " << std::setw(5) << std::fixed << std::setprecision(1)
              << yuzde << "% (" << islenen << "/" << toplam << ")"
              << std::flush;
}

int main() {
    constexpr std::size_t TOPLAM = 4000;
    constexpr int N_ISCI = 4;

    std::atomic<std::size_t> sayac{0};

    auto baslangic = std::chrono::steady_clock::now();

    // İşçiler: her biri TOPLAM/N_ISCI iş yapar
    std::vector<std::jthread> isciler;
    for (int w = 0; w < N_ISCI; ++w) {
        isciler.emplace_back([&, w](std::stop_token st) {
            std::mt19937 rng(w);
            std::uniform_int_distribution<int> uyku(1, 5);
            for (std::size_t i = 0; i < TOPLAM / N_ISCI; ++i) {
                if (st.stop_requested()) return;
                std::this_thread::sleep_for(std::chrono::milliseconds(uyku(rng)));
                sayac.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // İlerleme izleyici (ana thread)
    while (sayac.load(std::memory_order_relaxed) < TOPLAM) {
        cizProgressBar(sayac.load(std::memory_order_relaxed), TOPLAM);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    cizProgressBar(TOPLAM, TOPLAM);
    std::cout << '\n';

    for (auto& w : isciler) w.join();

    auto sure = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - baslangic);
    std::cout << "Toplam sure: " << sure.count() << " ms\n";
    std::cout << "Verim: " << (TOPLAM * 1000LL / sure.count()) << " is/sn\n";

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
[####################################....] 90.0% (3600/4000)
[########################################] 100.0% (4000/4000)
Toplam sure: 1500-3500 ms
Verim: ~1000-2500 is/sn
*/

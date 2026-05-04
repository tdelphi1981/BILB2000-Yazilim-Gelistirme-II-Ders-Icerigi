/**
 * Lab 11 - Çalışma 4: ASCII Progress Bar (Atomic Sayaç)
 *
 * Bu dosyadaki GOREV yorumlarını uygun kodla değiştirin.
 *
 * Derleme: g++ -std=c++20 -pthread -O2 -o calisma4 calisma4.cpp
 * Çalıştırma: ./calisma4
 */

#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <stop_token>
#include <thread>
#include <vector>

void cizProgressBar(std::size_t islenen, std::size_t toplam, int gen = 40) {
    // GOREV 1: ASCII progress bar
    //   - yuzde = 100.0 * islenen / toplam
    //   - dolu = (int)(gen * yuzde / 100.0)
    //   - "\r[" + dolu kadar '#' + (gen-dolu) kadar '.' + "] X.X% (i/n)"
    //   - std::flush

}

int main() {
    constexpr std::size_t TOPLAM = 4000;
    constexpr int N_ISCI = 4;

    std::atomic<std::size_t> sayac{0};
    auto baslangic = std::chrono::steady_clock::now();

    // GOREV 2: 4 jthread işçi
    //   - Her biri TOPLAM / N_ISCI iş yapar
    //   - 1-5 ms uyku (rng ile)
    //   - sayac.fetch_add(1, std::memory_order_relaxed)
    //   - stop_token kontrolu (opsiyonel)


    // GOREV 3: Ana thread: sayaç < TOPLAM olduğu sürece progress bar
    //   - cizProgressBar(sayac.load(), TOPLAM)
    //   - 100 ms uyu
    //   - sayac >= TOPLAM olunca dongu bitsin


    // Son kez %100 yazdir
    cizProgressBar(TOPLAM, TOPLAM);
    std::cout << '\n';

    auto sure = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - baslangic);
    std::cout << "Toplam sure: " << sure.count() << " ms\n";

    return 0;
}

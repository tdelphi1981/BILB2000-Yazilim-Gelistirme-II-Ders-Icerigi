/**
 * Atomic Sayaçla İlerleme Bildirimi
 *
 * Bölüm 11 Ünite 3 Konu 1
 *
 * Bu örnekte:
 *  - std::atomic<size_t> ile sayaç paylaşımı
 *  - memory_order_relaxed yeterli (sadece sayım)
 *  - Polling yöntemiyle UI tarafı sayacı izler
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 01_atomic 01_atomic_ilerleme.cpp
 */

#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

struct IlerlemeDurumu {
    std::atomic<std::size_t> islenen{0};
    std::size_t toplam = 0;
    std::atomic<bool> bitti{false};
};

void agirIs(IlerlemeDurumu& d, std::size_t n) {
    d.toplam = n;
    for (std::size_t i = 0; i < n; ++i) {
        // Gerçekçi gecikme
        std::this_thread::sleep_for(std::chrono::microseconds(50));
        d.islenen.fetch_add(1, std::memory_order_relaxed);
    }
    d.bitti.store(true, std::memory_order_release);
}

void ilerlemeyiGoster(const IlerlemeDurumu& d) {
    while (!d.bitti.load(std::memory_order_acquire)) {
        auto x = d.islenen.load(std::memory_order_relaxed);
        double y = (d.toplam == 0) ? 0.0 : 100.0 * x / d.toplam;
        std::cout << "\rIlerleme: " << x << "/" << d.toplam
                  << " (" << std::fixed << std::setprecision(1) << y << "%)"
                  << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    auto x = d.islenen.load(std::memory_order_relaxed);
    std::cout << "\rIlerleme: " << x << "/" << d.toplam << " (100.0%)\n";
}

int main() {
    IlerlemeDurumu d;
    constexpr std::size_t N = 5000;

    std::thread isci(agirIs, std::ref(d), N);
    std::thread izleyici(ilerlemeyiGoster, std::ref(d));

    isci.join();
    izleyici.join();

    std::cout << "Tamamlandi.\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
Ilerleme: 1024/5000 (20.5%)
Ilerleme: 2048/5000 (41.0%)
...
Ilerleme: 5000/5000 (100.0%)
Tamamlandi.
*/

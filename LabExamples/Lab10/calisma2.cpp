/**
 * Lab 10 - Çalışma 2: std::async ile Paralel Toplam
 *
 * Bu dosyayı önce seri çalıştırın, sonra TODO yorumunu tamamlayın.
 *
 * Derleme: g++ -std=c++20 -pthread -O2 -o calisma2 calisma2.cpp
 * Çalıştırma: ./calisma2
 */

#include <iostream>
#include <numeric>
#include <vector>
#include <chrono>
#include <future>

long long kismiTopla(const std::vector<int>& v,
                     std::size_t baslangic,
                     std::size_t bitis) {
    long long toplam = 0;
    for (std::size_t i = baslangic; i < bitis; ++i) {
        toplam += v[i];
    }
    return toplam;
}

int main() {
    constexpr std::size_t N = 40'000'000;
    std::vector<int> veri(N);
    std::iota(veri.begin(), veri.end(), 1);

    // Seri versiyon
    {
        auto t0 = std::chrono::steady_clock::now();
        long long toplam = kismiTopla(veri, 0, N);
        auto t1 = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            t1 - t0).count();
        std::cout << "Seri: " << toplam << " (" << ms << " ms)\n";
    }

    // TODO (Gorev 2.2): 4 iscili paralel versiyon
    //   1. std::vector<std::future<long long>> futurelar;
    //   2. std::size_t parca = N / 4;
    //   3. for i in 0..4:
    //        bas = i*parca, son = (i+1)*parca (son parca icin N)
    //        futurelar.push_back(std::async(std::launch::async,
    //            kismiTopla, std::cref(veri), bas, son));
    //   4. long long toplam = 0;
    //      for (auto& f : futurelar) toplam += f.get();
    //   5. Zamani olc ve yazdir

    return 0;
}

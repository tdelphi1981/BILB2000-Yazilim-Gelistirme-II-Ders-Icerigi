/**
 * std::async Temel Kullanımı
 *
 * Bu program, bir hesaplamayı asenkron olarak başlatıp sonucunu
 * future.get() ile alma örüntüsünü gösterir.
 *
 * Öğrenilecek kavramlar:
 * - std::async(launch::async, ...) ile asenkron başlatma
 * - std::future<T>::get() ile sonuç alma
 * - auto ile tür çıkarımı
 * - std::cref ile büyük veriyi referans olarak aktarma
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 2 - std::async ve Future/Promise
 *
 * Derleme: g++ -std=c++20 -pthread -o 01_async 01_async_temel.cpp
 * Çalıştırma: ./01_async
 */

#include <future>
#include <iostream>
#include <numeric>
#include <vector>
#include <chrono>

long long topla(const std::vector<int>& v) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return std::accumulate(v.begin(), v.end(), 0LL);
}

int main() {
    std::vector<int> sayilar(1'000'000);
    std::iota(sayilar.begin(), sayilar.end(), 1);

    std::cout << "async başlatıldı\n";
    auto gelecek = std::async(std::launch::async,
                               topla, std::cref(sayilar));

    // Bu arada ana izlek başka iş yapabilir
    std::cout << "Ana izlek diğer işleri yapıyor...\n";
    for (int i = 0; i < 3; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << "  tik " << i << "\n";
    }

    // Sonucu al (hazır değilse bekle)
    long long toplam = gelecek.get();
    std::cout << "Toplam: " << toplam << "\n";
    // Beklenen: N*(N+1)/2 = 500000500000
    return 0;
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * async başlatıldı
 * Ana izlek diğer işleri yapıyor...
 *   tik 0
 *   tik 1
 *   tik 2
 * Toplam: 500000500000
 */

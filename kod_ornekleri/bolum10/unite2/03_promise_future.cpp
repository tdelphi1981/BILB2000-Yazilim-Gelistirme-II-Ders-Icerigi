/**
 * std::promise ve std::future ile Manuel Değer Paylaşımı
 *
 * Bu program, kendi izleğinizi yaratıp sonucunu promise/future
 * kanalı üzerinden başka izleğe iletmeyi gösterir.
 *
 * Öğrenilecek kavramlar:
 * - std::promise<T> ile yazma ucu
 * - std::future<T> ile okuma ucu
 * - std::move ile promise aktarımı
 * - shared_future ile çoklu tüketici
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 2 - std::async ve Future/Promise
 *
 * Derleme: g++ -std=c++20 -pthread -o 03_promise 03_promise_future.cpp
 * Çalıştırma: ./03_promise
 */

#include <future>
#include <thread>
#include <iostream>
#include <chrono>
#include <vector>

void hesapla(std::promise<int> sozum, int girdi) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int sonuc = girdi * girdi;
    sozum.set_value(sonuc);  // Future artık hazır
}

int main() {
    // Basit kullanım
    std::cout << "=== Basit promise/future ===\n";
    std::promise<int> soz;
    std::future<int> gelecek = soz.get_future();

    std::thread t1(hesapla, std::move(soz), 7);
    std::cout << "Ana izlek bekliyor...\n";
    std::cout << "Sonuç: " << gelecek.get() << "\n";  // 49
    t1.join();

    // shared_future ile 3 tüketici
    std::cout << "\n=== shared_future ile 3 tüketici ===\n";
    std::promise<int> soz2;
    std::shared_future<int> paylasimli = soz2.get_future().share();

    auto tuketici = [paylasimli](int id) {
        int x = paylasimli.get();
        std::cout << "  Tüketici " << id << " gördü: " << x << "\n";
    };

    std::vector<std::thread> tuketiciler;
    for (int id = 0; id < 3; ++id) {
        tuketiciler.emplace_back(tuketici, id);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Ana izlek değeri sunuyor...\n";
    soz2.set_value(777);

    for (auto& t : tuketiciler) t.join();
    return 0;
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * === Basit promise/future ===
 * Ana izlek bekliyor...
 * Sonuç: 49
 *
 * === shared_future ile 3 tüketici ===
 * Ana izlek değeri sunuyor...
 *   Tüketici 0 gördü: 777
 *   Tüketici 1 gördü: 777
 *   Tüketici 2 gördü: 777
 */

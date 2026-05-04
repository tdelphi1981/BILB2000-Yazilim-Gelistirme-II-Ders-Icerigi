/**
 * Future Üzerinden İstisna Aktarımı
 *
 * Bu program, async içinde fırlatılan istisnaların future.get()
 * çağrısında nasıl yeniden fırlatıldığını gösterir. Ayrıca
 * std::promise::set_exception ile manuel istisna aktarımını
 * örnekler.
 *
 * Öğrenilecek kavramlar:
 * - async içinde istisna -> future'a otomatik aktarım
 * - promise::set_exception ile manuel aktarım
 * - std::current_exception yakalama
 * - broken_promise durumu
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 2 - std::async ve Future/Promise
 *
 * Derleme: g++ -std=c++20 -pthread -o 04_async_exc 04_async_exception.cpp
 * Çalıştırma: ./04_async_exc
 */

#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>

int riskliIs(int x) {
    if (x < 0) {
        throw std::runtime_error("Negatif değer!");
    }
    if (x == 0) {
        throw std::invalid_argument("Sıfır kabul edilmez");
    }
    return x * x;
}

void promiseIleCalis(std::promise<int> soz, int girdi) {
    try {
        int sonuc = riskliIs(girdi);
        soz.set_value(sonuc);
    } catch (...) {
        soz.set_exception(std::current_exception());
    }
}

int main() {
    // 1. async içindeki istisna otomatik aktarılır
    std::cout << "=== async ile istisna ===\n";
    for (int deger : {5, -3, 0}) {
        auto f = std::async(std::launch::async, riskliIs, deger);
        try {
            int sonuc = f.get();
            std::cout << "  " << deger << "^2 = " << sonuc << "\n";
        } catch (const std::exception& e) {
            std::cout << "  " << deger << " hata: "
                      << e.what() << "\n";
        }
    }

    // 2. promise ile manuel aktarım
    std::cout << "\n=== promise::set_exception ===\n";
    for (int deger : {10, -5}) {
        std::promise<int> soz;
        auto f = soz.get_future();
        std::thread t(promiseIleCalis, std::move(soz), deger);
        try {
            std::cout << "  " << deger << "^2 = " << f.get() << "\n";
        } catch (const std::exception& e) {
            std::cout << "  " << deger << " hata: "
                      << e.what() << "\n";
        }
        t.join();
    }

    // 3. broken_promise -- promise set edilmeden yıkılırsa
    std::cout << "\n=== broken_promise ===\n";
    std::future<int> yarim;
    {
        std::promise<int> gecici;
        yarim = gecici.get_future();
        // gecici burada set_value çağrılmadan yıkılır
    }
    try {
        int x = yarim.get();
        std::cout << "  beklenmedik: " << x << "\n";
    } catch (const std::future_error& e) {
        std::cout << "  future_error: " << e.what() << "\n";
    }
    return 0;
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * === async ile istisna ===
 *   5^2 = 25
 *   -3 hata: Negatif değer!
 *   0 hata: Sıfır kabul edilmez
 *
 * === promise::set_exception ===
 *   10^2 = 100
 *   -5 hata: Negatif değer!
 *
 * === broken_promise ===
 *   future_error: std::future_error: Broken promise
 */

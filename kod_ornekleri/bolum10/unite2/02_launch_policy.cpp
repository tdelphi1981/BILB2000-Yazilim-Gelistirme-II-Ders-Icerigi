/**
 * Launch Policy Karşılaştırması
 *
 * Bu program, std::launch::async ve std::launch::deferred
 * politikaları arasındaki farkı wait_for ile test ederek gösterir.
 *
 * Öğrenilecek kavramlar:
 * - std::launch::async: yeni izlekte hemen çalışır
 * - std::launch::deferred: get() çağrılana kadar çalışmaz
 * - future_status::ready, timeout, deferred
 * - Varsayılan politikanın tehlikesi
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 2 - std::async ve Future/Promise
 *
 * Derleme: g++ -std=c++20 -pthread -o 02_launch 02_launch_policy.cpp
 * Çalıştırma: ./02_launch
 */

#include <future>
#include <iostream>
#include <chrono>
#include <thread>

int gorev(const std::string& etiket) {
    std::cout << "[" << etiket << "] çalışıyor\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 42;
}

int main() {
    // 1. launch::async -- hemen yeni izlekte
    std::cout << "=== launch::async ===\n";
    auto f1 = std::async(std::launch::async, gorev, "async");
    auto durum1 = f1.wait_for(std::chrono::milliseconds(100));
    if (durum1 == std::future_status::timeout) {
        std::cout << "  Durum: hâlâ çalışıyor (beklenen)\n";
    }
    std::cout << "  Sonuç: " << f1.get() << "\n\n";

    // 2. launch::deferred -- get çağrılana kadar pasif
    std::cout << "=== launch::deferred ===\n";
    auto f2 = std::async(std::launch::deferred, gorev, "deferred");
    auto durum2 = f2.wait_for(std::chrono::milliseconds(100));
    if (durum2 == std::future_status::deferred) {
        std::cout << "  Durum: deferred (henüz çalışmadı)\n";
    }
    std::cout << "  Şimdi get çağıracağım...\n";
    std::cout << "  Sonuç: " << f2.get() << "\n\n";

    // 3. Polling örneği
    std::cout << "=== Polling örneği ===\n";
    auto f3 = std::async(std::launch::async, [] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return 100;
    });

    int adim = 0;
    while (f3.wait_for(std::chrono::milliseconds(400))
           != std::future_status::ready)
    {
        std::cout << "  adım " << ++adim << ": UI güncelleniyor\n";
    }
    std::cout << "  Sonuç hazır: " << f3.get() << "\n";
    return 0;
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * === launch::async ===
 * [async] çalışıyor
 *   Durum: hâlâ çalışıyor (beklenen)
 *   Sonuç: 42
 *
 * === launch::deferred ===
 *   Durum: deferred (henüz çalışmadı)
 *   Şimdi get çağıracağım...
 * [deferred] çalışıyor
 *   Sonuç: 42
 *
 * === Polling örneği ===
 *   adım 1: UI güncelleniyor
 *   adım 2: UI güncelleniyor
 *   adım 3: UI güncelleniyor
 *   adım 4: UI güncelleniyor
 *   Sonuç hazır: 100
 */

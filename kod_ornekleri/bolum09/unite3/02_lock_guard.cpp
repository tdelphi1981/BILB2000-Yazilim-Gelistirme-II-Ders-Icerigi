/**
 * lock_guard ile Guvenli Kilitleme
 *
 * Bu program, data race sorununu std::lock_guard ile cozer.
 * RAII tabanlı kilit yonetimi sayesinde istisna guvenliği saglanir.
 *
 * Ogrenilecek kavramlar:
 * - std::mutex temel kullanimi
 * - std::lock_guard ile RAII kilitleme
 * - Kritik bolge koruma
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 3 - std::mutex ve Senkronizasyon
 *
 * Derleme: g++ -std=c++20 -pthread -o 02_lock_guard 02_lock_guard.cpp
 * Calistirma: ./02_lock_guard
 */

#include <iostream>
#include <thread>
#include <mutex>

int sayac = 0;
std::mutex mtx;

void artir(int miktar) {
    for (int i = 0; i < miktar; ++i) {
        // RAII kilit: olusturuldiginda kilitler, yikildiginda acar
        std::lock_guard<std::mutex> kilit(mtx);
        ++sayac;
    }
}

int main() {
    constexpr int TEKRAR = 1'000'000;

    std::thread t1(artir, TEKRAR);
    std::thread t2(artir, TEKRAR);

    t1.join();
    t2.join();

    std::cout << "Beklenen: " << (2 * TEKRAR) << "\n";
    std::cout << "Gercek:   " << sayac << "\n";

    if (sayac == 2 * TEKRAR) {
        std::cout << "[OK] Sonuc dogru! Mutex korumasi calisiyor.\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * Beklenen: 2000000
 * Gercek:   2000000
 * [OK] Sonuc dogru! Mutex korumasi calisiyor.
 */

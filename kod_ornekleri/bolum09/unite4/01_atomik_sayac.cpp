/**
 * Atomik Sayac - std::atomic ile Thread-Safe Sayac
 *
 * Bu program, std::atomic<int> kullanarak birden fazla izlegin
 * ayni sayaci guvenli sekilde artirabilecegini gosterir.
 * Mutex kullanmadan veri yarisi onlenir.
 *
 * Ogrenilecek kavramlar:
 * - std::atomic<int> tanimi ve kullanimi
 * - fetch_add() atomik artirma
 * - load() ve store() islemleri
 * - Atomik vs mutex karsilastirmasi
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 4 - Atomik Islemler ve OBYS Entegrasyonu
 *
 * Derleme: g++ -std=c++20 -pthread -o 01_atomik_sayac 01_atomik_sayac.cpp
 * Calistirma: ./01_atomik_sayac
 */

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// Atomik sayac -- kilitsiz, thread-safe
std::atomic<int> atomikSayac{0};

// Normal sayac -- veri yarisi riski var (karsilastirma icin)
int normalSayac = 0;

void atomikArtir(int miktar) {
    for (int i = 0; i < miktar; ++i) {
        atomikSayac.fetch_add(1, std::memory_order_relaxed);
    }
}

void normalArtir(int miktar) {
    for (int i = 0; i < miktar; ++i) {
        ++normalSayac;  // [UYARI] Veri yarisi!
    }
}

int main() {
    constexpr int IZLEK_SAYISI = 8;
    constexpr int ARTIRMA = 100'000;

    std::cout << "=== Atomik Sayac Testi ===\n\n";
    std::cout << "Izlek sayisi: " << IZLEK_SAYISI << '\n';
    std::cout << "Her izlek " << ARTIRMA << " kez artiracak\n";
    std::cout << "Beklenen sonuc: " << IZLEK_SAYISI * ARTIRMA << "\n\n";

    // --- Atomik sayac testi ---
    {
        auto baslangic = std::chrono::steady_clock::now();
        std::vector<std::thread> izlekler;
        for (int i = 0; i < IZLEK_SAYISI; ++i)
            izlekler.emplace_back(atomikArtir, ARTIRMA);
        for (auto& t : izlekler)
            t.join();
        auto bitis = std::chrono::steady_clock::now();

        auto sure = std::chrono::duration_cast<std::chrono::microseconds>(
            bitis - baslangic).count();

        std::cout << "[ATOMIK] Sonuc: " << atomikSayac.load() << '\n';
        std::cout << "[ATOMIK] Sure:  " << sure << " us\n\n";
    }

    // --- Normal sayac testi (veri yarisi gosterimi) ---
    {
        auto baslangic = std::chrono::steady_clock::now();
        std::vector<std::thread> izlekler;
        for (int i = 0; i < IZLEK_SAYISI; ++i)
            izlekler.emplace_back(normalArtir, ARTIRMA);
        for (auto& t : izlekler)
            t.join();
        auto bitis = std::chrono::steady_clock::now();

        auto sure = std::chrono::duration_cast<std::chrono::microseconds>(
            bitis - baslangic).count();

        std::cout << "[NORMAL] Sonuc: " << normalSayac << '\n';
        std::cout << "[NORMAL] Sure:  " << sure << " us\n";

        if (normalSayac != IZLEK_SAYISI * ARTIRMA) {
            std::cout << "[NORMAL] Veri yarisi tespit edildi! "
                      << "Fark: "
                      << (IZLEK_SAYISI * ARTIRMA - normalSayac)
                      << " kayip\n";
        }
    }

    return 0;
}

/*
 * BEKLENEN CIKTI (ornek):
 * -----------------------
 * === Atomik Sayac Testi ===
 *
 * Izlek sayisi: 8
 * Her izlek 100000 kez artiracak
 * Beklenen sonuc: 800000
 *
 * [ATOMIK] Sonuc: 800000
 * [ATOMIK] Sure:  12345 us
 *
 * [NORMAL] Sonuc: 654321
 * [NORMAL] Sure:  9876 us
 * [NORMAL] Veri yarisi tespit edildi! Fark: 145679 kayip
 */

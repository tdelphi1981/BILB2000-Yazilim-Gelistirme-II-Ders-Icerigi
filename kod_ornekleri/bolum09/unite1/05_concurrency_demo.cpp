/**
 * hardware_concurrency ve Eszamanlilik Demo
 *
 * Bu program, hardware_concurrency() ile mevcut cekirdek sayisini tespit
 * eder ve N adet izlek olusturarak cikti siralanmasinin belirsiz oldugunu
 * gosterir (non-deterministic interleaving).
 *
 * Ogrenilecek kavramlar:
 * - std::thread::hardware_concurrency()
 * - Birden fazla izlek olusturma ve yonetme
 * - Cikti serpikmesi (interleaving)
 * - join() ile izlek bekleme
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 1 - Eszamanlilik Kavramlari ve OpenMP
 *
 * Derleme: g++ -std=c++20 -pthread -o 05_concurrency 05_concurrency_demo.cpp
 * Calistirma: ./05_concurrency
 */

#include <iostream>
#include <thread>
#include <vector>
#include <sstream>

// Her izlegin calistiracagi fonksiyon
void izlek_gorevi(int izlek_id, int toplam_izlek) {
    // [NOT] Mutex olmadan cikti serpikir - bu beklenen davranistir
    for (int i = 0; i < 3; ++i) {
        std::ostringstream oss;
        oss << "  [Izlek " << izlek_id << "/" << toplam_izlek
            << "] Adim " << (i + 1) << "/3" << '\n';
        std::cout << oss.str();
    }
}

int main() {
    // 1. Donanim bilgisi
    unsigned int cekirdek_sayisi = std::thread::hardware_concurrency();
    std::cout << "=== Donanim Bilgisi ===" << '\n';
    std::cout << "  hardware_concurrency(): " << cekirdek_sayisi << '\n';

    if (cekirdek_sayisi == 0) {
        std::cout << "  [!] Cekirdek sayisi alinamiyor, 4 varsayilacak." << '\n';
        cekirdek_sayisi = 4;
    }

    // 2. Izlek sayisi: cekirdek sayisi kadar
    unsigned int izlek_sayisi = cekirdek_sayisi;
    // Cok fazla cikti olmasin diye ust sinir koy
    if (izlek_sayisi > 8) izlek_sayisi = 8;

    std::cout << "\n=== " << izlek_sayisi << " Izlek Olusturuluyor ===" << '\n';
    std::cout << "  [NOT] Cikti sirasi her calistirmada farkli olacak!" << '\n';
    std::cout << '\n';

    // 3. Izlekleri olustur
    std::vector<std::thread> izlekler;
    izlekler.reserve(izlek_sayisi);

    for (unsigned int i = 0; i < izlek_sayisi; ++i) {
        izlekler.emplace_back(izlek_gorevi, i, izlek_sayisi);
    }

    // 4. Tum izleklerin bitmesini bekle
    for (auto& t : izlekler) {
        t.join();
    }

    std::cout << "\n=== Tum izlekler tamamlandi ===" << '\n';

    // 5. Farkli calistirmalarda farkli siralama gosterimi
    std::cout << "\n--- Ikinci calistirma (farkli siralama bekleniyor) ---" << '\n';

    std::vector<std::thread> izlekler2;
    izlekler2.reserve(izlek_sayisi);

    for (unsigned int i = 0; i < izlek_sayisi; ++i) {
        izlekler2.emplace_back([i, izlek_sayisi]() {
            std::ostringstream oss;
            oss << "  Izlek " << i << " calisiyor (ID: "
                << std::this_thread::get_id() << ")" << '\n';
            std::cout << oss.str();
        });
    }

    for (auto& t : izlekler2) {
        t.join();
    }

    std::cout << "\n[OK] Program basariyla tamamlandi." << '\n';

    return 0;
}

/*
 * BEKLENEN CIKTI (sirasi degisir!):
 * ----------------------------------
 * === Donanim Bilgisi ===
 *   hardware_concurrency(): 8
 *
 * === 8 Izlek Olusturuluyor ===
 *   [NOT] Cikti sirasi her calistirmada farkli olacak!
 *
 *   [Izlek 0/8] Adim 1/3
 *   [Izlek 3/8] Adim 1/3
 *   [Izlek 1/8] Adim 1/3
 *   [Izlek 0/8] Adim 2/3
 *   [Izlek 2/8] Adim 1/3
 *   ... (sira her seferinde degisir)
 *
 * === Tum izlekler tamamlandi ===
 *
 * --- Ikinci calistirma (farkli siralama bekleniyor) ---
 *   Izlek 2 calisiyor (ID: 0x70000a...)
 *   Izlek 0 calisiyor (ID: 0x70000b...)
 *   ...
 *
 * [OK] Program basariyla tamamlandi.
 */

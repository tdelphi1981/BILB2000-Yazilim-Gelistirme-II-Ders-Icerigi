/**
 * Data Race Gosterimi - Ortak Sayac
 *
 * Bu program, iki izlegin ayni sayaci korumasiz olarak artirdiginda
 * data race olustuğunu ve sonuclarin tutarsiz oldugunu gosterir.
 *
 * Ogrenilecek kavramlar:
 * - Data race (veri yarisi) kavrami
 * - Paylasimli veriye eszamanli erisim sorunu
 * - Tanimsiz davranis (undefined behavior)
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 3 - std::mutex ve Senkronizasyon
 *
 * Derleme: g++ -std=c++20 -pthread -o 01_data_race 01_data_race.cpp
 * Calistirma: ./01_data_race
 */

#include <iostream>
#include <thread>

int sayac = 0;  // Paylasimli veri - KORUMASIZ!

void artir(int miktar) {
    for (int i = 0; i < miktar; ++i) {
        ++sayac;  // Data race! Atomik degil
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

    if (sayac != 2 * TEKRAR) {
        std::cout << "[!] Data race tespit edildi! "
                  << "Sonuc beklenen degerden farkli.\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI (ornek - her calistirmada farkli):
 * --------------------------------------------------
 * Beklenen: 2000000
 * Gercek:   1543287
 * [!] Data race tespit edildi! Sonuc beklenen degerden farkli.
 */

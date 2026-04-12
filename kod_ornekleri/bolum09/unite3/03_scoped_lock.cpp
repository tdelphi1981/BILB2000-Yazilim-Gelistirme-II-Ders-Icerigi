/**
 * scoped_lock ile Deadlock Onleme (C++17)
 *
 * Bu program, iki mutex'in ters sirada kilitlendigi deadlock
 * senaryosunu std::scoped_lock ile guvenle cozer.
 *
 * Ogrenilecek kavramlar:
 * - Deadlock (kilitlenme) kavrami
 * - std::scoped_lock (C++17) kullanimi
 * - Birden fazla mutex'i guvenle kilitleme
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 3 - std::mutex ve Senkronizasyon
 *
 * Derleme: g++ -std=c++20 -pthread -o 03_scoped_lock 03_scoped_lock.cpp
 * Calistirma: ./03_scoped_lock
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

// OBYS benzeri iki ayri depo
struct OgrenciDepo {
    std::mutex mtx;
    std::vector<std::string> ogrenciler;
};

struct DersDepo {
    std::mutex mtx;
    std::vector<std::string> kayitlar;
};

OgrenciDepo ogr_depo;
DersDepo ders_depo;

// scoped_lock ile deadlock-free kayit islemi
void ogrenci_kayit(const std::string& isim,
                   const std::string& ders,
                   int tekrar) {
    for (int i = 0; i < tekrar; ++i) {
        // Iki mutex'i ayni anda guvenle kilitle
        std::scoped_lock kilit(ogr_depo.mtx, ders_depo.mtx);

        std::string tam_isim = isim + "_" + std::to_string(i);
        ogr_depo.ogrenciler.push_back(tam_isim);
        ders_depo.kayitlar.push_back(tam_isim + " -> " + ders);
    }
}

int main() {
    constexpr int KAYIT_SAYISI = 1000;

    // Iki izlek farkli siralarda depolara erisecek
    // scoped_lock sayesinde deadlock olusmaz
    std::thread t1(ogrenci_kayit, "Ali", "Yazilim_II", KAYIT_SAYISI);
    std::thread t2(ogrenci_kayit, "Ayse", "Veri_Yapilari", KAYIT_SAYISI);

    t1.join();
    t2.join();

    std::cout << "Ogrenci sayisi: "
              << ogr_depo.ogrenciler.size() << "\n";
    std::cout << "Kayit sayisi:   "
              << ders_depo.kayitlar.size() << "\n";

    // Ilk 3 kaydi goster
    std::cout << "\nIlk 3 kayit:\n";
    for (int i = 0; i < 3 && i < static_cast<int>(ders_depo.kayitlar.size()); ++i) {
        std::cout << "  " << ders_depo.kayitlar[i] << "\n";
    }

    if (ogr_depo.ogrenciler.size() == 2 * KAYIT_SAYISI) {
        std::cout << "\n[OK] Tum kayitlar basariyla tamamlandi.\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * Ogrenci sayisi: 2000
 * Kayit sayisi:   2000
 *
 * Ilk 3 kayit:
 *   Ali_0 -> Yazilim_II
 *   Ali_1 -> Yazilim_II
 *   Ayse_0 -> Veri_Yapilari
 *
 * [OK] Tum kayitlar basariyla tamamlandi.
 */

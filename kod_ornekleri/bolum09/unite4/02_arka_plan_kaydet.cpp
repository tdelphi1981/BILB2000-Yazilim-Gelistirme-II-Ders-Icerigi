/**
 * Arka Plan Kaydetme - BackgroundSaver Sinifi
 *
 * Bu program, OBYS projesinde arka planda dosya kaydetme
 * ozelligini gosterir. Mutex ile snapshot alinir, atomik
 * bayrakla durum takip edilir, ayri izlekte diske yazilir.
 *
 * Ogrenilecek kavramlar:
 * - std::atomic<bool> durum bayragi
 * - std::mutex ile veri snapshot'i
 * - Arka plan izleginde dosya I/O
 * - Snapshot al ve serbest birak stratejisi
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 4 - Atomik Islemler ve OBYS Entegrasyonu
 *
 * Derleme: g++ -std=c++20 -pthread -o 02_arka_plan_kaydet 02_arka_plan_kaydet.cpp
 * Calistirma: ./02_arka_plan_kaydet
 */

#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// Basitlestirilmis Ogrenci yapisi
struct Ogrenci {
    int numara;
    std::string ad;
    double ortalama;
};

class BackgroundSaver {
public:
    BackgroundSaver() = default;
    ~BackgroundSaver() { bekle(); }

    // Kopyalama/tasima yasak
    BackgroundSaver(const BackgroundSaver&) = delete;
    BackgroundSaver& operator=(const BackgroundSaver&) = delete;

    void kaydetBaslat(const std::string& dosyaYolu,
                      const std::vector<Ogrenci>& veri) {
        // Onceki kayit devam ediyorsa bekle
        bekle();

        // Bayragi kaldir
        kaydediliyor_.store(true, std::memory_order_release);

        // Mutex altinda veri snapshot'i al
        std::vector<Ogrenci> snapshot;
        {
            std::lock_guard<std::mutex> kilit(veriMutex_);
            snapshot = veri;  // derin kopya
        }
        // Kilit burada birakildi -- ana izlek serbest

        // Arka plan izlegini baslat
        kayitIzlegi_ = std::thread(
            &BackgroundSaver::kaydetGorev, this,
            dosyaYolu, std::move(snapshot));
    }

    bool kaydediliyorMu() const {
        return kaydediliyor_.load(std::memory_order_acquire);
    }

    void bekle() {
        if (kayitIzlegi_.joinable()) {
            kayitIzlegi_.join();
        }
    }

private:
    void kaydetGorev(const std::string& dosyaYolu,
                     std::vector<Ogrenci> snapshot) {
        std::cout << "  [Kayit izlegi] Yazma basliyor ("
                  << snapshot.size() << " kayit)...\n";

        // Gercek I/O islemini simule et
        std::ofstream dosya(dosyaYolu, std::ios::binary);
        if (dosya) {
            size_t adet = snapshot.size();
            dosya.write(reinterpret_cast<const char*>(&adet),
                        sizeof(adet));

            for (const auto& ogr : snapshot) {
                // Numara
                dosya.write(reinterpret_cast<const char*>(&ogr.numara),
                            sizeof(ogr.numara));
                // Ad uzunlugu + ad
                size_t uzunluk = ogr.ad.size();
                dosya.write(reinterpret_cast<const char*>(&uzunluk),
                            sizeof(uzunluk));
                dosya.write(ogr.ad.data(),
                            static_cast<std::streamsize>(uzunluk));
                // Ortalama
                dosya.write(
                    reinterpret_cast<const char*>(&ogr.ortalama),
                    sizeof(ogr.ortalama));
            }
            dosya.close();
        }

        // Buyuk veri yazimini simule etmek icin kisa bekleme
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Bayragi indir -- kayit tamamlandi
        kaydediliyor_.store(false, std::memory_order_release);
        std::cout << "  [Kayit izlegi] Yazma tamamlandi.\n";
    }

    std::mutex veriMutex_;
    std::atomic<bool> kaydediliyor_{false};
    std::thread kayitIzlegi_;
};

int main() {
    std::cout << "=== OBYS Arka Plan Kaydetme Ornegi ===\n\n";

    // OBYS verisini hazirla
    std::vector<Ogrenci> ogrenciler = {
        {1001, "Ali Yilmaz",    3.45},
        {1002, "Ayse Kaya",     3.78},
        {1003, "Mehmet Demir",  2.95},
        {1004, "Fatma Celik",   3.62},
        {1005, "Ahmet Ozturk",  3.10}
    };

    BackgroundSaver kaydedici;

    // Arka planda kaydet
    std::cout << "[Ana izlek] Kaydetme baslatiliyor...\n";
    kaydedici.kaydetBaslat("obys_test.bin", ogrenciler);

    // Ana izlek calismaya devam eder
    int sayac = 0;
    while (kaydedici.kaydediliyorMu()) {
        std::cout << "[Ana izlek] Calisiyorum... ("
                  << ++sayac << ")\n";
        std::this_thread::sleep_for(
            std::chrono::milliseconds(100));
    }

    std::cout << "\n[Ana izlek] Kayit tamamlandi!\n";

    // Dosya boyutunu kontrol et
    std::ifstream kontrol("obys_test.bin",
                          std::ios::binary | std::ios::ate);
    if (kontrol) {
        std::cout << "[Ana izlek] Dosya boyutu: "
                  << kontrol.tellg() << " byte\n";
    }

    // Temizlik
    std::remove("obys_test.bin");

    return 0;
}

/*
 * BEKLENEN CIKTI (ornek):
 * -----------------------
 * === OBYS Arka Plan Kaydetme Ornegi ===
 *
 * [Ana izlek] Kaydetme baslatiliyor...
 *   [Kayit izlegi] Yazma basliyor (5 kayit)...
 * [Ana izlek] Calisiyorum... (1)
 * [Ana izlek] Calisiyorum... (2)
 * [Ana izlek] Calisiyorum... (3)
 * [Ana izlek] Calisiyorum... (4)
 * [Ana izlek] Calisiyorum... (5)
 *   [Kayit izlegi] Yazma tamamlandi.
 *
 * [Ana izlek] Kayit tamamlandi!
 * [Ana izlek] Dosya boyutu: 168 byte
 */

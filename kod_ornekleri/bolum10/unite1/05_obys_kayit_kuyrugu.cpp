/**
 * OBYS Kayıt Kuyruğu
 *
 * Bu program, OBYS sisteminde UI'den gelen toplu öğrenci kayıt
 * taleplerini birden fazla arka plan işçisine paralel olarak
 * dağıtan bir kuyruk yapısını gösterir.
 *
 * Öğrenilecek kavramlar:
 * - Çoklu işçili üretici-tüketici deseni
 * - Dosya sistemi ile thread-safe etkileşim
 * - Atomic sayaç ile ilerleme izleme
 * - Destructor ile graceful shutdown
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 1 - condition_variable ile Koordinasyon
 *
 * Derleme: g++ -std=c++20 -pthread -o 05_kayit 05_obys_kayit_kuyrugu.cpp
 * Çalıştırma: ./05_kayit
 */

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <chrono>

struct Ogrenci {
    int numara;
    std::string ad;
    std::string bolum;
    double ortalama;
};

class KayitKuyrugu {
public:
    explicit KayitKuyrugu(std::size_t isci_sayisi = 3,
                          std::string dizin = "ogrenci_json")
        : dizin_(std::move(dizin))
    {
        std::filesystem::create_directories(dizin_);
        isciler_.reserve(isci_sayisi);
        for (std::size_t i = 0; i < isci_sayisi; ++i) {
            isciler_.emplace_back(&KayitKuyrugu::isci_dongusu,
                                   this, static_cast<int>(i));
        }
    }

    ~KayitKuyrugu() {
        bitir();
        for (auto& t : isciler_) {
            if (t.joinable()) t.join();
        }
    }

    void ekle(Ogrenci o) {
        if (kapaniyor_.load()) return;
        {
            std::lock_guard<std::mutex> kilit(mtx_);
            kuyruk_.push(std::move(o));
        }
        cv_.notify_one();
    }

    std::size_t yazilan_sayisi() const { return yazildi_.load(); }

    void bitir() {
        {
            std::lock_guard<std::mutex> kilit(mtx_);
            kapaniyor_ = true;
        }
        cv_.notify_all();
    }

private:
    void isci_dongusu(int id) {
        while (true) {
            Ogrenci o;
            {
                std::unique_lock<std::mutex> kilit(mtx_);
                cv_.wait(kilit, [this] {
                    return !kuyruk_.empty() || kapaniyor_.load();
                });
                if (kuyruk_.empty()) {
                    std::cout << "[İşçi " << id << "] kapanıyor\n";
                    return;
                }
                o = std::move(kuyruk_.front());
                kuyruk_.pop();
            }
            try {
                diske_yaz(o);
                yazildi_.fetch_add(1);
                std::cout << "  [İşçi " << id << "] "
                          << o.numara << " yazıldı\n";
            } catch (const std::exception& e) {
                std::cerr << "[İşçi " << id << "] hata: "
                          << e.what() << "\n";
            }
        }
    }

    void diske_yaz(const Ogrenci& o) {
        namespace fs = std::filesystem;
        auto yol = fs::path(dizin_) /
                   (std::to_string(o.numara) + ".json");
        std::ofstream cikti(yol);
        cikti << "{\n"
              << "  \"numara\": " << o.numara << ",\n"
              << "  \"ad\": \"" << o.ad << "\",\n"
              << "  \"bolum\": \"" << o.bolum << "\",\n"
              << "  \"ortalama\": " << o.ortalama << "\n"
              << "}\n";
        // Disk yavaşlığı simülasyonu
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::queue<Ogrenci> kuyruk_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic<bool> kapaniyor_{false};
    std::atomic<std::size_t> yazildi_{0};
    std::vector<std::thread> isciler_;
    std::string dizin_;
};

int main() {
    KayitKuyrugu kuyruk(3);  // 3 işçi

    // 15 öğrenci kaydı ekle (UI simülasyonu)
    for (int i = 0; i < 15; ++i) {
        Ogrenci o;
        o.numara = 10000 + i;
        o.ad = "Öğrenci " + std::to_string(i);
        o.bolum = "Bilgisayar";
        o.ortalama = 2.5 + (i % 5) * 0.3;
        kuyruk.ekle(std::move(o));
    }

    // İşçilerin bitirmesini bekle
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "\nToplam yazılan: " << kuyruk.yazilan_sayisi()
              << "\n";
    return 0;
}

/*
 * BEKLENEN DAVRANIŞ:
 * ------------------
 * 3 işçi paralel olarak dosyaları yazar. Toplam 15 dosya
 * yaklaşık 15*50/3 = 250 ms içinde oluşur (disk darboğazı olmazsa).
 * ogrenci_json/ dizininde her biri 10000-10014 numaralı JSON
 * dosyaları bulunur.
 */

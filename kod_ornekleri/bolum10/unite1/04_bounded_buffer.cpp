/**
 * Sınırlı Tampon (Bounded Buffer)
 *
 * Bu program, kapasitesi sınırlı bir kuyruğu iki koşul değişkeni
 * (not_full, not_empty) ile uygular. Üretici kuyruk dolduğunda
 * bekler, tüketici boşaldığında bekler.
 *
 * Öğrenilecek kavramlar:
 * - İki koşul değişkeni ile üretici-tüketici koordinasyonu
 * - Ring buffer veri yapısı
 * - std::optional<T> ile "yok" durumu temsili
 * - Kapatma bayrağı ve graceful shutdown
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 1 - condition_variable ile Koordinasyon
 *
 * Derleme: g++ -std=c++20 -pthread -o 04_bounded 04_bounded_buffer.cpp
 * Çalıştırma: ./04_bounded
 */

#include <mutex>
#include <condition_variable>
#include <vector>
#include <optional>
#include <thread>
#include <iostream>
#include <chrono>

template <typename T>
class SinirliBuffer {
public:
    explicit SinirliBuffer(std::size_t kapasite)
        : buffer_(kapasite), kapasite_(kapasite) {}

    void ekle(T deger) {
        std::unique_lock<std::mutex> kilit(mtx_);
        not_full_.wait(kilit, [this] {
            return sayi_ < kapasite_ || kapaniyor_;
        });
        if (kapaniyor_) return;

        buffer_[tail_] = std::move(deger);
        tail_ = (tail_ + 1) % kapasite_;
        ++sayi_;
        kilit.unlock();
        not_empty_.notify_one();
    }

    std::optional<T> al() {
        std::unique_lock<std::mutex> kilit(mtx_);
        not_empty_.wait(kilit, [this] {
            return sayi_ > 0 || kapaniyor_;
        });
        if (sayi_ == 0) return std::nullopt;

        T deger = std::move(buffer_[head_]);
        head_ = (head_ + 1) % kapasite_;
        --sayi_;
        kilit.unlock();
        not_full_.notify_one();
        return deger;
    }

    void kapat() {
        {
            std::lock_guard<std::mutex> kilit(mtx_);
            kapaniyor_ = true;
        }
        not_full_.notify_all();
        not_empty_.notify_all();
    }

private:
    std::vector<T> buffer_;
    std::size_t kapasite_;
    std::size_t head_ = 0;
    std::size_t tail_ = 0;
    std::size_t sayi_ = 0;
    bool kapaniyor_ = false;
    std::mutex mtx_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

int main() {
    SinirliBuffer<int> tampon(5);  // Kapasite 5

    std::thread uretici([&] {
        for (int i = 1; i <= 15; ++i) {
            tampon.ekle(i);
            std::cout << "[Üretici] " << i << " eklendi\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    std::thread tuketici([&] {
        for (int i = 0; i < 15; ++i) {
            auto deger = tampon.al();
            if (!deger) break;
            std::cout << "    [Tüketici] " << *deger << " alındı\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    });

    uretici.join();
    tampon.kapat();
    tuketici.join();

    std::cout << "Tamamlandı.\n";
    return 0;
}

/*
 * BEKLENEN DAVRANIŞ:
 * ------------------
 * - İlk 5 ekleme hızlı olur (tampon dolar)
 * - Sonrasında üretici her 200 ms'de bir yer açıldığında ekler
 * - Lockstep davranış: üretici ve tüketici eşitlenir
 */

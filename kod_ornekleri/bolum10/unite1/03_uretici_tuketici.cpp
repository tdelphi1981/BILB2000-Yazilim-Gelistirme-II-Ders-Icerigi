/**
 * Üretici-Tüketici Deseni -- Çoklu Tüketici
 *
 * Bu program, bir üretici ve üç tüketici arasında bir iş kuyruğu
 * üzerinden koordinasyonu gösterir. Her tüketici kuyruktan bir iş
 * çekip işler; üretici ise düzenli aralıklarla yeni işler ekler.
 *
 * Öğrenilecek kavramlar:
 * - std::queue ile paylaşımlı iş kuyruğu
 * - std::function<void()> ile tip-silme
 * - Çoklu tüketiciyle notify_one kullanımı
 * - Kapatma bayrağı ile temiz sonlandırma
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 1 - condition_variable ile Koordinasyon
 *
 * Derleme: g++ -std=c++20 -pthread -o 03_upt 03_uretici_tuketici.cpp
 * Çalıştırma: ./03_upt
 */

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <functional>
#include <atomic>
#include <iostream>
#include <chrono>

using Gorev = std::function<void()>;

class GorevKuyrugu {
    std::queue<Gorev> kuyruk_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic<bool> kapaniyor_{false};

public:
    void ekle(Gorev g) {
        {
            std::lock_guard<std::mutex> kilit(mtx_);
            kuyruk_.push(std::move(g));
        }
        cv_.notify_one();
    }

    bool al(Gorev& cikti) {
        std::unique_lock<std::mutex> kilit(mtx_);
        cv_.wait(kilit, [this] {
            return !kuyruk_.empty() || kapaniyor_.load();
        });
        if (kuyruk_.empty()) return false;

        cikti = std::move(kuyruk_.front());
        kuyruk_.pop();
        return true;
    }

    void kapat() {
        kapaniyor_.store(true);
        cv_.notify_all();
    }
};

int main() {
    GorevKuyrugu kuyruk;

    // 3 tüketici başlat
    std::vector<std::thread> tuketiciler;
    for (int id = 0; id < 3; ++id) {
        tuketiciler.emplace_back([&kuyruk, id] {
            Gorev g;
            while (kuyruk.al(g)) {
                g();
                std::cout << "[İşçi " << id << "] görev bitti\n";
            }
            std::cout << "[İşçi " << id << "] kapanıyor\n";
        });
    }

    // Üretici: 10 görev ekle
    for (int i = 0; i < 10; ++i) {
        kuyruk.ekle([i] {
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
            std::cout << "  -> Görev " << i << " işlendi\n";
        });
    }

    // Tüm görevlerin işlenmesine zaman ver
    std::this_thread::sleep_for(std::chrono::seconds(1));
    kuyruk.kapat();

    for (auto& t : tuketiciler) t.join();

    std::cout << "Tüm görevler tamamlandı.\n";
    return 0;
}

/*
 * BEKLENEN ÇIKTI (sıra değişir):
 * ------------------------------
 *   -> Görev 0 işlendi
 *   -> Görev 1 işlendi
 *   -> Görev 2 işlendi
 * [İşçi 0] görev bitti
 * [İşçi 1] görev bitti
 * ...
 * [İşçi 0] kapanıyor
 * [İşçi 1] kapanıyor
 * [İşçi 2] kapanıyor
 * Tüm görevler tamamlandı.
 */

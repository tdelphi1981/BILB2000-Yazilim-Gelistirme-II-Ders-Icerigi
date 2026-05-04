/**
 * Thread-Safe Depo with shared_mutex
 *
 * Bölüm 11 Ünite 1 Konu 3
 *
 * Bu örnekte:
 *  - shared_mutex ile çoklu okuyucu / tek yazıcı kalıbı
 *  - Şablon Depo<T> sınıfı (OBYS uyumlu)
 *  - 100K öğrencilik depo, 8 paralel okuyucu testi
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 03_depo 03_shared_mutex_depo.cpp
 */

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

struct Ogrenci {
    int numara;
    std::string ad;
    std::string bolum;

    Ogrenci(int n, std::string a, std::string b)
        : numara(n), ad(std::move(a)), bolum(std::move(b)) {}
};

template <typename T>
class Depo {
public:
    Depo() = default;
    Depo(const Depo&) = delete;
    Depo& operator=(const Depo&) = delete;

    std::shared_ptr<T> bul(int id) const {
        std::shared_lock<std::shared_mutex> kilit(sm_);
        auto it = kayitlar_.find(id);
        return (it != kayitlar_.end()) ? it->second : nullptr;
    }

    void ekle(int id, std::shared_ptr<T> kayit) {
        std::unique_lock<std::shared_mutex> kilit(sm_);
        kayitlar_[id] = std::move(kayit);
    }

    bool sil(int id) {
        std::unique_lock<std::shared_mutex> kilit(sm_);
        return kayitlar_.erase(id) > 0;
    }

    std::vector<std::shared_ptr<T>> tumKayitlar() const {
        std::shared_lock<std::shared_mutex> kilit(sm_);
        std::vector<std::shared_ptr<T>> v;
        v.reserve(kayitlar_.size());
        for (const auto& [k, p] : kayitlar_) v.push_back(p);
        return v;
    }

    std::size_t boyut() const {
        std::shared_lock<std::shared_mutex> kilit(sm_);
        return kayitlar_.size();
    }

private:
    mutable std::shared_mutex sm_;
    std::unordered_map<int, std::shared_ptr<T>> kayitlar_;
};

int main() {
    Depo<Ogrenci> depo;

    constexpr int N_OGRENCI = 100'000;
    constexpr int N_THREAD_OKU = 8;
    constexpr int OKUMA_BASINA = 50'000;

    // Verileri tek thread ile doldur
    for (int i = 0; i < N_OGRENCI; ++i) {
        depo.ekle(i, std::make_shared<Ogrenci>(
            i, "Ogrenci_" + std::to_string(i), "Bilgisayar"));
    }

    std::cout << "Depoda " << depo.boyut() << " kayit var.\n";

    // Paralel okuma testi
    auto baslangic = std::chrono::steady_clock::now();
    std::atomic<int> bulundu{0}, bulunamadi{0};

    std::vector<std::thread> okuyucular;
    for (int t = 0; t < N_THREAD_OKU; ++t) {
        okuyucular.emplace_back([&, t] {
            std::mt19937 rng(t);
            std::uniform_int_distribution<int> dist(0, N_OGRENCI - 1);
            for (int i = 0; i < OKUMA_BASINA; ++i) {
                int id = dist(rng);
                auto p = depo.bul(id);
                if (p) bulundu.fetch_add(1, std::memory_order_relaxed);
                else bulunamadi.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    for (auto& t : okuyucular) t.join();

    auto sure = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - baslangic);

    std::cout << "Toplam okuma: " << (bulundu + bulunamadi) << '\n';
    std::cout << "Bulundu: " << bulundu << ", Bulunamadi: " << bulunamadi << '\n';
    std::cout << "Sure: " << sure.count() << " ms\n";
    std::cout << "Hiz: " << (bulundu + bulunamadi) * 1000 / sure.count()
              << " ops/sn\n";

    return 0;
}

/*
BEKLENEN CIKTI (8 cekirdek):
----------------------------
Depoda 100000 kayit var.
Toplam okuma: 400000
Bulundu: 400000, Bulunamadi: 0
Sure: 30-60 ms
Hiz: 7-13 milyon ops/sn
*/

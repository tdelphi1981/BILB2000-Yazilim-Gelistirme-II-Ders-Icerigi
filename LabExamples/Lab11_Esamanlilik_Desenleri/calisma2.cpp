/**
 * Lab 11 - Çalışma 2: Sharded ConcurrentMap
 *
 * Bu dosyadaki GOREV yorumlarını uygun kodla değiştirin.
 *
 * Derleme: g++ -std=c++20 -pthread -O2 -o calisma2 calisma2.cpp
 * Çalıştırma: ./calisma2
 */

#include <atomic>
#include <chrono>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class ConcurrentMap {
public:
    explicit ConcurrentMap(std::size_t bolmeSayisi = 16)
        : bolmeler_(bolmeSayisi)
    {}

    void koy(const Key& k, Value v) {
        // GOREV 1: bolmeBul ile bolmeyi sec
        //   - auto& b = bolmeBul(k);
        // GOREV 2: bolmenin mutex'ini kilitle, b.veri[k] = std::move(v) yap

    }

    std::optional<Value> al(const Key& k) const {
        // GOREV 3: bolmeBul ile sec, kilitle, find ile ara
        //   - Bulunamazsa std::nullopt
        //   - Bulunursa it->second

        return std::nullopt;
    }

    bool sil(const Key& k) {
        // GOREV 4: bolmeBul, kilitle, b.veri.erase(k) > 0 dondur

        return false;
    }

    std::size_t boyut() const {
        // GOREV 5: TUM bolmeleri sirayla kilitle, boyutlari topla

        return 0;
    }

private:
    struct Bolme {
        mutable std::mutex m;
        std::unordered_map<Key, Value> veri;
    };

    Bolme& bolmeBul(const Key& k) {
        return bolmeler_[Hash{}(k) % bolmeler_.size()];
    }
    const Bolme& bolmeBul(const Key& k) const {
        return bolmeler_[Hash{}(k) % bolmeler_.size()];
    }

    // !!! UNUTMA: vector DEGIL! Mutex tasinmaz; deque blok-blok bellek kullanir.
    std::deque<Bolme> bolmeler_;
};

// =============================================================================
// TEST: 8 thread paralel yazar, 8 thread paralel okur
// =============================================================================
int main() {
    ConcurrentMap<int, std::string> map(16);

    constexpr int N_THREAD = 8;
    constexpr int IS_BASINA = 5000;

    std::vector<std::thread> isciler;
    std::atomic<int> hata{0};

    auto baslangic = std::chrono::steady_clock::now();

    // Yazıcılar
    for (int t = 0; t < N_THREAD; ++t) {
        isciler.emplace_back([&, t] {
            for (int i = 0; i < IS_BASINA; ++i) {
                int k = t * IS_BASINA + i;
                map.koy(k, "deger_" + std::to_string(k));
            }
        });
    }
    for (auto& w : isciler) w.join();
    isciler.clear();

    // Okuyucular
    for (int t = 0; t < N_THREAD; ++t) {
        isciler.emplace_back([&, t] {
            for (int i = 0; i < IS_BASINA; ++i) {
                int k = t * IS_BASINA + i;
                auto v = map.al(k);
                if (!v.has_value() || *v != "deger_" + std::to_string(k)) {
                    hata.fetch_add(1);
                }
            }
        });
    }
    for (auto& w : isciler) w.join();

    auto sure = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - baslangic);

    std::cout << "Toplam kayit: " << map.boyut() << '\n';
    std::cout << "Hata sayisi: " << hata.load() << '\n';
    std::cout << "Sure: " << sure.count() << " ms\n";

    return 0;
}

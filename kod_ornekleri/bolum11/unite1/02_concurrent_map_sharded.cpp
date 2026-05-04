/**
 * Sharded Concurrent Map
 *
 * Bölüm 11 Ünite 1 Konu 2
 *
 * Bu örnekte:
 *  - Anahtar hash'i ile bölmelere dağıtan ConcurrentMap
 *  - Her bölmenin kendi mutex'i var
 *  - Çoklu thread paralel erişimi destekler
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 02_cmap 02_concurrent_map_sharded.cpp
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
        auto& b = bolmeBul(k);
        std::lock_guard<std::mutex> kilit(b.m);
        b.veri[k] = std::move(v);
    }

    std::optional<Value> al(const Key& k) const {
        auto& b = bolmeBul(k);
        std::lock_guard<std::mutex> kilit(b.m);
        auto it = b.veri.find(k);
        if (it == b.veri.end()) return std::nullopt;
        return it->second;
    }

    bool sil(const Key& k) {
        auto& b = bolmeBul(k);
        std::lock_guard<std::mutex> kilit(b.m);
        return b.veri.erase(k) > 0;
    }

    std::size_t boyut() const {
        std::size_t toplam = 0;
        for (const auto& b : bolmeler_) {
            std::lock_guard<std::mutex> kilit(b.m);
            toplam += b.veri.size();
        }
        return toplam;
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

    std::deque<Bolme> bolmeler_;
};

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
            std::mt19937 rng(t * 1000);
            for (int i = 0; i < IS_BASINA; ++i) {
                int k = t * IS_BASINA + i;
                map.koy(k, "deger_" + std::to_string(k));
            }
        });
    }
    for (auto& w : isciler) w.join();
    isciler.clear();

    // Okuyucular (paralel)
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

/*
BEKLENEN CIKTI (8 cekirdek):
----------------------------
Toplam kayit: 40000
Hata sayisi: 0
Sure: 80-150 ms
*/

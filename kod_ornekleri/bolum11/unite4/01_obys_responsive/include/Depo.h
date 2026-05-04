#pragma once
#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

/**
 * Depo<T> - Thread-Safe Genel Depo
 *
 * Bölüm 11 Ünite 1 ve 4'te detaylı işlendi.
 *
 * shared_mutex ile çoklu okuyucu / tek yazıcı garantisi.
 * Arayüz Bölüm 1'deki sürüme uyumlu; sadece içeride kilit eklendi.
 */
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
        for (const auto& [k, p] : kayitlar_) {
            v.push_back(p);
        }
        return v;
    }

    std::size_t boyut() const {
        std::shared_lock<std::shared_mutex> kilit(sm_);
        return kayitlar_.size();
    }

    template <typename F>
    bool guncelle(int id, F donusturucu) {
        std::unique_lock<std::shared_mutex> kilit(sm_);
        auto it = kayitlar_.find(id);
        if (it == kayitlar_.end()) return false;
        donusturucu(*it->second);
        return true;
    }

    void temizle() {
        std::unique_lock<std::shared_mutex> kilit(sm_);
        kayitlar_.clear();
    }

private:
    mutable std::shared_mutex sm_;
    std::unordered_map<int, std::shared_ptr<T>> kayitlar_;
};

#pragma once
// Depo<K,V> - header-only genel amacli depolama sinifi
// map, optional ve ranges destegi ile
// Bolum: 03 - Unite 5
// Derleme: C++20

#include <map>
#include <optional>
#include <stdexcept>
#include <functional>

template <typename K, typename V>
class Depo {
public:
    void ekle(const K& anahtar, const V& deger) {
        if (_elemanlar.contains(anahtar)) {
            throw std::runtime_error("Eleman zaten mevcut!");
        }
        _elemanlar.emplace(anahtar, deger);
    }

    std::optional<V> bul(const K& anahtar) const {
        auto it = _elemanlar.find(anahtar);
        if (it != _elemanlar.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    const V& getir(const K& anahtar) const {
        auto it = _elemanlar.find(anahtar);
        if (it == _elemanlar.end()) {
            throw std::runtime_error("Eleman bulunamadi!");
        }
        return it->second;
    }

    bool sil(const K& anahtar) {
        return _elemanlar.erase(anahtar) > 0;
    }

    void guncelle(const K& anahtar, const V& deger) {
        auto it = _elemanlar.find(anahtar);
        if (it == _elemanlar.end()) {
            throw std::runtime_error("Guncellenecek eleman bulunamadi!");
        }
        it->second = deger;
    }

    const std::map<K, V>& tumunu_al() const {
        return _elemanlar;
    }

    std::size_t boyut() const {
        return _elemanlar.size();
    }

    bool bos_mu() const {
        return _elemanlar.empty();
    }

    void temizle() {
        _elemanlar.clear();
    }

    bool iceriyor_mu(const K& anahtar) const {
        return _elemanlar.contains(anahtar);
    }

    auto begin() const { return _elemanlar.cbegin(); }
    auto end() const { return _elemanlar.cend(); }

private:
    std::map<K, V> _elemanlar;
};

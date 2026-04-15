#pragma once

/**
 * depo.h -- Genel Amaçlı Depo Şablon Sınıfı
 *
 * Anahtar-değer çifti olarak veri saklayan şablon sınıf.
 * Öğrenciler GOREV işaretli yerleri tamamlayacaktır.
 *
 * Derleme: g++ -std=c++20 -o arkis src/main.cpp
 */

#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <ranges>
#include <vector>

template <typename Anahtar, typename Deger>
class Depo {
public:
    // Yeni kayıt ekler. Anahtar zaten varsa false döner.
    bool ekle(const Anahtar& anahtar, const Deger& deger) {
        auto [it, eklendi] = m_veriler.insert({anahtar, deger});
        return eklendi;
    }

    // === GOREV 1.2a - bul() Metodu (10 dk) ===
    // TODO: Verilen anahtarı map'te ara.
    //       Bulursa std::optional<Deger> olarak dön,
    //       bulamazsa std::nullopt dön.
    // ==========================================
    std::optional<Deger> bul(const Anahtar& anahtar) const {
        // Ogrenci bu metodu tamamlayacak
        return std::nullopt;
    }

    // === GOREV 1.2b - sil() Metodu (5 dk) ===
    // TODO: Verilen anahtarı map'ten sil.
    //       Başarılıysa true, bulunamadıysa false dön.
    // =========================================
    bool sil(const Anahtar& anahtar) {
        // Ogrenci bu metodu tamamlayacak
        return false;
    }

    // Tüm kayıtları döner (salt okunur)
    const std::map<Anahtar, Deger>& tumunu_al() const {
        return m_veriler;
    }

    // Kayıt sayısını döner
    std::size_t boyut() const {
        return m_veriler.size();
    }

    // Depoyu temizler
    void temizle() {
        m_veriler.clear();
    }

    // === GOREV 1.2c - filtrele() Metodu (15 dk) ===
    // TODO: Bir lambda/fonksiyon nesnesi (kosul) alarak
    //       kosulu saglayan tüm değerleri std::vector olarak dön.
    //       Ipucu: std::ranges::views::filter ve
    //              std::ranges::views::values kullanabilirsin.
    // ===============================================
    std::vector<Deger> filtrele(
        std::function<bool(const Deger&)> kosul) const
    {
        std::vector<Deger> sonuc;
        // Ogrenci bu metodu tamamlayacak
        return sonuc;
    }

private:
    std::map<Anahtar, Deger> m_veriler;
};

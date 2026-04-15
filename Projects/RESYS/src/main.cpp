/**
 * main.cpp -- RESYS Başlangıç Noktası
 *
 * Restoran Sipariş ve Stok Yönetim Sistemi - Faz 1 başlangıç kodu.
 * Bu dosya derlenip çalıştırılabilir durumdadır.
 * GOREV işaretli yerleri tamamlayarak sistemi geliştirin.
 *
 * Derleme: g++ -std=c++20 -o resys src/main.cpp
 * Çalıştırma: ./resys
 */

#include "../include/varliklar.h"
#include "../include/depo.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <ranges>
#include <string>

int main() {
    std::cout << "=== RESYS - Restoran Siparis ve Stok Yonetimi ===\n\n";

    // -- Menu deposu olustur ve ornek veriler ekle --
    Depo<int, MenuItem> menu;

    menu.ekle(101, {101, "Adana Kebap", "Ana Yemek", 280.0});
    menu.ekle(102, {102, "Iskender", "Ana Yemek", 320.0});
    menu.ekle(103, {103, "Mercimek Corbasi", "Corba", 85.0});
    menu.ekle(104, {104, "Ayran", "Icecek", 35.0});
    menu.ekle(105, {105, "Kunefe", "Tatli", 150.0});
    menu.ekle(106, {106, "Pide (Kiymali)", "Ana Yemek", 220.0});

    std::cout << "Menu ogesi sayisi: " << menu.boyut() << "\n\n";

    // Tum menu ogelerini listele
    std::cout << "--- Menu ---\n";
    for (const auto& [id, item] : menu.tumunu_al()) {
        std::cout << "  " << item << "\n";
    }
    std::cout << "\n";

    // -- Siparis deposu olustur --
    Depo<int, Siparis> siparisler;

    siparisler.ekle(1, {1, 3, "2026-04-14 12:30",
        {{101, 2, "Az acili"}, {103, 1, std::nullopt}, {104, 2, std::nullopt}},
        SiparisDurum::Hazirlaniyor});
    siparisler.ekle(2, {2, 7, "2026-04-14 12:45",
        {{102, 1, "Bol soslu"}, {105, 2, std::nullopt}},
        SiparisDurum::Hazir});
    siparisler.ekle(3, {3, 1, "2026-04-14 11:00",
        {{106, 1, std::nullopt}, {104, 1, std::nullopt}},
        SiparisDurum::TeslimEdildi});

    std::cout << "Siparis sayisi: " << siparisler.boyut() << "\n\n";

    std::cout << "--- Siparisler ---\n";
    for (const auto& [id, siparis] : siparisler.tumunu_al()) {
        std::cout << "  " << siparis << "\n";
        for (const auto& kalem : siparis.kalemler) {
            std::cout << "    - " << kalem << "\n";
        }
    }
    std::cout << "\n";

    // -- Stok deposu olustur --
    Depo<int, StokMalzeme> stok;

    stok.ekle(1, {1, "Dana Kiyma", 25.0, "kg", 10.0});
    stok.ekle(2, {2, "Pide Hamuru", 40.0, "kg", 15.0});
    stok.ekle(3, {3, "Ayran", 8.0, "lt", 20.0});    // Minimum altinda!
    stok.ekle(4, {4, "Mercimek", 30.0, "kg", 5.0});
    stok.ekle(5, {5, "Kunefe Peyniri", 3.0, "kg", 5.0}); // Minimum altinda!

    std::cout << "Stok malzeme sayisi: " << stok.boyut() << "\n\n";

    std::cout << "--- Stok Durumu ---\n";
    for (const auto& [id, malzeme] : stok.tumunu_al()) {
        std::cout << "  " << malzeme;
        if (malzeme.miktar < malzeme.minimum_stok) {
            std::cout << "  [!] STOK DUSUK";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    // === GOREV 1.3a - Kategori Filtreleme (10 dk) ===
    // TODO: "Ana Yemek" kategorisindeki menu ogelerini filtrele ve listele.
    //       Ipucu: menu.filtrele(...) metodunu kullan.
    // ================================================

    // === GOREV 1.3b - Fiyata Gore Siralama (10 dk) ===
    // TODO: Menu ogelerini fiyata gore sirala ve listele.
    //       Ipucu: tumunu_al() ile alip bir vector'e kopyala,
    //              sonra std::ranges::sort kullan.
    // =================================================

    // === GOREV 1.3c - Siparis Kuyrugu (15 dk) ===
    // TODO: Hazirlaniyor durumundaki siparisleri bir std::queue'ya ekle.
    //       Kuyruktan sirayla cikarip isleyen bir dongu yaz.
    //       Her siparisin durumunu Hazir olarak guncelle.
    // =============================================

    // === GOREV 1.3d - Masa Hesabi (10 dk) ===
    // TODO: Masa 3'un toplam hesabini hesapla.
    //       Her siparis kaleminin urun_id'si ile menu'den fiyati bul,
    //       adet ile carpip topla.
    // ========================================

    // === GOREV 1.3e - Dusuk Stok Raporu (10 dk) ===
    // TODO: Stok seviyesi minimum_stok altinda olan malzemeleri bul ve listele.
    //       Ipucu: stok.filtrele(...) metodunu kullan.
    // ==============================================

    // === GOREV 1.4 - Dosya Islemleri (20 dk) ===
    // TODO: Menu deposunu bir dosyaya kaydet ve geri oku.
    //       Ikili (binary) format kullanabilirsin.
    // ============================================

    std::cout << "=== Program sona erdi ===\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== RESYS - Restoran Siparis ve Stok Yonetimi ===

Menu ogesi sayisi: 6

--- Menu ---
  [101] Adana Kebap (Ana Yemek) 280 TL
  [102] Iskender (Ana Yemek) 320 TL
  [103] Mercimek Corbasi (Corba) 85 TL
  [104] Ayran (Icecek) 35 TL
  [105] Kunefe (Tatli) 150 TL
  [106] Pide (Kiymali) (Ana Yemek) 220 TL

Siparis sayisi: 3

--- Siparisler ---
  [Siparis #1] Masa: 3, Tarih: 2026-04-14 12:30, Durum: Hazirlaniyor, Kalem sayisi: 3
    - Urun #101 x2 [Not: Az acili]
    - Urun #103 x1
    - Urun #104 x2
  [Siparis #2] Masa: 7, Tarih: 2026-04-14 12:45, Durum: Hazir, Kalem sayisi: 2
    - Urun #102 x1 [Not: Bol soslu]
    - Urun #105 x2
  [Siparis #3] Masa: 1, Tarih: 2026-04-14 11:00, Durum: Teslim Edildi, Kalem sayisi: 2
    - Urun #106 x1
    - Urun #104 x1

Stok malzeme sayisi: 5

--- Stok Durumu ---
  [1] Dana Kiyma: 25 kg (Min: 10 kg)
  [2] Pide Hamuru: 40 kg (Min: 15 kg)
  [3] Ayran: 8 lt (Min: 20 lt)  [!] STOK DUSUK
  [4] Mercimek: 30 kg (Min: 5 kg)
  [5] Kunefe Peyniri: 3 kg (Min: 5 kg)  [!] STOK DUSUK

=== Program sona erdi ===
*/

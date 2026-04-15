/**
 * main.cpp -- KUBYS Başlangıç Noktası
 *
 * Kütüphane Yönetim Sistemi - Faz 1 başlangıç kodu.
 * Bu dosya derlenip çalıştırılabilir durumdadır.
 * GOREV işaretli yerleri tamamlayarak sistemi geliştirin.
 *
 * Derleme: g++ -std=c++20 -o kubys src/main.cpp
 * Çalıştırma: ./kubys
 */

#include "../include/varliklar.h"
#include "../include/depo.h"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>

int main() {
    std::cout << "=== KUBYS - Kutuphane Yonetim Sistemi ===\n\n";

    // -- Kitap deposu olustur ve ornek veriler ekle --
    Depo<std::string, Kitap> kitaplar;

    kitaplar.ekle("978-605-1234-01-1", {
        "978-605-1234-01-1", "C++ Programlama",
        "Bjarne Stroustrup", 2022, "Programlama", 3
    });
    kitaplar.ekle("978-605-1234-02-8", {
        "978-605-1234-02-8", "Veri Yapilari ve Algoritmalar",
        "Ayse Kaya", 2021, "Bilgisayar Bilimi", 2
    });
    kitaplar.ekle("978-605-1234-03-5", {
        "978-605-1234-03-5", "Qt ile GUI Gelistirme",
        "Mehmet Demir", 2023, "Programlama", 1
    });

    std::cout << "Kitap sayisi: " << kitaplar.boyut() << "\n\n";

    // Tüm kitaplari listele
    std::cout << "--- Kitap Katalogu ---\n";
    for (const auto& [isbn, kitap] : kitaplar.tumunu_al()) {
        std::cout << "  " << kitap << "\n";
    }
    std::cout << "\n";

    // -- Üye deposu olustur --
    Depo<int, Uye> uyeler;

    uyeler.ekle(1001, {1001, "Ali", "Yilmaz", "555-0101", "2024-09-01"});
    uyeler.ekle(1002, {1002, "Zeynep", "Kara", "555-0102", "2024-09-15"});

    std::cout << "Uye sayisi: " << uyeler.boyut() << "\n\n";

    // -- Ödünç kaydi deposu olustur --
    Depo<int, OduncKaydi> oduncler;

    oduncler.ekle(1, {1, 1001, "978-605-1234-01-1",
        "2025-03-01", std::nullopt, OduncDurum::Oduncte});
    oduncler.ekle(2, {2, 1002, "978-605-1234-02-8",
        "2025-02-15", "2025-03-10", OduncDurum::IadeEdildi});
    oduncler.ekle(3, {3, 1001, "978-605-1234-03-5",
        "2025-01-10", std::nullopt, OduncDurum::Gecikmis});

    std::cout << "--- Odunc Kayitlari ---\n";
    for (const auto& [id, kayit] : oduncler.tumunu_al()) {
        std::cout << "  " << kayit << "\n";
    }
    std::cout << "\n";

    // === GOREV 1.3a - Kategori Filtreleme (10 dk) ===
    // TODO: "Programlama" kategorisindeki kitaplari filtrele ve listele.
    //       Ipucu: kitaplar.filtrele(...) metodunu kullan.
    // ================================================

    // === GOREV 1.3b - Gecikme Raporu (10 dk) ===
    // TODO: Durumu "Gecikmis" olan odunc kayitlarini bul ve listele.
    // ============================================

    // === GOREV 1.3c - Siralama (10 dk) ===
    // TODO: Kitaplari yayin yilina gore sirala ve listele.
    //       Ipucu: tumunu_al() ile alip bir vector'e kopyala,
    //              sonra std::ranges::sort kullan.
    // =====================================

    // === GOREV 1.4 - Dosya Islemleri (20 dk) ===
    // TODO: Kitap deposunu bir dosyaya kaydet ve geri oku.
    //       Ikili (binary) format kullanabilirsin.
    // ============================================

    std::cout << "=== Program sona erdi ===\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== KUBYS - Kutuphane Yonetim Sistemi ===

Kitap sayisi: 3

--- Kitap Katalogu ---
  [978-605-1234-01-1] C++ Programlama - Bjarne Stroustrup (2022) [Kategori: Programlama, Kopya: 3]
  [978-605-1234-02-8] Veri Yapilari ve Algoritmalar - Ayse Kaya (2021) [Kategori: Bilgisayar Bilimi, Kopya: 2]
  [978-605-1234-03-5] Qt ile GUI Gelistirme - Mehmet Demir (2023) [Kategori: Programlama, Kopya: 1]

Uye sayisi: 2

--- Odunc Kayitlari ---
  [Kayit #1] Uye: 1001, ISBN: 978-605-1234-01-1, Odunc: 2025-03-01, Iade: ---, Durum: Oduncte
  [Kayit #2] Uye: 1002, ISBN: 978-605-1234-02-8, Odunc: 2025-02-15, Iade: 2025-03-10, Durum: Iade Edildi
  [Kayit #3] Uye: 1001, ISBN: 978-605-1234-03-5, Odunc: 2025-01-10, Iade: ---, Durum: Gecikmis

=== Program sona erdi ===
*/

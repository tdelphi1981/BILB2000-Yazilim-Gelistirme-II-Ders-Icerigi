/**
 * main.cpp -- ARKIS Başlangıç Noktası
 *
 * Araç Kiralama Sistemi - Faz 1 başlangıç kodu.
 * Bu dosya derlenip çalıştırılabilir durumdadır.
 * GOREV işaretli yerleri tamamlayarak sistemi geliştirin.
 *
 * Derleme: g++ -std=c++20 -o arkis src/main.cpp
 * Çalıştırma: ./arkis
 */

#include "../include/varliklar.h"
#include "../include/depo.h"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>

int main() {
    std::cout << "=== ARKIS - Arac Kiralama Sistemi ===\n\n";

    // -- Arac deposu olustur ve ornek veriler ekle --
    Depo<std::string, Arac> araclar;

    araclar.ekle("34ABC123", {
        "34ABC123", "Toyota", "Corolla",
        2023, "Benzin", 850.0, AracDurum::Musait
    });
    araclar.ekle("06DEF456", {
        "06DEF456", "Volkswagen", "Passat",
        2022, "Dizel", 1100.0, AracDurum::Kirada
    });
    araclar.ekle("35GHI789", {
        "35GHI789", "Renault", "Clio",
        2024, "Benzin", 650.0, AracDurum::Musait
    });
    araclar.ekle("16JKL012", {
        "16JKL012", "Ford", "Focus",
        2021, "Dizel", 750.0, AracDurum::Bakimda
    });

    std::cout << "Arac sayisi: " << araclar.boyut() << "\n\n";

    // Tum araclari listele
    std::cout << "--- Arac Katalogu ---\n";
    for (const auto& [plaka, arac] : araclar.tumunu_al()) {
        std::cout << "  " << arac << "\n";
    }
    std::cout << "\n";

    // -- Musteri deposu olustur --
    Depo<std::string, Musteri> musteriler;

    musteriler.ekle("12345678901", {
        "12345678901", "Ali", "Yilmaz", "555-0101", "B-123456"
    });
    musteriler.ekle("98765432109", {
        "98765432109", "Zeynep", "Kara", "555-0102", "B-654321"
    });
    musteriler.ekle("11223344556", {
        "11223344556", "Mehmet", "Demir", "555-0103", "B-112233"
    });

    std::cout << "Musteri sayisi: " << musteriler.boyut() << "\n\n";

    // -- Kiralama sozlesmesi deposu olustur --
    Depo<int, KiralamaSozlesmesi> sozlesmeler;

    sozlesmeler.ekle(1, {1, "06DEF456", "12345678901",
        "2025-04-01", "2025-04-05", 4400.0});
    sozlesmeler.ekle(2, {2, "34ABC123", "98765432109",
        "2025-04-10", std::nullopt, 0.0});
    sozlesmeler.ekle(3, {3, "35GHI789", "11223344556",
        "2025-03-20", "2025-03-25", 3250.0});

    std::cout << "--- Kiralama Sozlesmeleri ---\n";
    for (const auto& [id, sozlesme] : sozlesmeler.tumunu_al()) {
        std::cout << "  " << sozlesme << "\n";
    }
    std::cout << "\n";

    // === GOREV 1.3a - Musait Arac Filtreleme (10 dk) ===
    // TODO: Durumu "Musait" olan araclari filtrele ve listele.
    //       Ipucu: araclar.filtrele(...) metodunu kullan.
    //       Bonus: Benzersiz plaka listesini std::set ile olustur.
    // ===================================================

    // === GOREV 1.3b - Takvim Gorunumu (15 dk) ===
    // TODO: Kiralamalari baslangic tarihine gore grupla.
    //       map<string, vector<KiralamaSozlesmesi>> kullan.
    //       Her tarih icin kiralamalari listele.
    // =============================================

    // === GOREV 1.3c - Toplam Tutar Hesaplama (10 dk) ===
    // TODO: Tum tamamlanmis kiralamalarin toplam tutarini hesapla.
    //       Ipucu: std::accumulate veya ranges kullan.
    // ===================================================

    // === GOREV 1.3d - Musteri Kiralama Gecmisi (10 dk) ===
    // TODO: Belirli bir musterinin (TC no ile) tum kiralama
    //       gecmisini sorgula ve listele.
    // =====================================================

    // === GOREV 1.4 - Dosya Islemleri (20 dk) ===
    // TODO: Arac deposunu bir dosyaya kaydet ve geri oku.
    //       Ikili (binary) format kullanabilirsin.
    // ============================================

    std::cout << "=== Program sona erdi ===\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== ARKIS - Arac Kiralama Sistemi ===

Arac sayisi: 4

--- Arac Katalogu ---
  [06DEF456] Volkswagen Passat (2022) [Yakit: Dizel, Ucret: 1100 TL/gun, Durum: Kirada]
  [16JKL012] Ford Focus (2021) [Yakit: Dizel, Ucret: 750 TL/gun, Durum: Bakimda]
  [34ABC123] Toyota Corolla (2023) [Yakit: Benzin, Ucret: 850 TL/gun, Durum: Musait]
  [35GHI789] Renault Clio (2024) [Yakit: Benzin, Ucret: 650 TL/gun, Durum: Musait]

Musteri sayisi: 3

--- Kiralama Sozlesmeleri ---
  [Sozlesme #1] Plaka: 06DEF456, TC: 12345678901, Baslangic: 2025-04-01, Bitis: 2025-04-05, Tutar: 4400 TL
  [Sozlesme #2] Plaka: 34ABC123, TC: 98765432109, Baslangic: 2025-04-10, Bitis: ---, Tutar: 0 TL
  [Sozlesme #3] Plaka: 35GHI789, TC: 11223344556, Baslangic: 2025-03-20, Bitis: 2025-03-25, Tutar: 3250 TL

=== Program sona erdi ===
*/

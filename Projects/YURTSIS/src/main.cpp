/**
 * main.cpp -- YURTSIS Başlangıç Noktası
 *
 * Öğrenci Yurt Yönetim Sistemi - Faz 1 başlangıç kodu.
 * Bu dosya derlenip çalıştırılabilir durumdadır.
 * GOREV işaretli yerleri tamamlayarak sistemi geliştirin.
 *
 * Derleme: g++ -std=c++20 -o yurtsis src/main.cpp
 * Çalıştırma: ./yurtsis
 */

#include "../include/varliklar.h"
#include "../include/depo.h"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>

int main() {
    std::cout << "=== YURTSIS - Ogrenci Yurt Yonetim Sistemi ===\n\n";

    // -- Ogrenci deposu olustur ve ornek veriler ekle --
    Depo<int, Ogrenci> ogrenciler;

    ogrenciler.ekle(2001, {2001, "Ali", "Yilmaz", "Bilgisayar Muhendisligi", 2});
    ogrenciler.ekle(2002, {2002, "Zeynep", "Kara", "Elektrik Elektronik", 1});
    ogrenciler.ekle(2003, {2003, "Mehmet", "Demir", "Makine Muhendisligi", 3});
    ogrenciler.ekle(2004, {2004, "Ayse", "Celik", "Bilgisayar Muhendisligi", 1});

    std::cout << "Ogrenci sayisi: " << ogrenciler.boyut() << "\n\n";

    // Tum ogrencileri listele
    std::cout << "--- Ogrenci Listesi ---\n";
    for (const auto& [no, ogr] : ogrenciler.tumunu_al()) {
        std::cout << "  " << ogr << "\n";
    }
    std::cout << "\n";

    // -- Oda deposu olustur --
    Depo<std::string, Oda> odalar;

    odalar.ekle("A-101", {"A-101", "A", 2, "Cift", 1500.0});
    odalar.ekle("A-102", {"A-102", "A", 2, "Cift", 1500.0});
    odalar.ekle("A-201", {"A-201", "A", 1, "Tek", 2500.0});
    odalar.ekle("B-101", {"B-101", "B", 4, "Dort", 900.0});
    odalar.ekle("B-102", {"B-102", "B", 3, "Uc", 1100.0});

    std::cout << "Oda sayisi: " << odalar.boyut() << "\n\n";

    std::cout << "--- Oda Listesi ---\n";
    for (const auto& [no, oda] : odalar.tumunu_al()) {
        std::cout << "  " << oda << "\n";
    }
    std::cout << "\n";

    // -- Yerlestirme kaydi deposu olustur --
    Depo<int, YerlestirmeKaydi> yerlestirmeler;

    yerlestirmeler.ekle(1, {1, 2001, "A-101", "2025-Bahar",
        "2025-02-17", std::nullopt});
    yerlestirmeler.ekle(2, {2, 2002, "A-101", "2025-Bahar",
        "2025-02-17", std::nullopt});
    yerlestirmeler.ekle(3, {3, 2003, "B-101", "2024-Guz",
        "2024-09-16", "2025-01-31"});

    std::cout << "--- Yerlestirme Kayitlari ---\n";
    for (const auto& [id, kayit] : yerlestirmeler.tumunu_al()) {
        std::cout << "  " << kayit << "\n";
    }
    std::cout << "\n";

    // -- Odeme kaydi deposu olustur --
    Depo<int, OdemeKaydi> odemeler;

    odemeler.ekle(1, {1, 2001, 1500.0, "2025-03-01", OdemeDurum::Odendi});
    odemeler.ekle(2, {2, 2001, 1500.0, "2025-04-01", OdemeDurum::Beklemede});
    odemeler.ekle(3, {3, 2002, 1500.0, "2025-03-01", OdemeDurum::Odendi});
    odemeler.ekle(4, {4, 2002, 1500.0, "2025-04-01", OdemeDurum::Gecikmis});
    odemeler.ekle(5, {5, 2003, 900.0, "2024-10-01", OdemeDurum::Odendi});

    std::cout << "--- Odeme Kayitlari ---\n";
    for (const auto& [id, kayit] : odemeler.tumunu_al()) {
        std::cout << "  " << kayit << "\n";
    }
    std::cout << "\n";

    // === GOREV 1.3a - Bos Oda Arama (15 dk) ===
    // TODO: Belirli bir blokta (ornegin "A") kapasitesi dolu olmayan
    //       odalari bul ve listele.
    //       Ipucu: odalar.filtrele(...) ve yerlestirme verilerini
    //              birlikte kullanarak mevcut dolulugu hesapla.
    // ============================================

    // === GOREV 1.3b - Geciken Odemeler (10 dk) ===
    // TODO: Durumu "Gecikmis" olan odeme kayitlarini bul ve listele.
    // ==============================================

    // === GOREV 1.3c - Ogrenci Toplam Odeme (10 dk) ===
    // TODO: Belirli bir ogrencinin (ornegin 2001) toplam odeme
    //       tutarini hesapla.
    //       Ipucu: filtrele() ile ogrenciye ait odemeleri bul,
    //              sonra std::accumulate veya ranges ile topla.
    // ==================================================

    // === GOREV 1.3d - Doluluk Siralama (15 dk) ===
    // TODO: Odalari doluluk oranina gore sirala ve listele.
    //       Ipucu: tumunu_al() ile alip bir vector'e kopyala,
    //              sonra std::ranges::sort kullan.
    // ==============================================

    // === GOREV 1.4 - Dosya Islemleri (20 dk) ===
    // TODO: Ogrenci deposunu bir dosyaya kaydet ve geri oku.
    //       Ikili (binary) format kullanabilirsin.
    // ============================================

    std::cout << "=== Program sona erdi ===\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== YURTSIS - Ogrenci Yurt Yonetim Sistemi ===

Ogrenci sayisi: 4

--- Ogrenci Listesi ---
  [2001] Ali Yilmaz (Bilgisayar Muhendisligi, 2. sinif)
  [2002] Zeynep Kara (Elektrik Elektronik, 1. sinif)
  [2003] Mehmet Demir (Makine Muhendisligi, 3. sinif)
  [2004] Ayse Celik (Bilgisayar Muhendisligi, 1. sinif)

Oda sayisi: 5

--- Oda Listesi ---
  [A-101] Blok: A, Kapasite: 2, Tip: Cift, Ucret: 1500 TL
  [A-102] Blok: A, Kapasite: 2, Tip: Cift, Ucret: 1500 TL
  [A-201] Blok: A, Kapasite: 1, Tip: Tek, Ucret: 2500 TL
  [B-101] Blok: B, Kapasite: 4, Tip: Dort, Ucret: 900 TL
  [B-102] Blok: B, Kapasite: 3, Tip: Uc, Ucret: 1100 TL

--- Yerlestirme Kayitlari ---
  [Kayit #1] Ogrenci: 2001, Oda: A-101, Donem: 2025-Bahar, Baslangic: 2025-02-17, Bitis: ---
  [Kayit #2] Ogrenci: 2002, Oda: A-101, Donem: 2025-Bahar, Baslangic: 2025-02-17, Bitis: ---
  [Kayit #3] Ogrenci: 2003, Oda: B-101, Donem: 2024-Guz, Baslangic: 2024-09-16, Bitis: 2025-01-31

--- Odeme Kayitlari ---
  [Odeme #1] Ogrenci: 2001, Tutar: 1500 TL, Tarih: 2025-03-01, Durum: Odendi
  [Odeme #2] Ogrenci: 2001, Tutar: 1500 TL, Tarih: 2025-04-01, Durum: Beklemede
  [Odeme #3] Ogrenci: 2002, Tutar: 1500 TL, Tarih: 2025-03-01, Durum: Odendi
  [Odeme #4] Ogrenci: 2002, Tutar: 1500 TL, Tarih: 2025-04-01, Durum: Gecikmis
  [Odeme #5] Ogrenci: 2003, Tutar: 900 TL, Tarih: 2024-10-01, Durum: Odendi

=== Program sona erdi ===
*/

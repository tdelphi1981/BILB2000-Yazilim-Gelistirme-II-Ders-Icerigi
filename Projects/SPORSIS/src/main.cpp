/**
 * main.cpp -- SPORSIS Başlangıç Noktası
 *
 * Spor Salonu Üyelik Sistemi - Faz 1 başlangıç kodu.
 * Bu dosya derlenip çalıştırılabilir durumdadır.
 * GOREV işaretli yerleri tamamlayarak sistemi geliştirin.
 *
 * Derleme: g++ -std=c++20 -o sporsis src/main.cpp
 * Çalıştırma: ./sporsis
 */

#include "../include/varliklar.h"
#include "../include/depo.h"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>

int main() {
    std::cout << "=== SPORSIS - Spor Salonu Uyelik Sistemi ===\n\n";

    // -- Uye deposu olustur ve ornek veriler ekle --
    Depo<int, Uye> uyeler;

    uyeler.ekle(2001, {2001, "Ali", "Yilmaz", "555-0201", "2025-09-01"});
    uyeler.ekle(2002, {2002, "Zeynep", "Kara", "555-0202", "2025-09-15"});
    uyeler.ekle(2003, {2003, "Mehmet", "Demir", "555-0203", "2025-10-01"});
    uyeler.ekle(2004, {2004, "Elif", "Celik", "555-0204", "2025-10-10"});

    std::cout << "Uye sayisi: " << uyeler.boyut() << "\n\n";

    // Tum uyeleri listele
    std::cout << "--- Uye Listesi ---\n";
    for (const auto& [no, uye] : uyeler.tumunu_al()) {
        std::cout << "  " << uye << "\n";
    }
    std::cout << "\n";

    // -- Uyelik paketi deposu olustur --
    Depo<int, UyelikPaketi> paketler;

    paketler.ekle(1, {1, "Aylik", 30, 1500.0});
    paketler.ekle(2, {2, "3 Aylik", 90, 3750.0});
    paketler.ekle(3, {3, "Yillik", 365, 12000.0});

    std::cout << "Paket sayisi: " << paketler.boyut() << "\n\n";

    std::cout << "--- Uyelik Paketleri ---\n";
    for (const auto& [id, paket] : paketler.tumunu_al()) {
        std::cout << "  " << paket << "\n";
    }
    std::cout << "\n";

    // -- Uyelik kaydi deposu olustur --
    Depo<int, UyelikKaydi> uyelikler;

    uyelikler.ekle(1, {1, 2001, 3, "2025-09-01", std::nullopt,
        UyelikDurum::Aktif});
    uyelikler.ekle(2, {2, 2002, 1, "2025-09-15", "2025-10-15",
        UyelikDurum::SuresiDolmus});
    uyelikler.ekle(3, {3, 2003, 2, "2025-10-01", "2025-12-30",
        UyelikDurum::Aktif});
    uyelikler.ekle(4, {4, 2004, 1, "2025-10-10", "2025-11-10",
        UyelikDurum::Dondurulmus});

    std::cout << "--- Uyelik Kayitlari ---\n";
    for (const auto& [id, kayit] : uyelikler.tumunu_al()) {
        std::cout << "  " << kayit << "\n";
    }
    std::cout << "\n";

    // -- Antrenman kaydi deposu olustur --
    Depo<int, AntrenmanKaydi> antrenmanlar;

    antrenmanlar.ekle(1, {1, 2001, "2025-11-01", 60, "Kardiyo"});
    antrenmanlar.ekle(2, {2, 2001, "2025-11-02", 45, "Agirlik"});
    antrenmanlar.ekle(3, {3, 2003, "2025-11-01", 90, "Yoga"});
    antrenmanlar.ekle(4, {4, 2003, "2025-11-03", 50, "Kardiyo"});
    antrenmanlar.ekle(5, {5, 2001, "2025-11-04", 75, "Pilates"});

    std::cout << "--- Antrenman Kayitlari ---\n";
    for (const auto& [id, kayit] : antrenmanlar.tumunu_al()) {
        std::cout << "  " << kayit << "\n";
    }
    std::cout << "\n";

    // === GOREV 1.3a - Aktif Uyelik Filtreleme (10 dk) ===
    // TODO: Durumu "Aktif" olan uyelik kayitlarini filtrele ve listele.
    //       Ipucu: uyelikler.filtrele(...) metodunu kullan.
    // ====================================================

    // === GOREV 1.3b - Antrenman Istatistigi (10 dk) ===
    // TODO: Belirli bir uyenin (ornegin 2001) toplam antrenman
    //       suresini (dakika) hesapla.
    // ==================================================

    // === GOREV 1.3c - Siralama (10 dk) ===
    // TODO: Antrenman kayitlarini sureye gore azalan sirada
    //       sirala ve listele.
    //       Ipucu: tumunu_al() ile alip bir vector'e kopyala,
    //              sonra std::ranges::sort kullan.
    // =====================================

    // === GOREV 1.3d - unordered_map ile Hizli Erisim (15 dk) ===
    // TODO: Uye kart numarasindan hizli erisim icin
    //       std::unordered_map<int, Uye> olustur ve kullan.
    // ===========================================================

    // === GOREV 1.3e - deque ile Antrenman Gecmisi (10 dk) ===
    // TODO: Bir uye icin std::deque<AntrenmanKaydi> olustur.
    //       Basa ve sona yeni kayit ekleyip cikar.
    // ========================================================

    // === GOREV 1.3f - set ile Saat Dilimi Kontrolu (15 dk) ===
    // TODO: std::set<std::string> ile saat dilimi slotlarini yonet.
    //       Yeni bir ders eklerken cakisma kontrolu yap.
    // =========================================================

    // === GOREV 1.4 - Dosya Islemleri (20 dk) ===
    // TODO: Uye deposunu bir dosyaya kaydet ve geri oku.
    //       Ikili (binary) format kullanabilirsin.
    // ============================================

    std::cout << "=== Program sona erdi ===\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== SPORSIS - Spor Salonu Uyelik Sistemi ===

Uye sayisi: 4

--- Uye Listesi ---
  [2001] Ali Yilmaz (Tel: 555-0201, Kayit: 2025-09-01)
  [2002] Zeynep Kara (Tel: 555-0202, Kayit: 2025-09-15)
  [2003] Mehmet Demir (Tel: 555-0203, Kayit: 2025-10-01)
  [2004] Elif Celik (Tel: 555-0204, Kayit: 2025-10-10)

Paket sayisi: 3

--- Uyelik Paketleri ---
  [Paket #1] Aylik (30 gun, 1500 TL)
  [Paket #2] 3 Aylik (90 gun, 3750 TL)
  [Paket #3] Yillik (365 gun, 12000 TL)

--- Uyelik Kayitlari ---
  [Kayit #1] Uye: 2001, Paket: 3, Baslangic: 2025-09-01, Bitis: ---, Durum: Aktif
  [Kayit #2] Uye: 2002, Paket: 1, Baslangic: 2025-09-15, Bitis: 2025-10-15, Durum: Suresi Dolmus
  [Kayit #3] Uye: 2003, Paket: 2, Baslangic: 2025-10-01, Bitis: 2025-12-30, Durum: Aktif
  [Kayit #4] Uye: 2004, Paket: 1, Baslangic: 2025-10-10, Bitis: 2025-11-10, Durum: Dondurulmus

--- Antrenman Kayitlari ---
  [Antrenman #1] Uye: 2001, Tarih: 2025-11-01, Sure: 60 dk, Aktivite: Kardiyo
  [Antrenman #2] Uye: 2001, Tarih: 2025-11-02, Sure: 45 dk, Aktivite: Agirlik
  [Antrenman #3] Uye: 2003, Tarih: 2025-11-01, Sure: 90 dk, Aktivite: Yoga
  [Antrenman #4] Uye: 2003, Tarih: 2025-11-03, Sure: 50 dk, Aktivite: Kardiyo
  [Antrenman #5] Uye: 2001, Tarih: 2025-11-04, Sure: 75 dk, Aktivite: Pilates

=== Program sona erdi ===
*/

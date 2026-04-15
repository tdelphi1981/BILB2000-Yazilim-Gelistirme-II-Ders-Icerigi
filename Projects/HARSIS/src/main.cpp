/**
 * main.cpp -- HARSIS Başlangıç Noktası
 *
 * Hastane Randevu Sistemi - Faz 1 başlangıç kodu.
 * Bu dosya derlenip çalıştırılabilir durumdadır.
 * GOREV işaretli yerleri tamamlayarak sistemi geliştirin.
 *
 * Derleme: g++ -std=c++20 -o harsis src/main.cpp
 * Çalıştırma: ./harsis
 */

#include "../include/varliklar.h"
#include "../include/depo.h"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>

int main() {
    std::cout << "=== HARSIS - Hastane Randevu Sistemi ===\n\n";

    // -- Hasta deposu olustur ve ornek veriler ekle --
    Depo<std::string, Hasta> hastalar;

    hastalar.ekle("12345678901", {
        "12345678901", "Ali", "Yilmaz",
        "1985-03-15", "555-0101"
    });
    hastalar.ekle("23456789012", {
        "23456789012", "Zeynep", "Kara",
        "1990-07-22", "555-0102"
    });
    hastalar.ekle("34567890123", {
        "34567890123", "Mehmet", "Demir",
        "1978-11-05", "555-0103"
    });

    std::cout << "Hasta sayisi: " << hastalar.boyut() << "\n\n";

    // Tum hastalari listele
    std::cout << "--- Hasta Listesi ---\n";
    for (const auto& [tc, hasta] : hastalar.tumunu_al()) {
        std::cout << "  " << hasta << "\n";
    }
    std::cout << "\n";

    // -- Doktor deposu olustur --
    Depo<std::string, Doktor> doktorlar;

    doktorlar.ekle("D001", {
        "D001", "Ayse", "Ozturk", "Dahiliye",
        {"09:00", "09:30", "10:00", "10:30", "11:00"}
    });
    doktorlar.ekle("D002", {
        "D002", "Hasan", "Celik", "Ortopedi",
        {"13:00", "13:30", "14:00", "14:30"}
    });
    doktorlar.ekle("D003", {
        "D003", "Fatma", "Sahin", "Dahiliye",
        {"09:00", "09:30", "10:00", "14:00", "14:30"}
    });

    std::cout << "Doktor sayisi: " << doktorlar.boyut() << "\n\n";

    std::cout << "--- Doktor Listesi ---\n";
    for (const auto& [sicil, doktor] : doktorlar.tumunu_al()) {
        std::cout << "  " << doktor << "\n";
    }
    std::cout << "\n";

    // -- Randevu deposu olustur --
    Depo<int, Randevu> randevular;

    randevular.ekle(1, {1, "12345678901", "D001",
        "2025-04-15", "09:00", RandevuDurum::Onaylandi,
        "Kontrol muayenesi"});
    randevular.ekle(2, {2, "23456789012", "D002",
        "2025-04-15", "13:30", RandevuDurum::Beklemede,
        std::nullopt});
    randevular.ekle(3, {3, "12345678901", "D003",
        "2025-04-16", "10:00", RandevuDurum::Tamamlandi,
        "Kan tahlili sonuclari normal"});
    randevular.ekle(4, {4, "34567890123", "D001",
        "2025-04-15", "10:30", RandevuDurum::Iptal,
        "Hasta tarafindan iptal edildi"});

    std::cout << "--- Randevu Listesi ---\n";
    for (const auto& [id, randevu] : randevular.tumunu_al()) {
        std::cout << "  " << randevu << "\n";
    }
    std::cout << "\n";

    // === GOREV 1.3a - Brans Filtreleme (10 dk) ===
    // TODO: "Dahiliye" bransindaki doktorlari filtrele ve listele.
    //       Ipucu: doktorlar.filtrele(...) metodunu kullan.
    // =============================================

    // === GOREV 1.3b - Cakisma Kontrolu (15 dk) ===
    // TODO: Belirli bir doktorun belirli bir tarih ve saatte
    //       baska bir randevusu olup olmadigini kontrol et.
    //       Ipucu: randevular.filtrele(...) ile doktor ve
    //              tarih/saat eslesenleri bul.
    // =============================================

    // === GOREV 1.3c - Tarih Bazli Siralama (10 dk) ===
    // TODO: Belirli bir tarihteki tum randevulari saate gore sirala.
    //       Ipucu: tumunu_al() ile alip bir vector'e kopyala,
    //              sonra std::ranges::sort kullan.
    // =================================================

    // === GOREV 1.3d - Acil Hasta Onceliklendirme (15 dk) ===
    // TODO: std::priority_queue kullanarak acil hastalari
    //       oncelik sirasina gore isle.
    // =======================================================

    // === GOREV 1.4 - Dosya Islemleri (20 dk) ===
    // TODO: Hasta ve randevu depolarini bir dosyaya kaydet ve geri oku.
    //       Ikili (binary) format kullanabilirsin.
    // ============================================

    std::cout << "=== Program sona erdi ===\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== HARSIS - Hastane Randevu Sistemi ===

Hasta sayisi: 3

--- Hasta Listesi ---
  [TC: 12345678901] Ali Yilmaz (Dogum: 1985-03-15, Tel: 555-0101)
  [TC: 23456789012] Zeynep Kara (Dogum: 1990-07-22, Tel: 555-0102)
  [TC: 34567890123] Mehmet Demir (Dogum: 1978-11-05, Tel: 555-0103)

Doktor sayisi: 3

--- Doktor Listesi ---
  [Sicil: D001] Dr. Ayse Ozturk (Brans: Dahiliye, Uygun saatler: 09:00 09:30 10:00 10:30 11:00)
  [Sicil: D002] Dr. Hasan Celik (Brans: Ortopedi, Uygun saatler: 13:00 13:30 14:00 14:30)
  [Sicil: D003] Dr. Fatma Sahin (Brans: Dahiliye, Uygun saatler: 09:00 09:30 10:00 14:00 14:30)

--- Randevu Listesi ---
  [Randevu #1] Hasta TC: 12345678901, Doktor: D001, Tarih: 2025-04-15 09:00, Durum: Onaylandi, Not: Kontrol muayenesi
  [Randevu #2] Hasta TC: 23456789012, Doktor: D002, Tarih: 2025-04-15 13:30, Durum: Beklemede, Not: ---
  [Randevu #3] Hasta TC: 12345678901, Doktor: D003, Tarih: 2025-04-16 10:00, Durum: Tamamlandi, Not: Kan tahlili sonuclari normal
  [Randevu #4] Hasta TC: 34567890123, Doktor: D001, Tarih: 2025-04-15 10:30, Durum: Iptal, Not: Hasta tarafindan iptal edildi

=== Program sona erdi ===
*/

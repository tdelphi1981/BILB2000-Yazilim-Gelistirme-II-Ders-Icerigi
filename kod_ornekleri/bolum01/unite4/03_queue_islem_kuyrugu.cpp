/**
 * queue ile Islem Kuyrugu
 *
 * Bu program, std::queue kapsayicisini kullanarak OBYS'de
 * ders kayit isteklerinin sirayla islenmesini simule eder.
 *
 * Ogrenilecek kavramlar:
 * - std::queue tanimlama ve kullanimi
 * - FIFO (Ilk Giren Ilk Cikar) mantigi
 * - push(), front(), back(), pop(), empty(), size() metodlari
 * - Struct ile queue kullanimi
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 4 - Sirasiz Kapsayicilar ve Uyarlayicilar
 *
 * Derleme: g++ -std=c++20 -o 03_queue_islem_kuyrugu 03_queue_islem_kuyrugu.cpp
 * Calistirma: ./03_queue_islem_kuyrugu
 */

#include <iostream>
#include <queue>
#include <string>

struct KayitIstegi {
    int ogrenci_no;
    std::string ders_kodu;
    std::string ogrenci_adi;
};

int main() {
    std::cout << "=== OBYS Ders Kayit Kuyrugu ===" << '\n' << '\n';

    // Ders kayit istek kuyrugu
    std::queue<KayitIstegi> kayit_kuyrugu;

    // Istekleri kuyruga ekle (farkli ogrencilerden gelen istekler)
    kayit_kuyrugu.push({1001, "MAT101", "Ali Yilmaz"});
    kayit_kuyrugu.push({1002, "FIZ101", "Ayse Kaya"});
    kayit_kuyrugu.push({1003, "MAT101", "Mehmet Demir"});
    kayit_kuyrugu.push({1001, "FIZ101", "Ali Yilmaz"});
    kayit_kuyrugu.push({1004, "KIM101", "Zeynep Celik"});

    std::cout << "Kuyrukta " << kayit_kuyrugu.size()
              << " istek bekliyor." << '\n';
    std::cout << "Ilk istek: " << kayit_kuyrugu.front().ogrenci_adi
              << " -> " << kayit_kuyrugu.front().ders_kodu << '\n';
    std::cout << "Son istek: " << kayit_kuyrugu.back().ogrenci_adi
              << " -> " << kayit_kuyrugu.back().ders_kodu << '\n';

    // Istekleri sirayla isle
    std::cout << '\n' << "--- Istekler Isleniyor ---" << '\n';
    int sira = 1;
    while (!kayit_kuyrugu.empty()) {
        const auto& istek = kayit_kuyrugu.front();

        std::cout << sira++ << ". "
                  << istek.ogrenci_adi << " (No: " << istek.ogrenci_no
                  << ") -> " << istek.ders_kodu;

        // Basit kayit simulasyonu
        if (istek.ders_kodu == "MAT101" || istek.ders_kodu == "FIZ101"
            || istek.ders_kodu == "KIM101") {
            std::cout << " [BASARILI]" << '\n';
        } else {
            std::cout << " [BASARISIZ: Ders bulunamadi]" << '\n';
        }

        kayit_kuyrugu.pop();
    }

    std::cout << '\n' << "Tum istekler islendi. Kalan: "
              << kayit_kuyrugu.size() << '\n';

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === OBYS Ders Kayit Kuyrugu ===
 *
 * Kuyrukta 5 istek bekliyor.
 * Ilk istek: Ali Yilmaz -> MAT101
 * Son istek: Zeynep Celik -> KIM101
 *
 * --- Istekler Isleniyor ---
 * 1. Ali Yilmaz (No: 1001) -> MAT101 [BASARILI]
 * 2. Ayse Kaya (No: 1002) -> FIZ101 [BASARILI]
 * 3. Mehmet Demir (No: 1003) -> MAT101 [BASARILI]
 * 4. Ali Yilmaz (No: 1001) -> FIZ101 [BASARILI]
 * 5. Zeynep Celik (No: 1004) -> KIM101 [BASARILI]
 *
 * Tum istekler islendi. Kalan: 0
 */

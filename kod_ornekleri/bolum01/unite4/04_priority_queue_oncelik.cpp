/**
 * priority_queue ile Oncelikli Islem Yonetimi
 *
 * Bu program, std::priority_queue kapsayicisini kullanarak
 * OBYS'de farkli onceliklere sahip islemlerin siralamasini
 * gosterir. Yuksek oncelikli islemler once islenir.
 *
 * Ogrenilecek kavramlar:
 * - std::priority_queue tanimlama ve kullanimi
 * - Max-heap ve min-heap farki
 * - Ozel tur icin operator< tanimlama
 * - std::greater ile min-heap olusturma
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 4 - Sirasiz Kapsayicilar ve Uyarlayicilar
 *
 * Derleme: g++ -std=c++20 -o 04_priority_queue_oncelik 04_priority_queue_oncelik.cpp
 * Calistirma: ./04_priority_queue_oncelik
 */

#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <functional>  // std::greater

// OBYS islem yapisi
struct OBYSIslem {
    int oncelik;            // 1=dusuk, 5=yuksek
    std::string aciklama;
    std::string talep_eden;

    // Max-heap icin: buyuk oncelik tepede
    bool operator<(const OBYSIslem& other) const {
        return oncelik < other.oncelik;
    }
};

int main() {
    std::cout << "=== OBYS Oncelikli Islem Kuyrugu ===" << '\n' << '\n';

    // --- Bolum 1: Ozel tur ile priority_queue ---
    std::cout << "--- Max-Heap (Yuksek Oncelik Once) ---" << '\n';

    std::priority_queue<OBYSIslem> islem_kuyrugu;

    // Farkli onceliklerle islem ekle
    islem_kuyrugu.push({1, "Adres degisikligi",            "Ogrenci 1001"});
    islem_kuyrugu.push({5, "Not itirazi",                  "Ogrenci 1002"});
    islem_kuyrugu.push({3, "Ders kayit islemi",            "Ogrenci 1003"});
    islem_kuyrugu.push({5, "Burs basvurusu degerlendirme", "Ogrenci 1004"});
    islem_kuyrugu.push({2, "Transkript talebi",            "Ogrenci 1005"});
    islem_kuyrugu.push({4, "Staj basvurusu",               "Ogrenci 1001"});

    std::cout << "Kuyrukta " << islem_kuyrugu.size()
              << " islem bekliyor." << '\n' << '\n';

    int sira = 1;
    while (!islem_kuyrugu.empty()) {
        const auto& islem = islem_kuyrugu.top();
        std::cout << sira++ << ". [Oncelik " << islem.oncelik << "] "
                  << islem.aciklama
                  << " (" << islem.talep_eden << ")" << '\n';
        islem_kuyrugu.pop();
    }

    // --- Bolum 2: Min-heap ornegi (kucuk deger once) ---
    std::cout << '\n' << "--- Min-Heap (Kucuk Deger Once) ---" << '\n';

    // Not ortalamasi en dusuk ogrenciye oncelik ver (burs icin)
    std::priority_queue<double, std::vector<double>, std::greater<double>> not_ortalamalari;

    not_ortalamalari.push(3.85);
    not_ortalamalari.push(2.10);
    not_ortalamalari.push(3.45);
    not_ortalamalari.push(1.75);
    not_ortalamalari.push(2.90);

    std::cout << "Not ortalamalari (dusukten yuksege):" << '\n';
    while (!not_ortalamalari.empty()) {
        std::cout << "  " << not_ortalamalari.top() << '\n';
        not_ortalamalari.pop();
    }

    // --- Bolum 3: Temel int max-heap ---
    std::cout << '\n' << "--- Temel int Max-Heap ---" << '\n';

    std::priority_queue<int> sayilar;
    for (int s : {30, 10, 50, 20, 40}) {
        sayilar.push(s);
    }

    std::cout << "Siralama (buyukten kucuge):" << '\n';
    while (!sayilar.empty()) {
        std::cout << "  " << sayilar.top() << '\n';
        sayilar.pop();
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === OBYS Oncelikli Islem Kuyrugu ===
 *
 * --- Max-Heap (Yuksek Oncelik Once) ---
 * Kuyrukta 6 islem bekliyor.
 *
 * 1. [Oncelik 5] Not itirazi (Ogrenci 1002)
 * 2. [Oncelik 5] Burs basvurusu degerlendirme (Ogrenci 1004)
 * 3. [Oncelik 4] Staj basvurusu (Ogrenci 1001)
 * 4. [Oncelik 3] Ders kayit islemi (Ogrenci 1003)
 * 5. [Oncelik 2] Transkript talebi (Ogrenci 1005)
 * 6. [Oncelik 1] Adres degisikligi (Ogrenci 1001)
 *
 * --- Min-Heap (Kucuk Deger Once) ---
 * Not ortalamalari (dusukten yuksege):
 *   1.75
 *   2.1
 *   2.9
 *   3.45
 *   3.85
 *
 * --- Temel int Max-Heap ---
 * Siralama (buyukten kucuge):
 *   50
 *   40
 *   30
 *   20
 *   10
 */

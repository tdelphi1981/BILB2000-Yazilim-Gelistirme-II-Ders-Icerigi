/**
 * Thread Olusturma - Uc Farkli Yontem
 *
 * Bu program std::thread ile izlek olusturmanin uc yolunu gosterir:
 * fonksiyon isaretcisi, lambda ifadesi ve functor (cagrilabilir nesne).
 *
 * Ogrenilecek kavramlar:
 * - std::thread temel kullanimi
 * - Fonksiyon isaretcisi ile izlek baslatma
 * - Lambda ile izlek baslatma
 * - Functor ile izlek baslatma
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 2 - std::thread Temelleri
 *
 * Derleme: g++ -std=c++20 -pthread -o 01_thread_olusturma 01_thread_olusturma.cpp
 * Calistirma: ./01_thread_olusturma
 */

#include <iostream>
#include <thread>
#include <string>

// --- Yontem 1: Fonksiyon isaretcisi ---
void merhaba_de() {
    std::cout << "[Fonksiyon] Merhaba, ben ayri bir izlekten yaziyorum!\n";
}

void ogrenci_bilgisi(const std::string& isim, int numara) {
    std::cout << "[Fonksiyon] Ogrenci: " << isim
              << " (No: " << numara << ")\n";
}

// --- Yontem 3: Functor ---
struct ArkaplanGorev {
    int tekrar_sayisi;

    void operator()() const {
        for (int i = 0; i < tekrar_sayisi; ++i) {
            std::cout << "[Functor] Gorev adimi: " << (i + 1) << '\n';
        }
    }
};

int main() {
    std::cout << "=== std::thread Olusturma Ornekleri ===\n\n";

    // Yontem 1: Fonksiyon isaretcisi
    std::cout << "--- Yontem 1: Fonksiyon Isaretcisi ---\n";
    std::thread t1(merhaba_de);
    t1.join();

    // Parametreli fonksiyon
    std::thread t2(ogrenci_bilgisi, "Ali Yilmaz", 101);
    t2.join();

    // Yontem 2: Lambda ifadesi
    std::cout << "\n--- Yontem 2: Lambda Ifadesi ---\n";
    std::string ders = "Yazilim Gelistirme II";
    int kontenjan = 60;

    std::thread t3([&ders, kontenjan]() {
        std::cout << "[Lambda] " << ders
                  << " kontenjani: " << kontenjan << '\n';
    });
    t3.join();

    // Yontem 3: Functor
    std::cout << "\n--- Yontem 3: Functor ---\n";
    ArkaplanGorev gorev{3};
    std::thread t4(gorev);
    t4.join();

    std::cout << "\n[Ana Izlek] Tum izlekler tamamlandi.\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === std::thread Olusturma Ornekleri ===
 *
 * --- Yontem 1: Fonksiyon Isaretcisi ---
 * [Fonksiyon] Merhaba, ben ayri bir izlekten yaziyorum!
 * [Fonksiyon] Ogrenci: Ali Yilmaz (No: 101)
 *
 * --- Yontem 2: Lambda Ifadesi ---
 * [Lambda] Yazilim Gelistirme II kontenjani: 60
 *
 * --- Yontem 3: Functor ---
 * [Functor] Gorev adimi: 1
 * [Functor] Gorev adimi: 2
 * [Functor] Gorev adimi: 3
 *
 * [Ana Izlek] Tum izlekler tamamlandi.
 */

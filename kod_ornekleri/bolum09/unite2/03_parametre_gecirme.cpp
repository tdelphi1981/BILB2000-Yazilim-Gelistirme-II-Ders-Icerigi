/**
 * Thread'lere Parametre Gecirme
 *
 * Bu program izleklere deger, referans (std::ref/std::cref) ve
 * tasima (std::move) ile parametre gecirmenin uc yolunu gosterir.
 * OBYS baglami: ogrenci listesiyle GPA hesaplama.
 *
 * Ogrenilecek kavramlar:
 * - Deger ile parametre gecirme (varsayilan)
 * - std::ref ile referans gecirme
 * - std::cref ile sabit referans gecirme
 * - std::move ile tasima semantigi
 * - unique_ptr ile izlek kullanimi
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 2 - std::thread Temelleri
 *
 * Derleme: g++ -std=c++20 -pthread -o 03_parametre_gecirme 03_parametre_gecirme.cpp
 * Calistirma: ./03_parametre_gecirme
 */

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <memory>
#include <numeric>

// --- OBYS Ogrenci yapisi ---
struct Ogrenci {
    std::string isim;
    double not_ortalamasi;
};

// --- Deger ile gecirme ---
void selamla(std::string isim, int numara) {
    std::cout << "[Deger] Merhaba " << isim
              << " (No: " << numara << ")\n";
}

// --- Referans ile gecirme: sonuc yazma ---
void hesapla_toplam(const std::vector<int>& notlar, int& toplam) {
    toplam = 0;
    for (int n : notlar) {
        toplam += n;
    }
    std::cout << "[Ref] Toplam hesaplandi: " << toplam << '\n';
}

// --- const referans ile gecirme: OBYS GPA ---
void gpa_hesapla(const std::vector<Ogrenci>& liste, double& ortalama) {
    double toplam = 0.0;
    for (const auto& o : liste) {
        toplam += o.not_ortalamasi;
    }
    ortalama = liste.empty() ? 0.0 : toplam / liste.size();
    std::cout << "[CRef] GPA hesaplandi: " << ortalama << '\n';
}

// --- Tasima ile gecirme ---
void isle_unique(std::unique_ptr<int> veri) {
    std::cout << "[Move] unique_ptr degeri: " << *veri << '\n';
}

void isle_vektor(std::vector<int> veriler) {
    std::cout << "[Move] Vektor boyutu: " << veriler.size() << '\n';
}

int main() {
    std::cout << "=== Parametre Gecirme Ornekleri ===\n\n";

    // 1. Deger ile gecirme
    std::cout << "--- 1. Deger ile Gecirme ---\n";
    std::string ad = "Ayse Kaya";
    int no = 102;
    std::thread t1(selamla, ad, no);
    t1.join();
    std::cout << "Orijinal ad hala gecerli: " << ad << "\n\n";

    // 2. Referans ile gecirme (std::ref)
    std::cout << "--- 2. Referans ile Gecirme ---\n";
    std::vector<int> notlar = {85, 92, 78, 95, 88};
    int toplam = 0;
    std::thread t2(hesapla_toplam, std::cref(notlar), std::ref(toplam));
    t2.join();
    std::cout << "Ana izlekte toplam: " << toplam << "\n\n";

    // 3. const referans ile gecirme (std::cref) - OBYS ornegi
    std::cout << "--- 3. OBYS GPA Hesaplama ---\n";
    std::vector<Ogrenci> ogrenciler = {
        {"Ali",   3.2},
        {"Ayse",  3.8},
        {"Veli",  2.9},
        {"Zehra", 3.5}
    };
    double gpa_sonuc = 0.0;
    std::thread t3(gpa_hesapla, std::cref(ogrenciler), std::ref(gpa_sonuc));
    t3.join();
    std::cout << "Sinif ortalamasi: " << gpa_sonuc << "\n\n";

    // 4. Tasima ile gecirme (std::move)
    std::cout << "--- 4. Tasima ile Gecirme ---\n";

    // unique_ptr tasima (kopyalanamaz)
    auto ptr = std::make_unique<int>(42);
    std::thread t4(isle_unique, std::move(ptr));
    t4.join();
    std::cout << "ptr artik nullptr: " << std::boolalpha
              << (ptr == nullptr) << '\n';

    // Buyuk vektor tasima (performans icin)
    std::vector<int> buyuk_veri(10000, 85);
    std::cout << "Tasima oncesi boyut: " << buyuk_veri.size() << '\n';
    std::thread t5(isle_vektor, std::move(buyuk_veri));
    t5.join();
    std::cout << "Tasima sonrasi boyut: " << buyuk_veri.size() << '\n';

    // 5. Thread tasima
    std::cout << "\n--- 5. Thread Nesnesi Tasima ---\n";
    auto olustur = []() -> std::thread {
        return std::thread([]() {
            std::cout << "[Fabrika] Fabrikadan gelen izlek\n";
        });
    };
    std::thread t6 = olustur();
    t6.join();

    std::cout << "\n[Ana] Tum ornekler tamamlandi.\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Parametre Gecirme Ornekleri ===
 *
 * --- 1. Deger ile Gecirme ---
 * [Deger] Merhaba Ayse Kaya (No: 102)
 * Orijinal ad hala gecerli: Ayse Kaya
 *
 * --- 2. Referans ile Gecirme ---
 * [Ref] Toplam hesaplandi: 438
 * Ana izlekte toplam: 438
 *
 * --- 3. OBYS GPA Hesaplama ---
 * [CRef] GPA hesaplandi: 3.35
 * Sinif ortalamasi: 3.35
 *
 * --- 4. Tasima ile Gecirme ---
 * [Move] unique_ptr degeri: 42
 * ptr artik nullptr: true
 * Tasima oncesi boyut: 10000
 * [Move] Vektor boyutu: 10000
 * Tasima sonrasi boyut: 0
 *
 * --- 5. Thread Nesnesi Tasima ---
 * [Fabrika] Fabrikadan gelen izlek
 *
 * [Ana] Tum ornekler tamamlandi.
 */

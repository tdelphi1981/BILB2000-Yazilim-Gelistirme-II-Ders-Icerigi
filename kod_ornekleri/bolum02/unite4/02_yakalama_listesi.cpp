/**
 * Yakalama Listesi - Lambda Capture Modlari
 *
 * Bu program lambda ifadelerinin yakalama mekanizmasini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - [=] deger ile, [&] referans ile yakalama
 * - Karisik yakalama: [=, &x], [&, x]
 * - Init capture (C++14): [x = expr]
 * - this ve *this yakalama
 * - Dangling reference tehlikesi
 *
 * Bolum: 02 - Iteratorler ve STL Algoritmalari
 * Unite: 4 - Lambda Ifadeleri
 *
 * Derleme: g++ -std=c++20 -o 02_yakalama_listesi 02_yakalama_listesi.cpp
 * Calistirma: ./02_yakalama_listesi
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
#include <memory>
#include <numeric>
#include <iomanip>

int main() {
    std::cout << "=== Yakalama Listesi ===\n\n";

    // --- 1. Deger ile yakalama [x] ---
    std::cout << "--- Deger ile Yakalama [x] ---\n";
    int esik = 70;
    auto kontrol = [esik](int not_degeri) {
        return not_degeri >= esik;
    };

    std::cout << "Esik: " << esik << "\n";
    std::cout << "80 basarili mi? " << (kontrol(80) ? "Evet" : "Hayir") << "\n";
    std::cout << "60 basarili mi? " << (kontrol(60) ? "Evet" : "Hayir") << "\n";

    // Deger kopyalanir, sonraki degisiklikler lambda'yi etkilemez
    esik = 90;
    std::cout << "Esik 90'a degisti, 80 basarili mi? "
              << (kontrol(80) ? "Evet" : "Hayir")
              << " (lambda hala 70 kullanir)\n";

    // --- 2. Referans ile yakalama [&x] ---
    std::cout << "\n--- Referans ile Yakalama [&x] ---\n";
    int toplam = 0;
    std::vector<int> notlar = {85, 92, 78, 95, 88};

    std::for_each(notlar.begin(), notlar.end(),
        [&toplam](int n) { toplam += n; });

    std::cout << "Toplam: " << toplam << "\n";
    std::cout << "Ortalama: " << std::fixed << std::setprecision(1)
              << static_cast<double>(toplam) / notlar.size() << "\n";

    // --- 3. Tum degiskenleri yakalama [=] ve [&] ---
    std::cout << "\n--- Toplu Yakalama ---\n";
    int min_not = 60;
    int max_not = 100;
    std::string sinif_adi = "Bilgisayar";

    // [=] : Tum degiskenleri deger ile yakala
    auto rapor_deger = [=]() {
        std::cout << sinif_adi << ": " << min_not << "-" << max_not << "\n";
    };
    rapor_deger();

    // [&] : Tum degiskenleri referans ile yakala
    int sayac = 0;
    auto say = [&]() {
        for (int n : notlar) {
            if (n >= min_not && n <= max_not) {
                ++sayac;
            }
        }
    };
    say();
    std::cout << "Aralikta olan: " << sayac << " not\n";

    // --- 4. Karisik yakalama ---
    std::cout << "\n--- Karisik Yakalama ---\n";
    int basarili_sayisi = 0;
    double gpa_esik = 3.0;

    // gpa_esik deger ile, basarili_sayisi referans ile
    auto filtre = [gpa_esik, &basarili_sayisi](double gpa) {
        if (gpa >= gpa_esik) {
            ++basarili_sayisi;
            return true;
        }
        return false;
    };

    std::vector<double> gpalar = {3.45, 3.80, 2.90, 3.65, 2.10};
    for (double g : gpalar) filtre(g);
    std::cout << "GPA >= " << gpa_esik << ": " << basarili_sayisi << " ogrenci\n";

    // --- 5. Init capture (C++14) ---
    std::cout << "\n--- Init Capture ---\n";

    // Yeni degisken olustur
    auto sayac_lambda = [n = 0]() mutable { return ++n; };
    std::cout << "Sayac: " << sayac_lambda() << "\n";  // 1
    std::cout << "Sayac: " << sayac_lambda() << "\n";  // 2

    // Move capture (unique_ptr icin onemli)
    auto ptr = std::make_unique<std::string>("OBYS Sistemi");
    std::cout << "Tasimadan once: " << (ptr ? *ptr : "bos") << "\n";

    auto lambda_ptr = [p = std::move(ptr)]() {
        std::cout << "Lambda icinde: " << *p << "\n";
    };

    std::cout << "Tasimadan sonra: " << (ptr ? *ptr : "bos (tasinmis)") << "\n";
    lambda_ptr();

    // Hesaplama ile init capture
    std::vector<int> veri = {10, 20, 30, 40, 50};
    auto ort_lambda = [ort = std::accumulate(veri.begin(), veri.end(), 0.0) / veri.size()]() {
        std::cout << "Onceden hesaplanan ortalama: " << ort << "\n";
    };
    ort_lambda();

    // --- 6. this yakalama ---
    std::cout << "\n--- this Yakalama ---\n";

    struct OgrenciSinifi {
        std::string sinif_adi = "BM-2A";
        double min_gpa = 2.0;

        void basarilari_listele(const std::vector<double>& gpalar) const {
            // [this] ile uye degiskenlere erisim
            std::cout << sinif_adi << " basarililari:\n";
            int sira = 1;
            std::for_each(gpalar.begin(), gpalar.end(),
                [this, &sira](double gpa) {
                    if (gpa >= min_gpa) {
                        std::cout << "  " << sira++ << ". GPA: "
                                  << std::fixed << std::setprecision(2) << gpa << "\n";
                    }
                });
        }
    };

    OgrenciSinifi sinif;
    sinif.basarilari_listele(gpalar);

    // --- 7. Dangling reference uyarisi ---
    std::cout << "\n--- Dangling Reference Tehlikesi ---\n";
    std::cout << "  [!] Lambda referans ile yakalama yaparsa\n";
    std::cout << "      ve yakalanan degiskenin omru biterse\n";
    std::cout << "      -> TANIMSIZ DAVRANIS (dangling reference)\n";
    std::cout << "  [+] Cozum: Deger ile yakala veya yasam\n";
    std::cout << "      surelerini dikkatle yonet.\n";

    // Guvenli ornek: fonksiyondan lambda dondurme
    auto olustur_filtre = [](double esik_degeri) {
        // esik_degeri DEGER ile yakalanir - guvenli
        return [esik_degeri](double gpa) { return gpa >= esik_degeri; };
    };

    auto onur_filtre = olustur_filtre(3.5);
    std::cout << "\nOnur filtresi sonuclari:\n";
    for (double g : gpalar) {
        if (onur_filtre(g)) {
            std::cout << "  GPA " << std::fixed << std::setprecision(2)
                      << g << " -> Onur listesi\n";
        }
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Yakalama Listesi ===
 *
 * --- Deger ile Yakalama [x] ---
 * Esik: 70
 * 80 basarili mi? Evet
 * 60 basarili mi? Hayir
 * Esik 90'a degisti, 80 basarili mi? Evet (lambda hala 70 kullanir)
 *
 * --- Referans ile Yakalama [&x] ---
 * Toplam: 438
 * Ortalama: 87.6
 *
 * --- Toplu Yakalama ---
 * Bilgisayar: 60-100
 * Aralikta olan: 5 not
 *
 * --- Karisik Yakalama ---
 * GPA >= 3: 3 ogrenci
 *
 * --- Init Capture ---
 * Sayac: 1
 * Sayac: 2
 * Tasimadan once: OBYS Sistemi
 * Tasimadan sonra: bos (tasinmis)
 * Lambda icinde: OBYS Sistemi
 * Onceden hesaplanan ortalama: 30
 *
 * --- this Yakalama ---
 * BM-2A basarililari:
 *   1. GPA: 3.45
 *   2. GPA: 3.80
 *   3. GPA: 2.90
 *   4. GPA: 3.65
 *   5. GPA: 2.10
 *
 * --- Dangling Reference Tehlikesi ---
 * ... (uyarilar)
 * Onur filtresi sonuclari:
 *   GPA 3.80 -> Onur listesi
 *   GPA 3.65 -> Onur listesi
 */

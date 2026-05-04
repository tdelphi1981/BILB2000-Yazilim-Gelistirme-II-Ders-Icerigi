/**
 * std::thread::hardware_concurrency Kullanımı
 *
 * Bu program, sistemin kaç donanım izlek destekleyebildiğini
 * raporlar ve bu bilgiyi thread havuzu boyutunu seçmek için
 * nasıl kullanacağımızı gösterir.
 *
 * Öğrenilecek kavramlar:
 * - Donanım thread sayısını sorgulama
 * - hardware_concurrency'nin 0 dönebilme durumu
 * - CPU-yoğun vs I/O-yoğun iş yüklerinde işçi sayısı seçimi
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 3 - Thread Havuzu Tasarımı
 *
 * Derleme: g++ -std=c++20 -pthread -o 05_hw 05_hardware_concurrency.cpp
 * Çalıştırma: ./05_hw
 */

#include <thread>
#include <iostream>
#include <algorithm>

std::size_t onerilenIsciSayisi(const std::string& isTuru) {
    unsigned n = std::thread::hardware_concurrency();
    if (n == 0) {
        std::cout << "Uyarı: hardware_concurrency belirlenemedi\n";
        n = 4;  // Fallback
    }

    if (isTuru == "cpu") {
        // CPU-yoğun: fiziksel çekirdek kadar (HT'yi saymama)
        // n zaten mantıksal çekirdek; HT'li sistemde n/2 de denenebilir
        return n;
    } else if (isTuru == "io") {
        // I/O-yoğun: çekirdekten fazla olabilir, bekleme paralelleşir
        return n * 2;
    } else if (isTuru == "karma") {
        return n + 2;
    }
    return n;
}

int main() {
    unsigned n = std::thread::hardware_concurrency();
    std::cout << "Sistem donanım izlek sayısı: " << n << "\n";

    if (n == 0) {
        std::cout << "(belirlenemedi -- fallback kullanılır)\n";
    }

    std::cout << "\nÖnerilen işçi sayıları:\n";
    std::cout << "  CPU-yoğun iş:  " << onerilenIsciSayisi("cpu")   << "\n";
    std::cout << "  I/O-yoğun iş:  " << onerilenIsciSayisi("io")    << "\n";
    std::cout << "  Karma iş:      " << onerilenIsciSayisi("karma") << "\n";

    std::cout << "\nÖnemli not: Bu değerler genel rehberdir. En iyi sayı\n";
    std::cout << "için kendi iş yükünüzde 1, 2, 4, 8... işçi ile ölçüm yapın.\n";
    return 0;
}

/*
 * BEKLENEN ÇIKTI (sistem donanımına bağlı):
 * -----------------------------------------
 * Sistem donanım izlek sayısı: 8
 *
 * Önerilen işçi sayıları:
 *   CPU-yoğun iş:  8
 *   I/O-yoğun iş:  16
 *   Karma iş:      10
 *
 * Önemli not: Bu değerler genel rehberdir. En iyi sayı
 * için kendi iş yükünüzde 1, 2, 4, 8... işçi ile ölçüm yapın.
 */

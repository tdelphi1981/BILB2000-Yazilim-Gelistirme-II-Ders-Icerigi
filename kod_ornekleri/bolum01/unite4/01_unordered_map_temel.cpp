/**
 * unordered_map Temel Kullanim
 *
 * Bu program, std::unordered_map kapsayicisinin temel kullanımini
 * gosterir. OBYS senaryosunda ogrenci numarasi ile hizli arama
 * yapilmasini ornekler.
 *
 * Ogrenilecek kavramlar:
 * - unordered_map tanimlama ve eleman ekleme
 * - find() ile O(1) ortalama arama
 * - Structured bindings ile gezinme
 * - contains() (C++20) ile uyelik testi
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 4 - Sirasiz Kapsayicilar ve Uyarlayicilar
 *
 * Derleme: g++ -std=c++20 -o 01_unordered_map_temel 01_unordered_map_temel.cpp
 * Calistirma: ./01_unordered_map_temel
 */

#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    // OBYS: Ogrenci numarasi -> isim eslesmesi
    std::unordered_map<int, std::string> ogrenciler;

    // operator[] ile eleman ekleme
    ogrenciler[1001] = "Ali Yilmaz";
    ogrenciler[1002] = "Ayse Kaya";
    ogrenciler[1003] = "Mehmet Demir";

    // insert() ile eleman ekleme
    ogrenciler.insert({1004, "Zeynep Celik"});
    ogrenciler.insert(std::make_pair(1005, "Can Ozturk"));

    std::cout << "=== OBYS Ogrenci Listesi ===" << '\n';
    std::cout << "Toplam ogrenci: " << ogrenciler.size() << '\n' << '\n';

    // Tum elemanlari gezme (sira garantisi yok!)
    std::cout << "Tum ogrenciler (sirasiz):" << '\n';
    for (const auto& [numara, isim] : ogrenciler) {
        std::cout << "  " << numara << ": " << isim << '\n';
    }

    // find() ile arama: O(1) ortalama
    std::cout << '\n' << "--- Arama Islemleri ---" << '\n';
    int aranan_no = 1003;
    if (auto it = ogrenciler.find(aranan_no); it != ogrenciler.end()) {
        std::cout << aranan_no << " bulundu: " << it->second << '\n';
    } else {
        std::cout << aranan_no << " bulunamadi!" << '\n';
    }

    // contains() ile uyelik testi (C++20)
    int olmayan_no = 9999;
    if (ogrenciler.contains(olmayan_no)) {
        std::cout << olmayan_no << " kayitli." << '\n';
    } else {
        std::cout << olmayan_no << " kayitli degil." << '\n';
    }

    // count() ile kontrol (0 veya 1 dondurur)
    std::cout << "1001 kayit sayisi: " << ogrenciler.count(1001) << '\n';
    std::cout << "8888 kayit sayisi: " << ogrenciler.count(8888) << '\n';

    // Eleman silme
    std::cout << '\n' << "--- Silme Islemi ---" << '\n';
    ogrenciler.erase(1002);
    std::cout << "1002 silindi. Kalan ogrenci: " << ogrenciler.size() << '\n';

    // Bucket bilgileri
    std::cout << '\n' << "--- Hash Tablosu Bilgileri ---" << '\n';
    std::cout << "Bucket sayisi: " << ogrenciler.bucket_count() << '\n';
    std::cout << "Yuk faktoru:   " << ogrenciler.load_factor() << '\n';
    std::cout << "Max yuk fakt.: " << ogrenciler.max_load_factor() << '\n';

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === OBYS Ogrenci Listesi ===
 * Toplam ogrenci: 5
 *
 * Tum ogrenciler (sirasiz):
 *   1005: Can Ozturk
 *   1004: Zeynep Celik
 *   1003: Mehmet Demir
 *   1002: Ayse Kaya
 *   1001: Ali Yilmaz
 *
 * --- Arama Islemleri ---
 * 1003 bulundu: Mehmet Demir
 * 9999 kayitli degil.
 * 1001 kayit sayisi: 1
 * 8888 kayit sayisi: 0
 *
 * --- Silme Islemi ---
 * 1002 silindi. Kalan ogrenci: 4
 *
 * --- Hash Tablosu Bilgileri ---
 * Bucket sayisi: 7
 * Yuk faktoru:   0.571429
 * Max yuk fakt.: 1
 *
 * NOT: Eleman sirasi farkli olabilir (hash tablosu sirasiz calisir)
 */

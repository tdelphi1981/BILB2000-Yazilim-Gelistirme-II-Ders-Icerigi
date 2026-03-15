/**
 * map Kullanimi - Ogrenci Bilgi Yonetimi
 *
 * Bu program std::map kullaniarak ogrenci numarasi ile
 * ogrenci bilgilerini eslestirmeyi gosterir. Ekleme, arama,
 * guncelleme ve silme islemlerini icerir.
 *
 * Ogrenilecek kavramlar:
 * - std::map olusturma ve baslangic listesi
 * - insert(), emplace(), operator[] ile ekleme
 * - find(), at(), contains() ile arama
 * - erase() ile silme
 * - Range-based for ile dolasma
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 3 - Iliskisel Kapsayicilar
 *
 * Derleme: g++ -std=c++20 -o 01_map_kullanimi 01_map_kullanimi.cpp
 * Calistirma: ./01_map_kullanimi
 */

#include <iostream>
#include <map>
#include <string>

int main() {
    // --- 1. map Olusturma ---
    std::cout << "=== map Olusturma ve Ekleme ===\n\n";

    // Baslangic listesi ile olustur
    std::map<int, std::string> ogrenciler = {
        {2001, "Ali Yilmaz"},
        {2003, "Mehmet Demir"},
        {2002, "Ayse Kaya"}
    };

    // insert() ile ekle
    ogrenciler.insert({2004, "Zeynep Celik"});

    // emplace() ile ekle
    ogrenciler.emplace(2005, "Fatma Sahin");

    // operator[] ile ekle
    ogrenciler[2006] = "Can Ozturk";

    // Elemanlari listele (otomatik sirali!)
    std::cout << "Tum ogrenciler (sirali):\n";
    for (const auto& [numara, isim] : ogrenciler) {
        std::cout << "  " << numara << " -> " << isim << "\n";
    }

    // --- 2. Arama Islemleri ---
    std::cout << "\n=== Arama Islemleri ===\n\n";

    // find() ile arama
    auto it = ogrenciler.find(2003);
    if (it != ogrenciler.end()) {
        std::cout << "find(2003): " << it->second << "\n";
    }

    // at() ile erisim
    try {
        std::string isim = ogrenciler.at(2001);
        std::cout << "at(2001): " << isim << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Bulunamadi!\n";
    }

    // contains() ile kontrol (C++20)
    std::cout << "contains(2005): " << std::boolalpha
              << ogrenciler.contains(2005) << "\n";
    std::cout << "contains(9999): " << std::boolalpha
              << ogrenciler.contains(9999) << "\n";

    // --- 3. Guncelleme ---
    std::cout << "\n=== Guncelleme ===\n\n";

    std::cout << "Oncesi - 2003: " << ogrenciler[2003] << "\n";
    ogrenciler[2003] = "Mehmet Can Demir";
    std::cout << "Sonrasi - 2003: " << ogrenciler[2003] << "\n";

    // --- 4. Silme ---
    std::cout << "\n=== Silme ===\n\n";

    std::cout << "Silme oncesi boyut: " << ogrenciler.size() << "\n";
    ogrenciler.erase(2006);
    std::cout << "2006 silindi. Yeni boyut: " << ogrenciler.size() << "\n";

    // --- 5. operator[] Tuzagi ---
    std::cout << "\n=== operator[] Tuzagi ===\n\n";

    std::cout << "Boyut (oncesi): " << ogrenciler.size() << "\n";
    // Dikkat: var olmayan anahtara erisim otomatik ekler!
    std::string yok = ogrenciler[9999];
    std::cout << "Boyut (sonrasi): " << ogrenciler.size() << "\n";
    std::cout << "9999 icin bos deger eklendi: \""
              << ogrenciler[9999] << "\"\n";

    // Temizle
    ogrenciler.erase(9999);

    // --- 6. Son Durum ---
    std::cout << "\n=== Son Durum ===\n\n";
    for (const auto& [numara, isim] : ogrenciler) {
        std::cout << "  " << numara << " -> " << isim << "\n";
    }
    std::cout << "Toplam: " << ogrenciler.size() << " ogrenci\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ----------------
 * === map Olusturma ve Ekleme ===
 *
 * Tum ogrenciler (sirali):
 *   2001 -> Ali Yilmaz
 *   2002 -> Ayse Kaya
 *   2003 -> Mehmet Demir
 *   2004 -> Zeynep Celik
 *   2005 -> Fatma Sahin
 *   2006 -> Can Ozturk
 *
 * === Arama Islemleri ===
 *
 * find(2003): Mehmet Demir
 * at(2001): Ali Yilmaz
 * contains(2005): true
 * contains(9999): false
 *
 * === Guncelleme ===
 *
 * Oncesi - 2003: Mehmet Demir
 * Sonrasi - 2003: Mehmet Can Demir
 *
 * === Silme ===
 *
 * Silme oncesi boyut: 6
 * 2006 silindi. Yeni boyut: 5
 *
 * === operator[] Tuzagi ===
 *
 * Boyut (oncesi): 5
 * Boyut (sonrasi): 6
 * 9999 icin bos deger eklendi: ""
 *
 * === Son Durum ===
 *
 *   2001 -> Ali Yilmaz
 *   2002 -> Ayse Kaya
 *   2003 -> Mehmet Can Demir
 *   2004 -> Zeynep Celik
 *   2005 -> Fatma Sahin
 * Toplam: 5 ogrenci
 */

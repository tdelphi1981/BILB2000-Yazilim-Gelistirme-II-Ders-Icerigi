/**
 * Exception Tasarimi - Ozel Istisna Hiyerarsisi
 *
 * Bu program, OBYS projesi icin ozel bir istisna hiyerarsisi tasarlar.
 * Dogru istisna sinifi tasarimi, noexcept kullanimi ve const referans
 * ile yakalama gibi iyi pratikleri gosterir.
 *
 * Ogrenilecek kavramlar:
 * - Ozel istisna hiyerarsisi tasarimi
 * - std::runtime_error'dan turetme
 * - noexcept fonksiyon belirteci
 * - const referans ile yakalama
 * - Exception vs optional secim kriterleri
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 3 - Hata Yonetimi
 *
 * Derleme: g++ -std=c++20 -o 03_exception_tasarimi 03_exception_tasarimi.cpp
 * Calistirma: ./03_exception_tasarimi
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <optional>

// =============================================
// OBYS Istisna Hiyerarsisi
// =============================================

// Temel sinif: tum OBYS hatalari bundan turetilir
class OBYSHatasi : public std::runtime_error {
public:
    explicit OBYSHatasi(const std::string& mesaj)
        : std::runtime_error("OBYS Hatasi: " + mesaj) {}
};

// Ogrenci bulunamadi (numara ile arama basarisiz)
class OgrenciBulunamadi : public OBYSHatasi {
    int numara_;
public:
    explicit OgrenciBulunamadi(int numara)
        : OBYSHatasi(std::to_string(numara) + " numarali ogrenci bulunamadi"),
          numara_(numara) {}
    int numara() const noexcept { return numara_; }
};

// Gecersiz not degeri
class GecersizNot : public OBYSHatasi {
    int not_degeri_;
public:
    explicit GecersizNot(int not_d)
        : OBYSHatasi("Gecersiz not degeri: " + std::to_string(not_d)
                      + " (0-100 araliginda olmali)"),
          not_degeri_(not_d) {}
    int not_degeri() const noexcept { return not_degeri_; }
};

// Ders cakismasi
class DersCakismasi : public OBYSHatasi {
    std::string ders1_, ders2_;
public:
    DersCakismasi(const std::string& d1, const std::string& d2)
        : OBYSHatasi(d1 + " ile " + d2 + " dersleri cakisiyor"),
          ders1_(d1), ders2_(d2) {}
    const std::string& ders1() const noexcept { return ders1_; }
    const std::string& ders2() const noexcept { return ders2_; }
};

// =============================================
// OBYS Servis Katmani
// =============================================

struct Ogrenci {
    int numara;
    std::string isim;
    int notu;
};

class OgrenciServisi {
    std::vector<Ogrenci> liste_;
public:
    OgrenciServisi(std::initializer_list<Ogrenci> init) : liste_(init) {}

    // [ISTISNA] Numara ile bul - bulamama hata durumudur
    const Ogrenci& bul_veya_hata(int numara) const {
        for (const auto& ogr : liste_)
            if (ogr.numara == numara) return ogr;
        throw OgrenciBulunamadi(numara);
    }

    // [OPTIONAL] Numara ile bul - bulamama normal durumdur
    std::optional<Ogrenci> bul(int numara) const {
        for (const auto& ogr : liste_)
            if (ogr.numara == numara) return ogr;
        return std::nullopt;
    }

    // [ISTISNA] Not guncelle - gecersiz not hata durumudur
    void not_guncelle(int numara, int yeni_not) {
        if (yeni_not < 0 || yeni_not > 100)
            throw GecersizNot(yeni_not);
        auto& ogr = const_cast<Ogrenci&>(bul_veya_hata(numara));
        ogr.notu = yeni_not;
    }

    // noexcept fonksiyon - istisna firlatmayacagini garanti eder
    size_t ogrenci_sayisi() const noexcept {
        return liste_.size();
    }
};

int main() {
    OgrenciServisi servis = {
        {1001, "Ayse Yilmaz",  85},
        {1002, "Mehmet Demir", 72},
        {1003, "Zeynep Kara",  91}
    };

    // =============================================
    // 1) Basarili Islemler
    // =============================================
    std::cout << "=== Basarili Islemler ===\n";
    std::cout << "  Toplam ogrenci: " << servis.ogrenci_sayisi() << "\n";

    try {
        const auto& ogr = servis.bul_veya_hata(1001);
        std::cout << "  Bulunan: " << ogr.isim << " (Not: " << ogr.notu << ")\n";
    } catch (const OBYSHatasi& e) {
        std::cout << "  " << e.what() << "\n";
    }

    // =============================================
    // 2) OgrenciBulunamadi Istisnasi
    // =============================================
    std::cout << "\n=== OgrenciBulunamadi ===\n";

    try {
        servis.bul_veya_hata(9999);
    } catch (const OgrenciBulunamadi& e) {
        std::cout << "  " << e.what() << "\n";
        std::cout << "  Aranan numara: " << e.numara() << "\n";
    }

    // =============================================
    // 3) GecersizNot Istisnasi
    // =============================================
    std::cout << "\n=== GecersizNot ===\n";

    try {
        servis.not_guncelle(1001, 150);  // 0-100 disinda!
    } catch (const GecersizNot& e) {
        std::cout << "  " << e.what() << "\n";
        std::cout << "  Girilen not: " << e.not_degeri() << "\n";
    }

    // =============================================
    // 4) DersCakismasi Istisnasi
    // =============================================
    std::cout << "\n=== DersCakismasi ===\n";

    try {
        throw DersCakismasi("Matematik", "Fizik");
    } catch (const DersCakismasi& e) {
        std::cout << "  " << e.what() << "\n";
        std::cout << "  Dersler: " << e.ders1() << ", " << e.ders2() << "\n";
    }

    // =============================================
    // 5) Hiyerarsik Yakalama
    // =============================================
    std::cout << "\n=== Hiyerarsik Yakalama ===\n";

    // Temel sinif ile tum OBYS hatalarini yakala
    try {
        servis.bul_veya_hata(5555);
    } catch (const OBYSHatasi& e) {
        std::cout << "  OBYSHatasi olarak yakalandi: " << e.what() << "\n";
    }

    // runtime_error ile de yakalanabilir
    try {
        servis.not_guncelle(1001, -10);
    } catch (const std::runtime_error& e) {
        std::cout << "  runtime_error olarak yakalandi: " << e.what() << "\n";
    }

    // =============================================
    // 6) Exception vs Optional Secimi
    // =============================================
    std::cout << "\n=== Exception vs Optional ===\n";

    // optional: bulamama normal bir durum
    if (auto sonuc = servis.bul(9999)) {
        std::cout << "  " << sonuc->isim << "\n";
    } else {
        std::cout << "  optional ile: ogrenci yok (normal akis)\n";
    }

    // exception: bulamama bir hata durumu
    try {
        servis.bul_veya_hata(9999);
    } catch (const OgrenciBulunamadi&) {
        std::cout << "  exception ile: ogrenci yok (hata durumu)\n";
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Basarili Islemler ===
 *   Toplam ogrenci: 3
 *   Bulunan: Ayse Yilmaz (Not: 85)
 *
 * === OgrenciBulunamadi ===
 *   OBYS Hatasi: 9999 numarali ogrenci bulunamadi
 *   Aranan numara: 9999
 *
 * === GecersizNot ===
 *   OBYS Hatasi: Gecersiz not degeri: 150 (0-100 araliginda olmali)
 *   Girilen not: 150
 *
 * === DersCakismasi ===
 *   OBYS Hatasi: Matematik ile Fizik dersleri cakisiyor
 *   Dersler: Matematik, Fizik
 *
 * === Hiyerarsik Yakalama ===
 *   OBYSHatasi olarak yakalandi: OBYS Hatasi: 5555 numarali ogrenci bulunamadi
 *   runtime_error olarak yakalandi: OBYS Hatasi: Gecersiz not degeri: -10 (0-100 araliginda olmali)
 *
 * === Exception vs Optional ===
 *   optional ile: ogrenci yok (normal akis)
 *   exception ile: ogrenci yok (hata durumu)
 */

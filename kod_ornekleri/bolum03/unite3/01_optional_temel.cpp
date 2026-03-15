/**
 * Optional Temelleri - Deger Olabilir veya Olmayabilir
 *
 * Bu program, C++17 ile gelen std::optional turunu tanitir.
 * Bir fonksiyon sonuc bulamayabilir; optional bu durumu guvenli
 * sekilde ifade eder. OBYS baglaminda ogrenci arama ornegi sunar.
 *
 * Ogrenilecek kavramlar:
 * - std::optional olusturma: deger, nullopt, varsayilan
 * - Erisim: value(), value_or(), operator*, operator->
 * - Bool donusumu ve if-init kaliplari
 * - optional donduren fonksiyonlar
 * - bad_optional_access istisnasi
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 3 - Hata Yonetimi
 *
 * Derleme: g++ -std=c++20 -o 01_optional_temel 01_optional_temel.cpp
 * Calistirma: ./01_optional_temel
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>

// -- OBYS: Ogrenci yapisi --
struct Ogrenci {
    int numara;
    std::string isim;
    double ortalama;
};

// -- OBYS: Basit ogrenci deposu --
class OgrenciDepo {
    std::vector<Ogrenci> liste_;
public:
    OgrenciDepo(std::initializer_list<Ogrenci> init) : liste_(init) {}

    // Numara ile arama - bulamazsa nullopt doner
    std::optional<Ogrenci> bul(int numara) const {
        for (const auto& ogr : liste_) {
            if (ogr.numara == numara)
                return ogr;            // deger iceren optional
        }
        return std::nullopt;            // bos optional
    }

    // En yuksek ortalamayi dondur (bos depo icin nullopt)
    std::optional<double> en_yuksek_ortalama() const {
        if (liste_.empty())
            return std::nullopt;
        double maks = liste_[0].ortalama;
        for (const auto& ogr : liste_)
            if (ogr.ortalama > maks) maks = ogr.ortalama;
        return maks;
    }
};

int main() {
    // =============================================
    // 1) optional Olusturma Yollari
    // =============================================
    std::cout << "=== optional Olusturma ===\n";

    std::optional<int> a = 42;              // deger ile
    std::optional<int> b = std::nullopt;    // bos
    std::optional<int> c;                   // varsayilan: bos

    std::cout << "  a dolu mu? " << (a.has_value() ? "Evet" : "Hayir") << "\n";
    std::cout << "  b dolu mu? " << (b.has_value() ? "Evet" : "Hayir") << "\n";
    std::cout << "  c dolu mu? " << (c.has_value() ? "Evet" : "Hayir") << "\n";

    // =============================================
    // 2) Degere Erisim Yollari
    // =============================================
    std::cout << "\n=== Degere Erisim ===\n";

    std::cout << "  a.value()    = " << a.value() << "\n";
    std::cout << "  *a           = " << *a << "\n";
    std::cout << "  b.value_or(0)= " << b.value_or(0) << "\n";

    // operator-> ile struct erisimi
    std::optional<Ogrenci> ogr = Ogrenci{1001, "Ayse Yilmaz", 85.5};
    std::cout << "  ogr->isim    = " << ogr->isim << "\n";

    // =============================================
    // 3) Bool Donusumu ve if-init
    // =============================================
    std::cout << "\n=== Bool Donusumu ===\n";

    if (a) {
        std::cout << "  a bir deger tasiyor: " << *a << "\n";
    }
    if (!b) {
        std::cout << "  b bos (nullopt)\n";
    }

    // =============================================
    // 4) OBYS: Ogrenci Arama
    // =============================================
    std::cout << "\n=== OBYS Ogrenci Arama ===\n";

    OgrenciDepo depo = {
        {1001, "Ayse Yilmaz",  85.5},
        {1002, "Mehmet Demir", 72.0},
        {1003, "Zeynep Kara",  91.3}
    };

    // if-init kalipli arama
    if (auto sonuc = depo.bul(1002)) {
        std::cout << "  Bulundu: " << sonuc->isim
                  << " (Ort: " << sonuc->ortalama << ")\n";
    }

    if (auto sonuc = depo.bul(9999)) {
        std::cout << "  Bulundu: " << sonuc->isim << "\n";
    } else {
        std::cout << "  9999 numarali ogrenci bulunamadi\n";
    }

    // value_or ile varsayilan deger
    auto ort = depo.en_yuksek_ortalama().value_or(0.0);
    std::cout << "  En yuksek ortalama: " << ort << "\n";

    // =============================================
    // 5) bad_optional_access Istisnasi
    // =============================================
    std::cout << "\n=== bad_optional_access ===\n";

    try {
        std::optional<int> bos;
        int val = bos.value();  // istisna firlatir!
        std::cout << "  Bu satir calismazz: " << val << "\n";
    } catch (const std::bad_optional_access& e) {
        std::cout << "  Yakalanan hata: " << e.what() << "\n";
    }

    // =============================================
    // 6) optional Zincirleme
    // =============================================
    std::cout << "\n=== optional Zincirleme ===\n";

    auto ogr_sonuc = depo.bul(1003);
    // C++23 oncesi zincirleme: manuel kontrol
    std::optional<std::string> isim_sonuc;
    if (ogr_sonuc) {
        isim_sonuc = ogr_sonuc->isim;
    }
    std::cout << "  Ogrenci ismi: "
              << isim_sonuc.value_or("(bulunamadi)") << "\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === optional Olusturma ===
 *   a dolu mu? Evet
 *   b dolu mu? Hayir
 *   c dolu mu? Hayir
 *
 * === Degere Erisim ===
 *   a.value()    = 42
 *   *a           = 42
 *   b.value_or(0)= 0
 *   ogr->isim    = Ayse Yilmaz
 *
 * === Bool Donusumu ===
 *   a bir deger tasiyor: 42
 *   b bos (nullopt)
 *
 * === OBYS Ogrenci Arama ===
 *   Bulundu: Mehmet Demir (Ort: 72)
 *   9999 numarali ogrenci bulunamadi
 *   En yuksek ortalama: 91.3
 *
 * === bad_optional_access ===
 *   Yakalanan hata: bad_optional_access
 *
 * === optional Zincirleme ===
 *   Ogrenci ismi: Zeynep Kara
 */

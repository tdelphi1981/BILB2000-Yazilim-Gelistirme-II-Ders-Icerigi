/**
 * OBYS Hata Yonetimi - Kapsamli Entegrasyon Ornegi
 *
 * Bu program, optional, variant ve exception stratejilerini birlikte
 * kullanan kapsamli bir OBYS servis katmani gosterir. Her strateji
 * uygun oldugu yerde kullanilir: optional arama icin, variant
 * polimorfik not turleri icin, exception hata durumlari icin.
 *
 * Ogrenilecek kavramlar:
 * - optional: "bulunamadi" senaryolari (normal akis)
 * - variant: polimorfik not turleri (sayi, harf, muaf)
 * - exception: hata durumlari (tekrar kayit, gecersiz deger)
 * - Uc stratejinin birlikte kullanimi
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 3 - Hata Yonetimi
 *
 * Derleme: g++ -std=c++20 -o 04_obys_hata_yonetimi 04_obys_hata_yonetimi.cpp
 * Calistirma: ./04_obys_hata_yonetimi
 */

#include <iostream>
#include <optional>
#include <variant>
#include <stdexcept>
#include <string>
#include <vector>
#include <iomanip>

// -- overloaded kalip deseni --
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

// =============================================
// Istisna Hiyerarsisi
// =============================================
class OBYSHatasi : public std::runtime_error {
public:
    explicit OBYSHatasi(const std::string& mesaj)
        : std::runtime_error("[OBYS] " + mesaj) {}
};

class TekrarKayit : public OBYSHatasi {
    int numara_;
public:
    explicit TekrarKayit(int no)
        : OBYSHatasi(std::to_string(no) + " numarali ogrenci zaten kayitli"),
          numara_(no) {}
    int numara() const noexcept { return numara_; }
};

class GecersizNot : public OBYSHatasi {
public:
    explicit GecersizNot(const std::string& detay)
        : OBYSHatasi("Gecersiz not: " + detay) {}
};

// =============================================
// Veri Modeli
// =============================================
struct Muaf { std::string sebep; };

using DersNotu = std::variant<int, std::string, Muaf>;

struct Ogrenci {
    int numara;
    std::string isim;
};

struct NotKaydi {
    int ogrenci_no;
    std::string ders;
    DersNotu not_degeri;
};

// =============================================
// OBYS Servis Katmani
// =============================================
class OBYSServisi {
    std::vector<Ogrenci> ogrenciler_;
    std::vector<NotKaydi> notlar_;

public:
    // [OPTIONAL] Ogrenci arama - bulamama normal bir durumdur
    std::optional<Ogrenci> bul(int numara) const {
        for (const auto& ogr : ogrenciler_)
            if (ogr.numara == numara) return ogr;
        return std::nullopt;
    }

    // [EXCEPTION] Ogrenci ekleme - tekrar kayit bir hatadir
    void ekle(int numara, const std::string& isim) {
        if (bul(numara).has_value())
            throw TekrarKayit(numara);
        ogrenciler_.push_back({numara, isim});
    }

    // [VARIANT] Not alma - farkli not turleri donebilir
    DersNotu not_al(int numara, const std::string& ders) const {
        for (const auto& nk : notlar_)
            if (nk.ogrenci_no == numara && nk.ders == ders)
                return nk.not_degeri;
        // Kayit yoksa muaf olarak dondur
        return Muaf{"Not kaydi bulunamadi"};
    }

    // [EXCEPTION] Not kaydet - gecersiz deger bir hatadir
    void not_kaydet(int numara, const std::string& ders, DersNotu not_d) {
        // Ogrenci kontrolu
        if (!bul(numara).has_value())
            throw OBYSHatasi(std::to_string(numara) + " numarali ogrenci yok");

        // Sayi notu kontrolu
        if (auto* sayi = std::get_if<int>(&not_d)) {
            if (*sayi < 0 || *sayi > 100)
                throw GecersizNot(std::to_string(*sayi) + " (0-100 olmali)");
        }

        // Harf notu kontrolu
        if (auto* harf = std::get_if<std::string>(&not_d)) {
            const std::vector<std::string> gecerli = {
                "AA","BA","BB","CB","CC","DC","DD","FD","FF"};
            bool bulundu = false;
            for (const auto& g : gecerli)
                if (g == *harf) { bulundu = true; break; }
            if (!bulundu)
                throw GecersizNot(*harf + " gecerli bir harf notu degil");
        }

        notlar_.push_back({numara, ders, not_d});
    }

    // Ogrenci listesini yazdir
    void listele() const {
        for (const auto& ogr : ogrenciler_)
            std::cout << "  [" << ogr.numara << "] " << ogr.isim << "\n";
    }

    size_t ogrenci_sayisi() const noexcept { return ogrenciler_.size(); }
};

// Not degerini metne cevir
std::string not_metni(const DersNotu& n) {
    return std::visit(overloaded{
        [](int sayi) { return std::to_string(sayi); },
        [](const std::string& harf) { return harf; },
        [](const Muaf& m) { return std::string("Muaf: " + m.sebep); }
    }, n);
}

int main() {
    OBYSServisi obys;

    // =============================================
    // 1) Ogrenci Ekleme (exception stratejisi)
    // =============================================
    std::cout << "=== Ogrenci Ekleme ===\n";

    try {
        obys.ekle(1001, "Ayse Yilmaz");
        obys.ekle(1002, "Mehmet Demir");
        obys.ekle(1003, "Zeynep Kara");
        std::cout << "  3 ogrenci basariyla eklendi\n";

        // Tekrar ekleme denemesi
        obys.ekle(1001, "Ayse Yilmaz");
    } catch (const TekrarKayit& e) {
        std::cout << "  " << e.what() << "\n";
    }

    // =============================================
    // 2) Ogrenci Arama (optional stratejisi)
    // =============================================
    std::cout << "\n=== Ogrenci Arama ===\n";

    if (auto ogr = obys.bul(1002)) {
        std::cout << "  Bulundu: " << ogr->isim << "\n";
    }

    if (auto ogr = obys.bul(9999)) {
        std::cout << "  " << ogr->isim << "\n";
    } else {
        std::cout << "  9999 numarali ogrenci bulunamadi (normal durum)\n";
    }

    // =============================================
    // 3) Not Kaydetme (exception + variant)
    // =============================================
    std::cout << "\n=== Not Kaydetme ===\n";

    try {
        obys.not_kaydet(1001, "Matematik", 85);
        std::cout << "  1001 Matematik: sayi notu kaydedildi\n";

        obys.not_kaydet(1002, "Fizik", std::string("BB"));
        std::cout << "  1002 Fizik: harf notu kaydedildi\n";

        obys.not_kaydet(1003, "Kimya", Muaf{"Yatay gecis"});
        std::cout << "  1003 Kimya: muafiyet kaydedildi\n";
    } catch (const OBYSHatasi& e) {
        std::cout << "  " << e.what() << "\n";
    }

    // Hatali not denemeleri
    std::cout << "\n=== Hatali Not Denemeleri ===\n";

    try {
        obys.not_kaydet(1001, "Fizik", 150);  // 0-100 disinda
    } catch (const GecersizNot& e) {
        std::cout << "  " << e.what() << "\n";
    }

    try {
        obys.not_kaydet(1002, "Kimya", std::string("XY"));  // gecersiz harf
    } catch (const GecersizNot& e) {
        std::cout << "  " << e.what() << "\n";
    }

    try {
        obys.not_kaydet(5555, "Biyoloji", 70);  // olmayan ogrenci
    } catch (const OBYSHatasi& e) {
        std::cout << "  " << e.what() << "\n";
    }

    // =============================================
    // 4) Not Sorgulama (variant stratejisi)
    // =============================================
    std::cout << "\n=== Not Sorgulama ===\n";

    struct Sorgu { int no; std::string ders; };
    std::vector<Sorgu> sorgular = {
        {1001, "Matematik"}, {1002, "Fizik"},
        {1003, "Kimya"},     {1001, "Biyoloji"}
    };

    for (const auto& s : sorgular) {
        DersNotu notu = obys.not_al(s.no, s.ders);
        std::cout << "  " << s.no << " " << std::setw(10) << std::left
                  << s.ders << " -> " << not_metni(notu) << "\n";
    }

    // =============================================
    // 5) Strateji Ozeti
    // =============================================
    std::cout << "\n=== Hata Yonetimi Strateji Ozeti ===\n";
    std::cout << "  optional  -> bul()        : bulamama normal akistir\n";
    std::cout << "  variant   -> not_al()     : farkli tur donusleri\n";
    std::cout << "  exception -> ekle/kaydet  : is kurali ihlali hatadir\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Ogrenci Ekleme ===
 *   3 ogrenci basariyla eklendi
 *   [OBYS] 1001 numarali ogrenci zaten kayitli
 *
 * === Ogrenci Arama ===
 *   Bulundu: Mehmet Demir
 *   9999 numarali ogrenci bulunamadi (normal durum)
 *
 * === Not Kaydetme ===
 *   1001 Matematik: sayi notu kaydedildi
 *   1002 Fizik: harf notu kaydedildi
 *   1003 Kimya: muafiyet kaydedildi
 *
 * === Hatali Not Denemeleri ===
 *   [OBYS] Gecersiz not: 150 (0-100 olmali)
 *   [OBYS] Gecersiz not: XY gecerli bir harf notu degil
 *   [OBYS] 5555 numarali ogrenci yok
 *
 * === Not Sorgulama ===
 *   1001 Matematik  -> 85
 *   1002 Fizik       -> BB
 *   1003 Kimya       -> Muaf: Yatay gecis
 *   1001 Biyoloji    -> Muaf: Not kaydi bulunamadi
 *
 * === Hata Yonetimi Strateji Ozeti ===
 *   optional  -> bul()        : bulamama normal akistir
 *   variant   -> not_al()     : farkli tur donusleri
 *   exception -> ekle/kaydet  : is kurali ihlali hatadir
 */

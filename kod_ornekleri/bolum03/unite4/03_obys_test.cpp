/**
 * OBYS Test - assert Tabanli Birim Testleri
 *
 * Bu program, OBYS veri ve servis katmanlarini basit assert
 * tabanli testlerle dogrular. Ogrenci ekleme, bulma, kenar
 * durumlari, hata durumlari, not turleri ve ranges sorgulama
 * islevlerini test eder.
 *
 * Ogrenilecek kavramlar:
 * - assert() ile birim test yazimi
 * - Ozel CHECK makrosu ile test raporlama
 * - Exception beklenen testler (try/catch deseni)
 * - optional donus degerlerinin test edilmesi
 * - Kenar durumlari (bos depo, sinir degerleri)
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 4 - OBYS STL Donusumu - Mini Proje
 *
 * Derleme: g++ -std=c++20 -o 03_obys_test 03_obys_test.cpp
 * Calistirma: ./03_obys_test
 */

#include <iostream>
#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <optional>
#include <variant>
#include <stdexcept>
#include <format>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <functional>

// --- Test Altyapisi ---

int toplam_test = 0;
int basarili_test = 0;
int basarisiz_test = 0;

#define CHECK(ifade, mesaj)                                         \
    do {                                                            \
        ++toplam_test;                                              \
        if (ifade) {                                                \
            ++basarili_test;                                        \
            std::cout << std::format("  [+] {}\n", mesaj);         \
        } else {                                                    \
            ++basarisiz_test;                                       \
            std::cout << std::format("  [-] {} (BASARISIZ)\n", mesaj); \
        }                                                           \
    } while (0)

// --- Exception Hiyerarsisi ---

class OBYSHatasi : public std::runtime_error {
public:
    explicit OBYSHatasi(const std::string& m) : std::runtime_error(m) {}
};

class OgrenciBulunamadi : public OBYSHatasi {
public:
    explicit OgrenciBulunamadi(int n)
        : OBYSHatasi(std::format("Ogrenci bulunamadi: {}", n)) {}
};

class GecersizNot : public OBYSHatasi {
public:
    explicit GecersizNot(const std::string& d)
        : OBYSHatasi(std::format("Gecersiz not: {}", d)) {}
};

class TekrarNumara : public OBYSHatasi {
public:
    explicit TekrarNumara(int n)
        : OBYSHatasi(std::format("Tekrar numara: {}", n)) {}
};

// --- Veri Yapilari ---

struct Muaf {};
using DersNotu = std::variant<int, std::string, Muaf>;

struct Ogrenci {
    std::string isim;
    int numara;
    std::string bolum;
    double ortalama;
};

// --- OgrenciDepo ---

class OgrenciDepo {
    std::map<int, Ogrenci> depom;
public:
    void ekle(const Ogrenci& ogr) {
        if (depom.contains(ogr.numara)) throw TekrarNumara(ogr.numara);
        depom.emplace(ogr.numara, ogr);
    }

    std::optional<Ogrenci> bul(int numara) const {
        auto it = depom.find(numara);
        if (it != depom.end()) return it->second;
        return std::nullopt;
    }

    void sil(int numara) {
        if (depom.erase(numara) == 0) throw OgrenciBulunamadi(numara);
    }

    std::size_t boyut() const { return depom.size(); }
    const std::map<int, Ogrenci>& tumunu_al() const { return depom; }
};

// --- Not dogrulama ---

void not_dogrula(const DersNotu& not_degeri) {
    std::visit([](const auto& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
            if (val < 0 || val > 100)
                throw GecersizNot(std::format("Puan 0-100 arasi olmali: {}", val));
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::vector<std::string> gecerli = {"AA","BA","BB","CB","CC","DC","DD","FF"};
            if (std::ranges::find(gecerli, val) == gecerli.end())
                throw GecersizNot(std::format("Gecersiz harf: {}", val));
        }
    }, not_degeri);
}

// --- Test Fonksiyonlari ---

void testOgrenciEkleme() {
    std::cout << "\n--- testOgrenciEkleme ---\n";
    OgrenciDepo depo;

    depo.ekle({"Ali Yilmaz", 1001, "Bilgisayar", 80.0});
    CHECK(depo.boyut() == 1, "Bir ogrenci eklendi, boyut = 1");

    depo.ekle({"Veli Kaya", 1002, "Elektronik", 70.0});
    CHECK(depo.boyut() == 2, "Ikinci ogrenci eklendi, boyut = 2");

    auto ogr = depo.bul(1001);
    CHECK(ogr.has_value(), "Eklenen ogrenci bulunabiliyor");
    CHECK(ogr->isim == "Ali Yilmaz", "Isim dogru kaydedildi");
    CHECK(ogr->numara == 1001, "Numara dogru kaydedildi");
}

void testOgrenciBulma() {
    std::cout << "\n--- testOgrenciBulma ---\n";
    OgrenciDepo depo;
    depo.ekle({"Zeynep Kara", 2001, "Bilgisayar", 91.0});

    auto var_olan = depo.bul(2001);
    CHECK(var_olan.has_value(), "Var olan ogrenci bulundu");
    CHECK(var_olan->isim == "Zeynep Kara", "Bulunan ogrenci dogru");

    auto yok_olan = depo.bul(9999);
    CHECK(!yok_olan.has_value(), "Olmayan ogrenci -> nullopt");
}

void testKenarDurumlari() {
    std::cout << "\n--- testKenarDurumlari ---\n";
    OgrenciDepo depo;

    CHECK(depo.boyut() == 0, "Bos depo boyutu = 0");

    auto sonuc = depo.bul(1);
    CHECK(!sonuc.has_value(), "Bos depoda arama -> nullopt");

    // Sinir deger numaralari
    depo.ekle({"Min Numara", 0, "Test", 50.0});
    depo.ekle({"Buyuk Numara", 999999, "Test", 60.0});
    CHECK(depo.bul(0).has_value(), "Numara 0 gecerli ve bulunabiliyor");
    CHECK(depo.bul(999999).has_value(), "Buyuk numara gecerli ve bulunabiliyor");
}

void testHataDurumlari() {
    std::cout << "\n--- testHataDurumlari ---\n";
    OgrenciDepo depo;
    depo.ekle({"Ahmet Can", 3001, "Makine", 75.0});

    // Tekrar numara ekleme
    bool tekrar_hatasi = false;
    try {
        depo.ekle({"Baska Kisi", 3001, "Fizik", 60.0});
    } catch (const TekrarNumara&) {
        tekrar_hatasi = true;
    }
    CHECK(tekrar_hatasi, "Tekrar numara -> TekrarNumara exception");

    // Olmayan ogrenciyi silme
    bool bulunamadi_hatasi = false;
    try {
        depo.sil(7777);
    } catch (const OgrenciBulunamadi&) {
        bulunamadi_hatasi = true;
    }
    CHECK(bulunamadi_hatasi, "Olmayan sil -> OgrenciBulunamadi exception");
}

void testNotTurleri() {
    std::cout << "\n--- testNotTurleri ---\n";

    // Gecerli tamsayi not
    DersNotu n1 = 85;
    bool n1_gecerli = true;
    try { not_dogrula(n1); } catch (...) { n1_gecerli = false; }
    CHECK(n1_gecerli, "Puan 85 gecerli");

    // Gecerli harf notu
    DersNotu n2 = std::string("AA");
    bool n2_gecerli = true;
    try { not_dogrula(n2); } catch (...) { n2_gecerli = false; }
    CHECK(n2_gecerli, "Harf 'AA' gecerli");

    // Muaf (her zaman gecerli)
    DersNotu n3 = Muaf{};
    bool n3_gecerli = true;
    try { not_dogrula(n3); } catch (...) { n3_gecerli = false; }
    CHECK(n3_gecerli, "Muaf her zaman gecerli");

    // Gecersiz puan
    DersNotu n4 = 150;
    bool n4_hata = false;
    try { not_dogrula(n4); } catch (const GecersizNot&) { n4_hata = true; }
    CHECK(n4_hata, "Puan 150 -> GecersizNot exception");

    // Gecersiz harf
    DersNotu n5 = std::string("XY");
    bool n5_hata = false;
    try { not_dogrula(n5); } catch (const GecersizNot&) { n5_hata = true; }
    CHECK(n5_hata, "Harf 'XY' -> GecersizNot exception");
}

void testRangesSorgulama() {
    std::cout << "\n--- testRangesSorgulama ---\n";
    OgrenciDepo depo;
    depo.ekle({"A", 1, "Bil", 90.0});
    depo.ekle({"B", 2, "Elk", 60.0});
    depo.ekle({"C", 3, "Bil", 85.0});
    depo.ekle({"D", 4, "Mak", 45.0});
    depo.ekle({"E", 5, "Bil", 72.0});

    // filter: ortalama >= 75
    const auto& tum = depo.tumunu_al();
    std::vector<std::string> basarili;
    for (const auto& ogr : tum | std::views::values
                                | std::views::filter([](const Ogrenci& o) {
                                    return o.ortalama >= 75.0;
                                  })) {
        basarili.push_back(ogr.isim);
    }
    CHECK(basarili.size() == 2, "Filter: 2 ogrenci >= 75");
    CHECK(basarili[0] == "A" && basarili[1] == "C",
          "Filter sonucu: A ve C");

    // transform: isimleri topla
    std::vector<std::string> isimler;
    for (auto&& isim : tum | std::views::values
                            | std::views::transform(&Ogrenci::isim)) {
        isimler.push_back(isim);
    }
    CHECK(isimler.size() == 5, "Transform: 5 isim dondu");

    // take: ilk 3 kayit
    std::vector<int> ilk_uc;
    for (const auto& [numara, ogr] : tum | std::views::take(3)) {
        ilk_uc.push_back(numara);
    }
    CHECK(ilk_uc.size() == 3, "Take(3): 3 kayit alindi");
    CHECK(ilk_uc[0] == 1 && ilk_uc[1] == 2 && ilk_uc[2] == 3,
          "Take(3) sonucu: 1, 2, 3");
}

// --- Ana Program ---

int main() {
    std::cout << "===== OBYS Birim Testleri =====\n";

    testOgrenciEkleme();
    testOgrenciBulma();
    testKenarDurumlari();
    testHataDurumlari();
    testNotTurleri();
    testRangesSorgulama();

    // Ozet
    std::cout << "\n===== Test Ozeti =====\n";
    std::cout << std::format("Toplam  : {}\n", toplam_test);
    std::cout << std::format("Basarili: {}\n", basarili_test);
    std::cout << std::format("Basarisiz: {}\n", basarisiz_test);

    if (basarisiz_test == 0) {
        std::cout << "Tum testler basarili!\n";
    } else {
        std::cout << "Bazi testler basarisiz oldu.\n";
    }

    return basarisiz_test;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * ===== OBYS Birim Testleri =====
 *
 * --- testOgrenciEkleme ---
 *   [+] Bir ogrenci eklendi, boyut = 1
 *   [+] Ikinci ogrenci eklendi, boyut = 2
 *   [+] Eklenen ogrenci bulunabiliyor
 *   [+] Isim dogru kaydedildi
 *   [+] Numara dogru kaydedildi
 *
 * --- testOgrenciBulma ---
 *   [+] Var olan ogrenci bulundu
 *   [+] Bulunan ogrenci dogru
 *   [+] Olmayan ogrenci -> nullopt
 *
 * --- testKenarDurumlari ---
 *   [+] Bos depo boyutu = 0
 *   [+] Bos depoda arama -> nullopt
 *   [+] Numara 0 gecerli ve bulunabiliyor
 *   [+] Buyuk numara gecerli ve bulunabiliyor
 *
 * --- testHataDurumlari ---
 *   [+] Tekrar numara -> TekrarNumara exception
 *   [+] Olmayan sil -> OgrenciBulunamadi exception
 *
 * --- testNotTurleri ---
 *   [+] Puan 85 gecerli
 *   [+] Harf 'AA' gecerli
 *   [+] Muaf her zaman gecerli
 *   [+] Puan 150 -> GecersizNot exception
 *   [+] Harf 'XY' -> GecersizNot exception
 *
 * --- testRangesSorgulama ---
 *   [+] Filter: 2 ogrenci >= 75
 *   [+] Filter sonucu: A ve C
 *   [+] Transform: 5 isim dondu
 *   [+] Take(3): 3 kayit alindi
 *   [+] Take(3) sonucu: 1, 2, 3
 *
 * ===== Test Ozeti =====
 * Toplam  : 24
 * Basarili: 24
 * Basarisiz: 0
 * Tum testler basarili!
 */

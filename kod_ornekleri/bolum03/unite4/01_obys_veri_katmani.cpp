/**
 * OBYS Veri Katmani - map, variant ve Exception ile Yeniden Tasarim
 *
 * Bu program, Ogrenci Bilgi Yonetim Sistemi'nin veri katmanini
 * modern C++ yapilariyla yeniden tasarlar. Eski vector tabanli
 * yaklasim yerine map ile O(log n) erisim, variant ile esnek
 * not tipleri ve ozel exception siniflarini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - std::map ile anahtar-deger tabanli depolama
 * - std::variant ile tip-guvenli birlestirme (union)
 * - std::optional ile guveli deger donusu
 * - std::visit ile variant uzerinde islem
 * - Ozel exception hiyerarsisi tasarimi
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 4 - OBYS STL Donusumu - Mini Proje
 *
 * Derleme: g++ -std=c++20 -o 01_obys_veri_katmani 01_obys_veri_katmani.cpp
 * Calistirma: ./01_obys_veri_katmani
 */

#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <variant>
#include <stdexcept>
#include <format>
#include <vector>
#include <algorithm>

// --- Ozel Exception Hiyerarsisi ---

class OBYSHatasi : public std::runtime_error {
public:
    explicit OBYSHatasi(const std::string& mesaj)
        : std::runtime_error(mesaj) {}
};

class OgrenciBulunamadi : public OBYSHatasi {
public:
    explicit OgrenciBulunamadi(int numara)
        : OBYSHatasi(std::format("Ogrenci bulunamadi: {}", numara)) {}
};

class GecersizNot : public OBYSHatasi {
public:
    explicit GecersizNot(const std::string& detay)
        : OBYSHatasi(std::format("Gecersiz not: {}", detay)) {}
};

class TekrarNumara : public OBYSHatasi {
public:
    explicit TekrarNumara(int numara)
        : OBYSHatasi(std::format("Bu numara zaten kayitli: {}", numara)) {}
};

// --- Veri Yapilari ---

// Muaf (bos etiket tipi - variant icinde "muaf" durumu temsil eder)
struct Muaf {};

// Not tipi: tamsayi (0-100), harf (AA, BA, ...) veya Muaf
using DersNotu = std::variant<int, std::string, Muaf>;

struct Ogrenci {
    std::string isim;
    int numara;
    std::string bolum;
    double ortalama;
};

// --- Veri Katmani: OgrenciDepo ---

class OgrenciDepo {
    std::map<int, Ogrenci> depom;   // numara -> Ogrenci

public:
    // Ogrenci ekle (tekrar numara kontrolu)
    void ekle(const Ogrenci& ogr) {
        if (depom.contains(ogr.numara)) {
            throw TekrarNumara(ogr.numara);
        }
        depom.emplace(ogr.numara, ogr);
    }

    // Numara ile bul (optional donus)
    std::optional<Ogrenci> bul(int numara) const {
        auto it = depom.find(numara);       // O(log n) arama
        if (it != depom.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    // Sil (bulunamazsa exception)
    void sil(int numara) {
        if (depom.erase(numara) == 0) {
            throw OgrenciBulunamadi(numara);
        }
    }

    // Guncelle (bulunamazsa exception)
    void guncelle(int numara, const Ogrenci& yeni) {
        auto it = depom.find(numara);
        if (it == depom.end()) {
            throw OgrenciBulunamadi(numara);
        }
        it->second = yeni;
    }

    // Toplam kayit sayisi
    std::size_t boyut() const { return depom.size(); }

    // Tum kayitlara erisim (const)
    const std::map<int, Ogrenci>& tumunu_al() const { return depom; }
};

// --- Not Islemleri ---

// Gecerlilik kontrolu
void not_dogrula(const DersNotu& not_degeri) {
    std::visit([](const auto& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
            if (val < 0 || val > 100) {
                throw GecersizNot(std::format("Puan 0-100 arasi olmali: {}", val));
            }
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::vector<std::string> gecerli = {"AA","BA","BB","CB","CC","DC","DD","FF"};
            if (std::ranges::find(gecerli, val) == gecerli.end()) {
                throw GecersizNot(std::format("Gecersiz harf notu: {}", val));
            }
        }
        // Muaf her zaman gecerli
    }, not_degeri);
}

// Not gosterimi
std::string not_goster(const DersNotu& not_degeri) {
    return std::visit([](const auto& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
            return std::format("Puan: {}", val);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return std::format("Harf: {}", val);
        } else {
            return "Muaf";
        }
    }, not_degeri);
}

// --- Demo ---

int main() {
    OgrenciDepo depo;

    // 1) Ogrenci ekleme
    std::cout << "=== Ogrenci Ekleme ===\n";
    depo.ekle({"Ayse Yilmaz",  1001, "Bilgisayar", 85.5});
    depo.ekle({"Mehmet Demir",  1002, "Elektronik", 72.0});
    depo.ekle({"Zeynep Kara",   1003, "Bilgisayar", 91.3});
    depo.ekle({"Ali Ozturk",    1004, "Makine",     68.4});
    std::cout << std::format("Toplam: {} ogrenci eklendi.\n", depo.boyut());

    // 2) map tabanli O(log n) arama
    std::cout << "\n=== Ogrenci Arama (map O(log n)) ===\n";
    if (auto sonuc = depo.bul(1003); sonuc.has_value()) {
        const auto& o = sonuc.value();
        std::cout << std::format("Bulundu: {} (No: {}, Bolum: {})\n",
                                 o.isim, o.numara, o.bolum);
    }
    if (auto sonuc = depo.bul(9999); !sonuc.has_value()) {
        std::cout << "9999 numarali ogrenci bulunamadi (optional bos).\n";
    }

    // 3) variant tabanli not islemleri
    std::cout << "\n=== Not Turleri (variant) ===\n";
    DersNotu n1 = 85;
    DersNotu n2 = std::string("BA");
    DersNotu n3 = Muaf{};

    not_dogrula(n1);
    not_dogrula(n2);
    not_dogrula(n3);

    std::cout << std::format("Not 1: {}\n", not_goster(n1));
    std::cout << std::format("Not 2: {}\n", not_goster(n2));
    std::cout << std::format("Not 3: {}\n", not_goster(n3));

    // 4) Guncelleme
    std::cout << "\n=== Ogrenci Guncelleme ===\n";
    depo.guncelle(1002, {"Mehmet Demir", 1002, "Elektronik", 78.5});
    auto g = depo.bul(1002);
    std::cout << std::format("{} yeni ortalama: {:.1f}\n",
                             g->isim, g->ortalama);

    // 5) Exception senaryolari
    std::cout << "\n=== Hata Yonetimi ===\n";
    try {
        depo.ekle({"Kopya Kayit", 1001, "Test", 50.0});
    } catch (const TekrarNumara& e) {
        std::cout << std::format("[HATA] {}\n", e.what());
    }

    try {
        DersNotu gecersiz = -10;
        not_dogrula(gecersiz);
    } catch (const GecersizNot& e) {
        std::cout << std::format("[HATA] {}\n", e.what());
    }

    try {
        depo.sil(8888);
    } catch (const OgrenciBulunamadi& e) {
        std::cout << std::format("[HATA] {}\n", e.what());
    }

    // 6) Tum kayitlar
    std::cout << "\n=== Tum Kayitlar (map sirali) ===\n";
    for (const auto& [numara, ogr] : depo.tumunu_al()) {
        std::cout << std::format("  {} - {} ({}, Ort: {:.1f})\n",
                                 numara, ogr.isim, ogr.bolum, ogr.ortalama);
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Ogrenci Ekleme ===
 * Toplam: 4 ogrenci eklendi.
 *
 * === Ogrenci Arama (map O(log n)) ===
 * Bulundu: Zeynep Kara (No: 1003, Bolum: Bilgisayar)
 * 9999 numarali ogrenci bulunamadi (optional bos).
 *
 * === Not Turleri (variant) ===
 * Not 1: Puan: 85
 * Not 2: Harf: BA
 * Not 3: Muaf
 *
 * === Ogrenci Guncelleme ===
 * Mehmet Demir yeni ortalama: 78.5
 *
 * === Hata Yonetimi ===
 * [HATA] Bu numara zaten kayitli: 1001
 * [HATA] Gecersiz not: Puan 0-100 arasi olmali: -10
 * [HATA] Ogrenci bulunamadi: 8888
 *
 * === Tum Kayitlar (map sirali) ===
 *   1001 - Ayse Yilmaz (Bilgisayar, Ort: 85.5)
 *   1002 - Mehmet Demir (Elektronik, Ort: 78.5)
 *   1003 - Zeynep Kara (Bilgisayar, Ort: 91.3)
 *   1004 - Ali Ozturk (Makine, Ort: 68.4)
 */

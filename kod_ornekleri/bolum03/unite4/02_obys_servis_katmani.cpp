/**
 * OBYS Servis Katmani - Ranges Pipeline ve format ile Sorgulama
 *
 * Bu program, OBYS'nin servis katmanini olusturur. Veri katmani
 * uzerinde ranges pipeline, views::filter, views::transform ve
 * string_view tabanli arama islemlerini gosterir. Tum ciktilar
 * std::format ile biclmlendirilmistir.
 *
 * Ogrenilecek kavramlar:
 * - ranges::find_if ile arama
 * - views::filter | views::transform pipeline zincirleme
 * - string_view ile sifir kopyali parametre gecisi
 * - std::format ile bicimli cikti
 * - Servis katmaninda exception yonetimi
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 4 - OBYS STL Donusumu - Mini Proje
 *
 * Derleme: g++ -std=c++20 -o 02_obys_servis_katmani 02_obys_servis_katmani.cpp
 * Calistirma: ./02_obys_servis_katmani
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
#include <numeric>

// --- Veri Yapilari (veri katmanindan) ---

struct Muaf {};

using DersNotu = std::variant<int, std::string, Muaf>;

struct Ogrenci {
    std::string isim;
    int numara;
    std::string bolum;
    double ortalama;
};

// --- Basit Exception ---

class OBYSHatasi : public std::runtime_error {
public:
    explicit OBYSHatasi(const std::string& mesaj)
        : std::runtime_error(mesaj) {}
};

// --- Veri Katmani ---

class OgrenciDepo {
    std::map<int, Ogrenci> depom;
public:
    void ekle(const Ogrenci& ogr) {
        depom.emplace(ogr.numara, ogr);
    }

    std::optional<Ogrenci> bul(int numara) const {
        auto it = depom.find(numara);
        if (it != depom.end()) return it->second;
        return std::nullopt;
    }

    const std::map<int, Ogrenci>& tumunu_al() const { return depom; }
    std::size_t boyut() const { return depom.size(); }
};

// --- Servis Katmani ---

class ServisKatmani {
    OgrenciDepo& depo;

public:
    explicit ServisKatmani(OgrenciDepo& d) : depo(d) {}

    // Isim veya bolum icinde kriter gecen ogrencileri ara
    std::vector<Ogrenci> ogrenciAra(std::string_view kriter) const {
        std::vector<Ogrenci> sonuclar;

        for (const auto& [numara, ogr] : depo.tumunu_al()) {
            if (ogr.isim.find(kriter) != std::string::npos ||
                ogr.bolum.find(kriter) != std::string::npos) {
                sonuclar.push_back(ogr);
            }
        }
        return sonuclar;
    }

    // Ortalamasi >= esik olan ogrencilerin isim listesi (pipeline)
    std::vector<std::string> basariliListele(double esik = 75.0) const {
        std::vector<std::string> sonuc;
        const auto& tum = depo.tumunu_al();

        // views::filter | views::transform pipeline
        auto pipeline = tum
            | std::views::values
            | std::views::filter([esik](const Ogrenci& o) {
                return o.ortalama >= esik;
              })
            | std::views::transform([](const Ogrenci& o) {
                return std::format("{} ({:.1f})", o.isim, o.ortalama);
              });

        for (auto&& s : pipeline) {
            sonuc.push_back(s);
        }
        return sonuc;
    }

    // Belirli bir bolumun raporu
    void bolumRaporu(std::string_view bolum) const {
        std::cout << std::format("--- {} Bolumu Raporu ---\n", bolum);

        const auto& tum = depo.tumunu_al();
        int sayac = 0;
        double toplam = 0.0;

        auto bolum_ogrencileri = tum
            | std::views::values
            | std::views::filter([bolum](const Ogrenci& o) {
                return o.bolum == bolum;
              });

        for (const auto& ogr : bolum_ogrencileri) {
            std::cout << std::format("  {} (No: {}, Ort: {:.1f})\n",
                                     ogr.isim, ogr.numara, ogr.ortalama);
            toplam += ogr.ortalama;
            ++sayac;
        }

        if (sayac > 0) {
            std::cout << std::format("  Toplam: {} ogrenci, Ortalama: {:.1f}\n",
                                     sayac, toplam / sayac);
        } else {
            std::cout << "  Bu bolumde ogrenci bulunamadi.\n";
        }
    }

    // Genel istatistik (min, max, ortalama)
    void istatistik() const {
        const auto& tum = depo.tumunu_al();
        if (tum.empty()) {
            std::cout << "Depoda ogrenci yok.\n";
            return;
        }

        // Ortalamalari bir vector'e topla
        std::vector<double> ortalamalar;
        for (const auto& [numara, ogr] : tum) {
            ortalamalar.push_back(ogr.ortalama);
        }

        auto [min_it, max_it] = std::ranges::minmax_element(ortalamalar);
        double toplam = std::accumulate(ortalamalar.begin(),
                                         ortalamalar.end(), 0.0);
        double genel_ort = toplam / static_cast<double>(ortalamalar.size());

        // En basarili ve en dusuk ogrenciyi bul
        // Ortalamalari vector'e aldik, simdi asil ogrenciyi bulalim
        const Ogrenci* en_yuksek_ptr = nullptr;
        const Ogrenci* en_dusuk_ptr = nullptr;
        for (const auto& [numara, ogr] : tum) {
            if (!en_yuksek_ptr || ogr.ortalama > en_yuksek_ptr->ortalama)
                en_yuksek_ptr = &ogr;
            if (!en_dusuk_ptr || ogr.ortalama < en_dusuk_ptr->ortalama)
                en_dusuk_ptr = &ogr;
        }

        std::cout << "=== Genel Istatistikler ===\n";
        std::cout << std::format("  Toplam ogrenci : {}\n", tum.size());
        std::cout << std::format("  Genel ortalama : {:.1f}\n", genel_ort);
        std::cout << std::format("  En yuksek      : {} ({:.1f})\n",
                                 en_yuksek_ptr->isim, en_yuksek_ptr->ortalama);
        std::cout << std::format("  En dusuk       : {} ({:.1f})\n",
                                 en_dusuk_ptr->isim, en_dusuk_ptr->ortalama);
    }
};

// --- Demo ---

int main() {
    // Veri katmanini hazirla
    OgrenciDepo depo;
    depo.ekle({"Ayse Yilmaz",    1001, "Bilgisayar", 85.5});
    depo.ekle({"Mehmet Demir",    1002, "Elektronik", 72.0});
    depo.ekle({"Zeynep Kara",     1003, "Bilgisayar", 91.3});
    depo.ekle({"Ali Ozturk",      1004, "Makine",     68.4});
    depo.ekle({"Fatma Celik",     1005, "Bilgisayar", 78.9});
    depo.ekle({"Burak Sahin",     1006, "Elektronik", 55.2});
    depo.ekle({"Elif Aksoy",      1007, "Makine",     82.7});
    depo.ekle({"Cem Yildiz",      1008, "Bilgisayar", 94.1});

    ServisKatmani servis(depo);

    // 1) Arama
    std::cout << "=== Ogrenci Arama: 'Bilgisayar' ===\n";
    auto sonuclar = servis.ogrenciAra("Bilgisayar");
    for (const auto& ogr : sonuclar) {
        std::cout << std::format("  {} (No: {})\n", ogr.isim, ogr.numara);
    }

    // 2) Basarili listesi (pipeline)
    std::cout << "\n=== Basarili Ogrenciler (Ort >= 75) ===\n";
    auto basarili = servis.basariliListele(75.0);
    for (const auto& s : basarili) {
        std::cout << std::format("  {}\n", s);
    }

    // 3) Bolum raporu
    std::cout << "\n";
    servis.bolumRaporu("Bilgisayar");

    std::cout << "\n";
    servis.bolumRaporu("Elektronik");

    // 4) Genel istatistik
    std::cout << "\n";
    servis.istatistik();

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Ogrenci Arama: 'Bilgisayar' ===
 *   Ayse Yilmaz (No: 1001)
 *   Zeynep Kara (No: 1003)
 *   Fatma Celik (No: 1005)
 *   Cem Yildiz (No: 1008)
 *
 * === Basarili Ogrenciler (Ort >= 75) ===
 *   Ayse Yilmaz (85.5)
 *   Zeynep Kara (91.3)
 *   Fatma Celik (78.9)
 *   Elif Aksoy (82.7)
 *   Cem Yildiz (94.1)
 *
 * --- Bilgisayar Bolumu Raporu ---
 *   Ayse Yilmaz (No: 1001, Ort: 85.5)
 *   Zeynep Kara (No: 1003, Ort: 91.3)
 *   Fatma Celik (No: 1005, Ort: 78.9)
 *   Cem Yildiz (No: 1008, Ort: 94.1)
 *   Toplam: 4 ogrenci, Ortalama: 87.5
 *
 * --- Elektronik Bolumu Raporu ---
 *   Mehmet Demir (No: 1002, Ort: 72.0)
 *   Burak Sahin (No: 1006, Ort: 55.2)
 *   Toplam: 2 ogrenci, Ortalama: 63.6
 *
 * === Genel Istatistikler ===
 *   Toplam ogrenci : 8
 *   Genel ortalama : 78.5
 *   En yuksek      : Cem Yildiz (94.1)
 *   En dusuk       : Burak Sahin (55.2)
 */

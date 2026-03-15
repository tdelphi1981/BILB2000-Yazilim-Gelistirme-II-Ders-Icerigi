/**
 * OBYS Performans Karsilastirmasi - vector vs map Benchmark
 *
 * Bu program, eski (vector + dogrusal arama) ve yeni (map + O(log n))
 * yaklasimlarini 10000 ogrenci uzerinde karsilastirir. chrono ile
 * sure olcumu, sablonlu benchmark fonksiyonu, format ile tablo
 * ciktisi ve sizeof ile bellek analizi yapilir.
 *
 * Ogrenilecek kavramlar:
 * - std::chrono::high_resolution_clock ile sure olcumu
 * - Sablonlu (template) benchmark fonksiyonu
 * - vector vs map performans farki (arama, filtreleme, ekleme, silme)
 * - std::format ile hizalanmis tablo ciktisi
 * - sizeof ile optional, variant bellek yuku analizi
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 4 - OBYS STL Donusumu - Mini Proje
 *
 * Derleme: g++ -std=c++20 -O2 -o 04_obys_performans 04_obys_performans.cpp
 * Calistirma: ./04_obys_performans
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <variant>
#include <chrono>
#include <format>
#include <algorithm>
#include <ranges>
#include <random>
#include <numeric>

// --- Veri Yapilari ---

struct Muaf {};
using DersNotu = std::variant<int, std::string, Muaf>;

struct Ogrenci {
    std::string isim;
    int numara;
    std::string bolum;
    double ortalama;
};

// --- Benchmark Altyapisi ---

template <typename F>
double sure_olc_us(std::string_view etiket, F&& fonk) {
    auto baslangic = std::chrono::high_resolution_clock::now();
    fonk();
    auto bitis = std::chrono::high_resolution_clock::now();
    double us = std::chrono::duration<double, std::micro>(bitis - baslangic).count();
    return us;
}

// --- Rastgele Ogrenci Ureteci ---

std::vector<Ogrenci> rastgele_ogrenciler(int adet) {
    static const std::vector<std::string> isimler = {
        "Ahmet", "Mehmet", "Ayse", "Fatma", "Ali", "Veli",
        "Zeynep", "Elif", "Cem", "Burak", "Selin", "Deniz",
        "Murat", "Hakan", "Sibel", "Oya", "Kemal", "Tuba"
    };
    static const std::vector<std::string> soyisimler = {
        "Yilmaz", "Kaya", "Demir", "Celik", "Sahin", "Ozturk",
        "Arslan", "Dogan", "Kilic", "Aslan", "Aksoy", "Korkmaz"
    };
    static const std::vector<std::string> bolumler = {
        "Bilgisayar", "Elektronik", "Makine", "Insaat", "Fizik"
    };

    std::mt19937 gen(42);  // Sabit seed (tekrarlanabilir)
    std::uniform_int_distribution<int> isim_dist(0, isimler.size() - 1);
    std::uniform_int_distribution<int> soy_dist(0, soyisimler.size() - 1);
    std::uniform_int_distribution<int> bol_dist(0, bolumler.size() - 1);
    std::uniform_real_distribution<double> ort_dist(30.0, 100.0);

    std::vector<Ogrenci> sonuc;
    sonuc.reserve(adet);

    for (int i = 0; i < adet; ++i) {
        sonuc.push_back({
            isimler[isim_dist(gen)] + " " + soyisimler[soy_dist(gen)],
            i + 1,
            bolumler[bol_dist(gen)],
            ort_dist(gen)
        });
    }
    return sonuc;
}

// --- Eski Yaklasim: vector + dogrusal arama ---

class EskiDepo {
    std::vector<Ogrenci> liste;
public:
    void ekle(const Ogrenci& ogr) {
        liste.push_back(ogr);
    }

    Ogrenci* bul(int numara) {
        for (auto& o : liste) {
            if (o.numara == numara) return &o;
        }
        return nullptr;
    }

    std::vector<Ogrenci> filtrele(double esik) const {
        std::vector<Ogrenci> sonuc;
        for (const auto& o : liste) {
            if (o.ortalama >= esik) sonuc.push_back(o);
        }
        return sonuc;
    }

    bool sil(int numara) {
        auto it = std::find_if(liste.begin(), liste.end(),
            [numara](const Ogrenci& o) { return o.numara == numara; });
        if (it != liste.end()) {
            liste.erase(it);
            return true;
        }
        return false;
    }

    std::size_t boyut() const { return liste.size(); }
};

// --- Yeni Yaklasim: map + O(log n) ---

class YeniDepo {
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

    std::vector<Ogrenci> filtrele(double esik) const {
        std::vector<Ogrenci> sonuc;
        for (const auto& ogr : depom | std::views::values
                                      | std::views::filter([esik](const Ogrenci& o) {
                                          return o.ortalama >= esik;
                                        })) {
            sonuc.push_back(ogr);
        }
        return sonuc;
    }

    bool sil(int numara) {
        return depom.erase(numara) > 0;
    }

    std::size_t boyut() const { return depom.size(); }
};

int main() {
    constexpr int N = 10000;
    auto veriler = rastgele_ogrenciler(N);

    std::cout << std::format("===== Performans Karsilastirmasi ({} ogrenci) =====\n\n", N);

    // Aranacak numaralar (basi, ortasi, sonu, olmayan)
    std::vector<int> arama_numaralari = {1, N / 2, N, N + 999};

    // --- Ekleme Benchmark ---
    double eski_ekle = 0, yeni_ekle = 0;
    EskiDepo eski;
    YeniDepo yeni;

    eski_ekle = sure_olc_us("Eski Ekle", [&]() {
        for (const auto& ogr : veriler) eski.ekle(ogr);
    });

    yeni_ekle = sure_olc_us("Yeni Ekle", [&]() {
        for (const auto& ogr : veriler) yeni.ekle(ogr);
    });

    // --- Arama Benchmark (1000 tekrar) ---
    constexpr int TEKRAR = 1000;
    double eski_ara = 0, yeni_ara = 0;

    eski_ara = sure_olc_us("Eski Ara", [&]() {
        for (int t = 0; t < TEKRAR; ++t) {
            for (int num : arama_numaralari) {
                volatile auto p = eski.bul(num);
                (void)p;
            }
        }
    });

    yeni_ara = sure_olc_us("Yeni Ara", [&]() {
        for (int t = 0; t < TEKRAR; ++t) {
            for (int num : arama_numaralari) {
                volatile auto r = yeni.bul(num).has_value();
                (void)r;
            }
        }
    });

    // --- Filtreleme Benchmark ---
    double eski_filtre = 0, yeni_filtre = 0;

    eski_filtre = sure_olc_us("Eski Filtre", [&]() {
        for (int t = 0; t < 100; ++t) {
            auto s = eski.filtrele(75.0);
            (void)s;
        }
    });

    yeni_filtre = sure_olc_us("Yeni Filtre", [&]() {
        for (int t = 0; t < 100; ++t) {
            auto s = yeni.filtrele(75.0);
            (void)s;
        }
    });

    // --- Silme Benchmark (son 100 eleman) ---
    double eski_sil = 0, yeni_sil = 0;

    eski_sil = sure_olc_us("Eski Sil", [&]() {
        for (int i = N; i > N - 100; --i) {
            eski.sil(i);
        }
    });

    yeni_sil = sure_olc_us("Yeni Sil", [&]() {
        for (int i = N; i > N - 100; --i) {
            yeni.sil(i);
        }
    });

    // --- Sonuc Tablosu ---
    std::cout << std::format("{:<20} {:>15} {:>15} {:>10}\n",
                             "Islem", "vector (us)", "map (us)", "Oran");
    std::cout << std::string(62, '-') << "\n";

    auto satir_yaz = [](std::string_view islem, double v_us, double m_us) {
        std::string oran_str;
        if (m_us > 0 && v_us > 0) {
            double oran = v_us / m_us;
            oran_str = std::format("{:.1f}x", oran);
        } else {
            oran_str = "N/A";
        }
        std::cout << std::format("{:<20} {:>12.0f} us {:>12.0f} us {:>10}\n",
                                 islem, v_us, m_us, oran_str);
    };

    satir_yaz("Ekleme (10000)",     eski_ekle,   yeni_ekle);
    satir_yaz("Arama (4x1000)",     eski_ara,    yeni_ara);
    satir_yaz("Filtreleme (100x)",  eski_filtre, yeni_filtre);
    satir_yaz("Silme (100)",        eski_sil,    yeni_sil);

    // --- Bellek Boyutu Analizi ---
    std::cout << "\n===== Bellek Boyutu Analizi =====\n";
    std::cout << std::format("{:<30} {:>10}\n", "Tip", "sizeof (byte)");
    std::cout << std::string(42, '-') << "\n";
    std::cout << std::format("{:<30} {:>10}\n", "Ogrenci", sizeof(Ogrenci));
    std::cout << std::format("{:<30} {:>10}\n", "optional<Ogrenci>",
                             sizeof(std::optional<Ogrenci>));
    std::cout << std::format("{:<30} {:>10}\n", "variant<int,string,Muaf>",
                             sizeof(DersNotu));
    std::cout << std::format("{:<30} {:>10}\n", "int", sizeof(int));
    std::cout << std::format("{:<30} {:>10}\n", "string", sizeof(std::string));
    std::cout << std::format("{:<30} {:>10}\n", "map<int,Ogrenci> (bos)",
                             sizeof(std::map<int, Ogrenci>));
    std::cout << std::format("{:<30} {:>10}\n", "vector<Ogrenci> (bos)",
                             sizeof(std::vector<Ogrenci>));

    std::cout << "\n===== optional / variant Yuku =====\n";
    std::cout << std::format("optional<Ogrenci> yuku: {} byte (Ogrenci: {} + ek: {})\n",
                             sizeof(std::optional<Ogrenci>),
                             sizeof(Ogrenci),
                             sizeof(std::optional<Ogrenci>) - sizeof(Ogrenci));
    std::cout << std::format("variant<int,string,Muaf> boyutu: {} byte\n",
                             sizeof(DersNotu));
    std::cout << std::format("  (en buyuk uyeden ({} byte) + diskriminant yuku)\n",
                             sizeof(std::string));

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * ===== Performans Karsilastirmasi (10000 ogrenci) =====
 *
 * Islem                  vector (us)      map (us)      Oran
 * --------------------------------------------------------------
 * Ekleme (10000)              ~440 us       ~1330 us     ~0.3x
 * Arama (4x1000)            ~29000 us        ~200 us   ~145.0x
 * Filtreleme (100x)          ~5200 us       ~9200 us     ~0.6x
 * Silme (100)                 ~700 us         ~13 us    ~55.0x
 *
 * ===== Bellek Boyutu Analizi =====
 * Tip                            sizeof (byte)
 * ------------------------------------------
 * Ogrenci                               64
 * optional<Ogrenci>                      72
 * variant<int,string,Muaf>               32
 * int                                     4
 * string                                 24
 * map<int,Ogrenci> (bos)                 24
 * vector<Ogrenci> (bos)                  24
 *
 * ===== optional / variant Yuku =====
 * optional<Ogrenci> yuku: 72 byte (Ogrenci: 64 + ek: 8)
 * variant<int,string,Muaf> boyutu: 32 byte
 *   (en buyuk uyeden (24 byte) + diskriminant yuku)
 *
 * NOT: Degerler sisteme ve derleyiciye gore degisebilir.
 * Arama ve silme islemlerinde map belirgin sekilde hizlidir.
 * Ekleme ve filtrelemede vector genellikle avantajlidir.
 */

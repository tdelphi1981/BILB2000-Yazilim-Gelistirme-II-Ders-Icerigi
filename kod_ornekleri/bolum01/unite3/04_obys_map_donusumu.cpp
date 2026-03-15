/**
 * OBYS Depo Donusumu - vector'den map'e Gecis
 *
 * Bu program OBYS projesindeki Depo<T> sablonunun vector'den
 * map'e donusumunu gosterir. Her iki yaklasimin performans
 * farkini ve map'in avantajlarini ortaya koyar.
 *
 * Ogrenilecek kavramlar:
 * - Template sinif tasarimi
 * - vector vs map performans karsilastirmasi
 * - shared_ptr ile nesne yonetimi
 * - map tabanli CRUD islemleri
 * - set ile benzersizlik kontrolu
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 3 - Iliskisel Kapsayicilar
 *
 * Derleme: g++ -std=c++20 -o 04_obys_donusum 04_obys_map_donusumu.cpp
 * Calistirma: ./04_obys_donusum
 */

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <memory>
#include <vector>

// Basit Ogrenci sinifi
struct Ogrenci {
    int numara;
    std::string isim;
    double gpa;

    Ogrenci(int no, const std::string& ad, double puan)
        : numara(no), isim(ad), gpa(puan) {}

    int getId() const { return numara; }
};

// --- ESKI: vector tabanli Depo ---
template <typename T>
class VectorDepo {
    std::vector<std::shared_ptr<T>> veriler;

public:
    void ekle(std::shared_ptr<T> nesne) {
        veriler.push_back(nesne);
    }

    // O(n) arama - yavas!
    std::shared_ptr<T> bul(int id) const {
        for (const auto& v : veriler) {
            if (v->getId() == id) return v;
        }
        return nullptr;
    }

    void sil(int id) {
        veriler.erase(
            std::remove_if(veriler.begin(), veriler.end(),
                [id](const auto& v) { return v->getId() == id; }),
            veriler.end()
        );
    }

    size_t boyut() const { return veriler.size(); }

    void listele() const {
        for (const auto& v : veriler) {
            std::cout << "  " << v->numara << " - "
                      << v->isim << " (GPA: " << v->gpa << ")\n";
        }
    }
};

// --- YENI: map tabanli Depo ---
template <typename T>
class MapDepo {
    std::map<int, std::shared_ptr<T>> veriler;

public:
    bool ekle(std::shared_ptr<T> nesne) {
        auto [it, eklendi] = veriler.emplace(
            nesne->getId(), nesne);
        return eklendi;
    }

    // O(log n) arama - hizli!
    std::shared_ptr<T> bul(int id) const {
        auto it = veriler.find(id);
        if (it != veriler.end()) return it->second;
        return nullptr;
    }

    bool sil(int id) {
        return veriler.erase(id) > 0;
    }

    size_t boyut() const { return veriler.size(); }

    // Sirali listeleme (map otomatik siralar)
    void listele() const {
        for (const auto& [no, ogr] : veriler) {
            std::cout << "  " << no << " - "
                      << ogr->isim << " (GPA: " << ogr->gpa << ")\n";
        }
    }

    // GPA'ya gore filtreleme
    std::vector<std::shared_ptr<T>> filtrele(double min_gpa) const {
        std::vector<std::shared_ptr<T>> sonuc;
        for (const auto& [no, ogr] : veriler) {
            if (ogr->gpa >= min_gpa) {
                sonuc.push_back(ogr);
            }
        }
        return sonuc;
    }
};

int main() {
    // --- 1. Eski Yontem: VectorDepo ---
    std::cout << "=== Eski Yontem: VectorDepo ===\n\n";

    VectorDepo<Ogrenci> eski_depo;
    eski_depo.ekle(std::make_shared<Ogrenci>(2003, "Mehmet Demir", 2.95));
    eski_depo.ekle(std::make_shared<Ogrenci>(2001, "Ali Yilmaz", 3.45));
    eski_depo.ekle(std::make_shared<Ogrenci>(2002, "Ayse Kaya", 3.78));

    eski_depo.listele();

    auto bulunan = eski_depo.bul(2001);
    if (bulunan) {
        std::cout << "\n  Bulunan: " << bulunan->isim << "\n";
    }

    // --- 2. Yeni Yontem: MapDepo ---
    std::cout << "\n=== Yeni Yontem: MapDepo ===\n\n";

    MapDepo<Ogrenci> yeni_depo;
    yeni_depo.ekle(std::make_shared<Ogrenci>(2003, "Mehmet Demir", 2.95));
    yeni_depo.ekle(std::make_shared<Ogrenci>(2001, "Ali Yilmaz", 3.45));
    yeni_depo.ekle(std::make_shared<Ogrenci>(2002, "Ayse Kaya", 3.78));
    yeni_depo.ekle(std::make_shared<Ogrenci>(2004, "Zeynep Celik", 3.92));
    yeni_depo.ekle(std::make_shared<Ogrenci>(2005, "Fatma Sahin", 3.10));

    std::cout << "Tum ogrenciler (sirali!):\n";
    yeni_depo.listele();

    // Arama
    auto sonuc = yeni_depo.bul(2002);
    if (sonuc) {
        std::cout << "\n  O(log n) arama: " << sonuc->isim << "\n";
    }

    // Tekrar ekleme kontrolu
    bool tekrar = yeni_depo.ekle(
        std::make_shared<Ogrenci>(2001, "Ali Yilmaz", 3.45));
    std::cout << "  2001 tekrar ekleme: "
              << (tekrar ? "Basarili" : "Zaten mevcut") << "\n";

    // --- 3. Filtreleme ---
    std::cout << "\n=== Onur Listesi (GPA >= 3.5) ===\n\n";

    auto onur = yeni_depo.filtrele(3.5);
    for (const auto& ogr : onur) {
        std::cout << "  " << ogr->isim << " - GPA: " << ogr->gpa << "\n";
    }

    // --- 4. Silme ---
    std::cout << "\n=== Silme Islemi ===\n\n";

    std::cout << "  Onceki boyut: " << yeni_depo.boyut() << "\n";
    bool silindi = yeni_depo.sil(2003);
    std::cout << "  2003 silindi: " << std::boolalpha << silindi << "\n";
    std::cout << "  Yeni boyut: " << yeni_depo.boyut() << "\n";

    // --- 5. Benzersiz Ders Kodlari (set) ---
    std::cout << "\n=== Benzersiz Ders Kodlari (set) ===\n\n";

    std::set<std::string> ders_kodlari;
    ders_kodlari.insert("MAT101");
    ders_kodlari.insert("FIZ102");
    ders_kodlari.insert("BIL201");
    ders_kodlari.insert("MAT101"); // tekrar - eklenmez

    std::cout << "  Kayitli dersler (" << ders_kodlari.size() << "):\n";
    for (const auto& kod : ders_kodlari) {
        std::cout << "    " << kod << "\n";
    }

    // --- 6. Performans Notu ---
    std::cout << "\n=== Performans Karsilastirmasi ===\n\n";
    std::cout << "  100.000 kayit icin:\n";
    std::cout << "  vector arama: ortalama 50.000 karsilastirma\n";
    std::cout << "  map arama:    en fazla 17 karsilastirma\n";
    std::cout << "  Kazanc:       ~2941x daha hizli!\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ----------------
 * === Eski Yontem: VectorDepo ===
 *
 *   2003 - Mehmet Demir (GPA: 2.95)
 *   2001 - Ali Yilmaz (GPA: 3.45)
 *   2002 - Ayse Kaya (GPA: 3.78)
 *
 *   Bulunan: Ali Yilmaz
 *
 * === Yeni Yontem: MapDepo ===
 *
 * Tum ogrenciler (sirali!):
 *   2001 - Ali Yilmaz (GPA: 3.45)
 *   2002 - Ayse Kaya (GPA: 3.78)
 *   2003 - Mehmet Demir (GPA: 2.95)
 *   2004 - Zeynep Celik (GPA: 3.92)
 *   2005 - Fatma Sahin (GPA: 3.1)
 *
 *   O(log n) arama: Ayse Kaya
 *   2001 tekrar ekleme: Zaten mevcut
 *
 * === Onur Listesi (GPA >= 3.5) ===
 *
 *   Ayse Kaya - GPA: 3.78
 *   Zeynep Celik - GPA: 3.92
 *
 * === Silme Islemi ===
 *
 *   Onceki boyut: 5
 *   2003 silindi: true
 *   Yeni boyut: 4
 *
 * === Benzersiz Ders Kodlari (set) ===
 *
 *   Kayitli dersler (3):
 *     BIL201
 *     FIZ102
 *     MAT101
 *
 * === Performans Karsilastirmasi ===
 *
 *   100.000 kayit icin:
 *   vector arama: ortalama 50.000 karsilastirma
 *   map arama:    en fazla 17 karsilastirma
 *   Kazanc:       ~2941x daha hizli!
 */

/**
 * OBYS Depo<T> - vector Tabanli Depo Sinifi
 *
 * Bu program OBYS projesindeki Depo<T> sinifinin
 * vector<shared_ptr<T>> ile nasil calistigini gosterir.
 * Ekleme, arama ve listeleme operasyonlarini icermektedir.
 *
 * Ogrenilecek kavramlar:
 * - Template sinif ile generic depo tasarimi
 * - vector<shared_ptr<T>> kullanimi
 * - emplace_back ile yerinde insa
 * - Lineer arama ve sinirlamalari
 * - move semantigi ile verimli ekleme
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 2 - Sirali Kapsayicilar
 *
 * Derleme: g++ -std=c++20 -o 04_obys_depo_vector 04_obys_depo_vector.cpp
 * Calistirma: ./04_obys_depo_vector
 */

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

// Temel varlik sinifi
class Varlik {
protected:
    int id;
public:
    explicit Varlik(int id) : id(id) {}
    virtual ~Varlik() = default;
    int getId() const { return id; }
    virtual void yazdir() const = 0;
};

// Ogrenci sinifi
class Ogrenci : public Varlik {
    std::string isim;
    double ortalama;
public:
    Ogrenci(int numara, std::string isim, double ort)
        : Varlik(numara), isim(std::move(isim)), ortalama(ort) {}

    const std::string& getIsim() const { return isim; }
    double getOrtalama() const { return ortalama; }

    void yazdir() const override {
        std::cout << "  [" << id << "] " << isim
                  << " (Ort: " << ortalama << ")" << '\n';
    }
};

// Generic Depo sinifi - vector<shared_ptr<T>> tabanli
template<typename T>
class Depo {
    std::vector<std::shared_ptr<T>> elemanlar;

public:
    // Eleman ekle (move semantigi ile)
    void ekle(std::shared_ptr<T> eleman) {
        elemanlar.push_back(std::move(eleman));  // O(1) amortize
    }

    // ID ile ara (lineer tarama - O(n))
    std::shared_ptr<T> bul(int id) const {
        for (const auto& e : elemanlar) {
            if (e->getId() == id) return e;
        }
        return nullptr;
    }

    // Tum elemanlari listele
    void listele() const {
        for (const auto& e : elemanlar) {
            e->yazdir();
        }
    }

    // Eleman sayisi
    size_t boyut() const { return elemanlar.size(); }

    // Kapasite bilgisi
    size_t kapasite() const { return elemanlar.capacity(); }

    // Onceden kapasite ayir
    void rezerveEt(size_t n) { elemanlar.reserve(n); }

    // ID ile sil (O(n) - kaydirma gerekir)
    bool sil(int id) {
        auto it = std::find_if(elemanlar.begin(), elemanlar.end(),
            [id](const std::shared_ptr<T>& e) {
                return e->getId() == id;
            });

        if (it != elemanlar.end()) {
            elemanlar.erase(it);
            return true;
        }
        return false;
    }

    // Kosullu filtreleme
    std::vector<std::shared_ptr<T>> filtrele(
        std::function<bool(const std::shared_ptr<T>&)> kosul) const {
        std::vector<std::shared_ptr<T>> sonuc;
        for (const auto& e : elemanlar) {
            if (kosul(e)) {
                sonuc.push_back(e);
            }
        }
        return sonuc;
    }
};

int main() {
    std::cout << "=== OBYS Depo<Ogrenci> Ornegi ===" << '\n' << '\n';

    Depo<Ogrenci> ogrenciDepo;

    // Onceden kapasite ayir
    ogrenciDepo.rezerveEt(10);
    std::cout << "Baslangic kapasite: " << ogrenciDepo.kapasite() << '\n' << '\n';

    // Ogrenci ekle
    ogrenciDepo.ekle(std::make_shared<Ogrenci>(101, "Ali Yilmaz", 3.45));
    ogrenciDepo.ekle(std::make_shared<Ogrenci>(102, "Ayse Kaya", 3.82));
    ogrenciDepo.ekle(std::make_shared<Ogrenci>(103, "Mehmet Demir", 2.90));
    ogrenciDepo.ekle(std::make_shared<Ogrenci>(104, "Zeynep Arslan", 3.65));
    ogrenciDepo.ekle(std::make_shared<Ogrenci>(105, "Can Ozturk", 2.15));

    std::cout << "Tum ogrenciler (" << ogrenciDepo.boyut() << " kisi):" << '\n';
    ogrenciDepo.listele();
    std::cout << '\n';

    // ID ile arama
    std::cout << "Numara 103 araniyor..." << '\n';
    auto bulunan = ogrenciDepo.bul(103);
    if (bulunan) {
        std::cout << "Bulundu: ";
        bulunan->yazdir();
    }
    std::cout << '\n';

    // Filtreleme: ortalamasi 3.0 uzerinde olanlar
    std::cout << "Ortalamasi 3.0 uzerinde olanlar:" << '\n';
    auto basarililar = ogrenciDepo.filtrele(
        [](const std::shared_ptr<Ogrenci>& o) {
            return o->getOrtalama() >= 3.0;
        });

    for (const auto& o : basarililar) {
        o->yazdir();
    }
    std::cout << '\n';

    // Silme
    std::cout << "Numara 105 siliniyor..." << '\n';
    if (ogrenciDepo.sil(105)) {
        std::cout << "Silme basarili. Kalan: " << ogrenciDepo.boyut() << '\n';
    }
    std::cout << '\n';

    std::cout << "Guncel liste:" << '\n';
    ogrenciDepo.listele();

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== OBYS Depo<Ogrenci> Ornegi ===

Baslangic kapasite: 10

Tum ogrenciler (5 kisi):
  [101] Ali Yilmaz (Ort: 3.45)
  [102] Ayse Kaya (Ort: 3.82)
  [103] Mehmet Demir (Ort: 2.9)
  [104] Zeynep Arslan (Ort: 3.65)
  [105] Can Ozturk (Ort: 2.15)

Numara 103 araniyor...
Bulundu:   [103] Mehmet Demir (Ort: 2.9)

Ortalamasi 3.0 uzerinde olanlar:
  [101] Ali Yilmaz (Ort: 3.45)
  [102] Ayse Kaya (Ort: 3.82)
  [104] Zeynep Arslan (Ort: 3.65)

Numara 105 siliniyor...
Silme basarili. Kalan: 4

Guncel liste:
  [101] Ali Yilmaz (Ort: 3.45)
  [102] Ayse Kaya (Ort: 3.82)
  [103] Mehmet Demir (Ort: 2.9)
  [104] Zeynep Arslan (Ort: 3.65)
*/

/**
 * Batch Import - Birden Fazla Dosyayı Paralel İçe Aktarma
 *
 * Bölüm 11 Ünite 2 Konu 3
 *
 * Bu örnekte:
 *  - jthread + stop_token ile iptal edilebilir batch import
 *  - Her dosya için ayrı sonuç yapısı (DosyaSonuc)
 *  - Hata izolasyonu: bir dosya hatası diğerlerini durdurmaz
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 03_batch 03_batch_import.cpp
 */

#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <sstream>
#include <stop_token>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

struct Ogrenci {
    int numara;
    std::string ad;
    std::string bolum;
};

class Depo {
    mutable std::shared_mutex sm_;
    std::unordered_map<int, std::shared_ptr<Ogrenci>> v_;
public:
    void ekle(int id, std::shared_ptr<Ogrenci> o) {
        std::unique_lock<std::shared_mutex> g(sm_);
        v_[id] = std::move(o);
    }
    std::size_t boyut() const {
        std::shared_lock<std::shared_mutex> g(sm_);
        return v_.size();
    }
};

struct DosyaSonuc {
    std::string yol;
    std::size_t okunanSatir = 0;
    std::size_t basariliKayit = 0;
    std::vector<std::string> hatalar;
    bool tamamlandi = false;
};

struct BatchSonuc {
    std::vector<DosyaSonuc> dosyalar;
    std::size_t toplamKayit = 0;
    std::chrono::milliseconds sure{0};
    bool iptalEdildi = false;
};

Ogrenci parse(const std::string& satir) {
    Ogrenci o;
    std::istringstream iss(satir);
    std::string alan;
    std::getline(iss, alan, ','); o.numara = std::stoi(alan);
    std::getline(iss, o.ad, ',');
    std::getline(iss, o.bolum, ',');
    return o;
}

void birDosyaAktar(const std::string& yol, Depo& depo,
                    DosyaSonuc& sonuc, std::stop_token st) {
    std::ifstream dosya(yol);
    if (!dosya.is_open()) {
        sonuc.hatalar.push_back("Dosya acilmadi");
        return;
    }
    std::string satir;
    std::getline(dosya, satir);   // başlık atla
    std::size_t satirNo = 1;

    while (std::getline(dosya, satir)) {
        if (st.stop_requested()) {
            sonuc.hatalar.push_back("Iptal edildi");
            return;
        }
        ++satirNo;
        try {
            auto o = parse(satir);
            depo.ekle(o.numara, std::make_shared<Ogrenci>(std::move(o)));
            ++sonuc.basariliKayit;
        } catch (const std::exception& ex) {
            sonuc.hatalar.push_back(
                "Satir " + std::to_string(satirNo) + ": " + ex.what());
        }
        ++sonuc.okunanSatir;
    }
    sonuc.tamamlandi = true;
}

BatchSonuc batchIceAktar(const std::vector<std::string>& yollar,
                         Depo& depo, std::stop_source ss) {
    BatchSonuc sonuc;
    sonuc.dosyalar.resize(yollar.size());
    auto baslangic = std::chrono::steady_clock::now();

    std::vector<std::jthread> isciler;
    for (std::size_t i = 0; i < yollar.size(); ++i) {
        sonuc.dosyalar[i].yol = yollar[i];
        isciler.emplace_back(
            [&, i](std::stop_token st) {
                birDosyaAktar(yollar[i], depo, sonuc.dosyalar[i], st);
            },
            ss.get_token());
    }

    for (auto& js : isciler) js.join();

    sonuc.iptalEdildi = ss.stop_requested();
    for (const auto& d : sonuc.dosyalar) {
        sonuc.toplamKayit += d.basariliKayit;
    }
    sonuc.sure = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - baslangic);
    return sonuc;
}

void testDosyasiOlustur(const std::string& yol, int baslangic, int adet) {
    std::ofstream f(yol);
    f << "numara,ad,bolum\n";
    for (int i = 0; i < adet; ++i) {
        f << (baslangic + i) << ",Ogrenci_" << (baslangic + i)
          << ",Bilgisayar\n";
    }
}

int main() {
    std::vector<std::string> yollar = {
        "/tmp/batch1.csv", "/tmp/batch2.csv",
        "/tmp/batch3.csv", "/tmp/batch4.csv"
    };

    int basla = 100000;
    for (auto& y : yollar) {
        testDosyasiOlustur(y, basla, 5000);
        basla += 5000;
    }

    Depo depo;
    std::stop_source ss;

    auto sonuc = batchIceAktar(yollar, depo, ss);

    std::cout << "Toplam yuklenen: " << sonuc.toplamKayit << '\n';
    std::cout << "Sure: " << sonuc.sure.count() << " ms\n";
    std::cout << "Iptal: " << (sonuc.iptalEdildi ? "Evet" : "Hayir") << "\n\n";

    for (const auto& d : sonuc.dosyalar) {
        std::cout << d.yol << ": " << d.basariliKayit << " kayit, "
                  << d.hatalar.size() << " hata\n";
    }

    std::cout << "\nDepoda toplam: " << depo.boyut() << " ogrenci\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
Toplam yuklenen: 20000
Sure: 100-300 ms
Iptal: Hayir

/tmp/batch1.csv: 5000 kayit, 0 hata
/tmp/batch2.csv: 5000 kayit, 0 hata
/tmp/batch3.csv: 5000 kayit, 0 hata
/tmp/batch4.csv: 5000 kayit, 0 hata

Depoda toplam: 20000 ogrenci
*/

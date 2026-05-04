/**
 * OBYS Asenkron JSON Export
 *
 * Bu program, 10.000 öğrencinin JSON export işlemini arka planda
 * çalıştırır; ana izlek wait_for ile ilerlemeyi yoklar ve UI donması
 * olmadan progress bar güncellemesi simülasyonu yapar.
 *
 * Öğrenilecek kavramlar:
 * - std::async ile asenkron uzun iş
 * - wait_for ile polling deseni
 * - std::atomic sayaçlarıyla ilerleme izleme
 * - İptal bayrağı ile kooperatif iptal
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 2 - std::async ve Future/Promise
 *
 * Derleme: g++ -std=c++20 -pthread -o 05_export 05_obys_async_export.cpp
 * Çalıştırma: ./05_export
 */

#include <future>
#include <atomic>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

struct Ogrenci {
    int numara;
    std::string ad;
    std::string bolum;
    double ortalama;
};

struct ExportSonuc {
    bool basarili;
    std::size_t islenenKayit;
    std::string dosyaYolu;
    std::string hataMesaji;
};

class AsyncExportServisi {
public:
    std::future<ExportSonuc> jsonExport(
        std::vector<Ogrenci> ogrenciler,
        std::string hedefDosya)
    {
        toplam_ = ogrenciler.size();
        islenen_.store(0);
        iptalBayragi_.store(false);
        return std::async(std::launch::async,
            &AsyncExportServisi::exportCalistir, this,
            std::move(ogrenciler), std::move(hedefDosya));
    }

    double yuzde() const {
        if (toplam_ == 0) return 0.0;
        return 100.0 * islenen_.load() / toplam_;
    }

    void iptal() { iptalBayragi_.store(true); }

private:
    ExportSonuc exportCalistir(
        std::vector<Ogrenci> ogrenciler,
        std::string dosya)
    {
        ExportSonuc sonuc;
        sonuc.dosyaYolu = dosya;
        std::ofstream cikti(dosya);
        if (!cikti.is_open()) {
            sonuc.basarili = false;
            sonuc.hataMesaji = "Dosya açılamadı";
            return sonuc;
        }

        cikti << "[\n";
        for (std::size_t i = 0; i < ogrenciler.size(); ++i) {
            if (iptalBayragi_.load()) {
                sonuc.basarili = false;
                sonuc.hataMesaji = "Kullanıcı iptal etti";
                sonuc.islenenKayit = islenen_.load();
                return sonuc;
            }
            const auto& o = ogrenciler[i];
            cikti << "  {"
                  << "\"numara\":" << o.numara
                  << ",\"ad\":\"" << o.ad << "\""
                  << ",\"bolum\":\"" << o.bolum << "\""
                  << ",\"ortalama\":" << o.ortalama
                  << "}"
                  << (i + 1 < ogrenciler.size() ? "," : "")
                  << "\n";
            islenen_.fetch_add(1);
            // Disk yavaşlığı simülasyonu
            if (i % 100 == 0) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(1));
            }
        }
        cikti << "]\n";
        sonuc.basarili = true;
        sonuc.islenenKayit = islenen_.load();
        return sonuc;
    }

    std::atomic<std::size_t> islenen_{0};
    std::atomic<bool> iptalBayragi_{false};
    std::size_t toplam_ = 0;
};

int main() {
    // Test verisi
    std::vector<Ogrenci> ogrenciler;
    for (int i = 0; i < 2000; ++i) {
        ogrenciler.push_back({
            10000 + i,
            "Öğrenci " + std::to_string(i),
            "Bilgisayar",
            2.0 + (i % 10) * 0.2
        });
    }

    AsyncExportServisi servis;
    auto future = servis.jsonExport(std::move(ogrenciler),
                                     "export_test.json");

    // Polling ile UI simülasyonu
    while (future.wait_for(std::chrono::milliseconds(50))
           != std::future_status::ready)
    {
        std::cout << "İlerleme: " << static_cast<int>(servis.yuzde())
                  << "%\r" << std::flush;
    }
    std::cout << "\n";

    auto sonuc = future.get();
    if (sonuc.basarili) {
        std::cout << "Başarılı: " << sonuc.islenenKayit
                  << " kayıt yazıldı -> " << sonuc.dosyaYolu << "\n";
    } else {
        std::cout << "Hata: " << sonuc.hataMesaji << "\n";
    }
    return 0;
}

/*
 * BEKLENEN DAVRANIŞ:
 * ------------------
 * Program çalışırken UI ilerleme yüzdesi gösterir:
 *   İlerleme: 12%
 *   İlerleme: 47%
 *   ...
 * Sonunda "Başarılı: 2000 kayıt yazıldı -> export_test.json"
 */

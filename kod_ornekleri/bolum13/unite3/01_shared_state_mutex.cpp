/**
 * Paylaşılan Durum ve std::shared_mutex - Çoklu Okuyucu, Tek Yazar
 *
 * Bu örnek, OBYS'nin merkezindeki "OgrenciDepo" sınıfının paylaşılan
 * durumunu çoklu thread'den nasıl güvenle erişilebilir kıldığını gösterir.
 * std::shared_mutex sayesinde aynı anda birden fazla okuyucu thread
 * (paralel sorgu) çalışırken yazar thread tek başına kilidi alır.
 *
 * OBYS bağlamında: 3 öğrenci aynı anda not listesini sorgularken,
 * sekreterlik aynı anda yeni bir öğrenci kaydı eklemek isteyebilir.
 *
 * Öğrenilecek kavramlar:
 * - std::shared_mutex
 * - std::shared_lock (okuma) vs std::unique_lock (yazma)
 * - OBYS depo örüntüsü ile thread güvenliği
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 3 - Paylaşılan Durum Yönetimi
 *
 * Derleme: g++ -std=c++20 -pthread -o ornek 01_shared_state_mutex.cpp
 * Çalıştırma: ./ornek
 */

#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

struct Ogrenci {
    std::string numara;
    std::string ad_soyad;
    double gano;
};

class OgrenciDepo {
public:
    void ekle(const Ogrenci& o) {
        std::unique_lock kilit(kilit_);
        depo_[o.numara] = o;
    }

    std::optional<Ogrenci> bul(const std::string& numara) const {
        std::shared_lock kilit(kilit_);
        if (auto it = depo_.find(numara); it != depo_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::size_t boyut() const {
        std::shared_lock kilit(kilit_);
        return depo_.size();
    }

private:
    mutable std::shared_mutex kilit_;
    std::unordered_map<std::string, Ogrenci> depo_;
};

void okuyucu_dongusu(const OgrenciDepo& depo, int okuyucu_no) {
    std::mt19937 rng(static_cast<unsigned>(okuyucu_no));
    std::uniform_int_distribution<int> dagilim(1, 50);

    for (int i = 0; i < 5; ++i) {
        const std::string numara = "OGR"
            + std::to_string(dagilim(rng));
        if (auto ogr = depo.bul(numara)) {
            std::cout << "[Okuyucu " << okuyucu_no << "] "
                      << numara << " -> " << ogr->ad_soyad
                      << " (GANO " << ogr->gano << ")\n";
        } else {
            std::cout << "[Okuyucu " << okuyucu_no << "] "
                      << numara << " bulunamadı.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void yazar_dongusu(OgrenciDepo& depo) {
    for (int i = 1; i <= 25; ++i) {
        Ogrenci o{
            .numara = "OGR" + std::to_string(i),
            .ad_soyad = "Öğrenci " + std::to_string(i),
            .gano = 2.0 + (i % 20) * 0.1,
        };
        depo.ekle(o);
        std::cout << "[Yazar] " << o.numara
                  << " eklendi. (toplam=" << depo.boyut() << ")\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    OgrenciDepo depo;

    std::vector<std::thread> okuyucular;
    for (int i = 1; i <= 3; ++i) {
        okuyucular.emplace_back(okuyucu_dongusu, std::cref(depo), i);
    }
    std::thread yazar(yazar_dongusu, std::ref(depo));

    yazar.join();
    for (auto& t : okuyucular) t.join();

    std::cout << "Son depo boyutu: " << depo.boyut() << '\n';
    return 0;
}

/*
BEKLENEN ÇIKTI (kısmen rastgele sıralı):
----------------------------------------
[Yazar] OGR1 eklendi. (toplam=1)
[Okuyucu 1] OGR3 -> Öğrenci 3 (GANO 2.3)
[Okuyucu 2] OGR42 bulunamadı.
...
Son depo boyutu: 25
*/

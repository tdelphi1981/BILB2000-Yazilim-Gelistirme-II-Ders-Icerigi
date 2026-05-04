/**
 * Length-Prefix Çerçeveleme — Kodla / Çöz Kütüphanesi
 *
 * Bu programda:
 *  - kodla(string) -> vector<uint8_t> ile mesaja 4-byte big-endian
 *    uzunluk önekı eklenir.
 *  - Coz sınıfı, akış (stream) tabanlı çözücüdür: kendisine parça
 *    parça veri besleyebilirsin, biriktirir; bir tam mesaj
 *    alındığında onu döndürür.
 *  - htonl / ntohl ile bayt düzeni dönüşümü (host <-> network)
 *  - main() fonksiyonunda parça parça besleme (chunked feed) ile
 *    gerçek bir TCP akışını taklit ediyoruz.
 *
 * OBYS bağlamı: TCP bir bayt akışıdır, mesaj sınırlarını korumaz.
 * "Merhaba" + "Sunucu" arka arkaya gönderilirse karşı tarafa
 * "MerhabaSunucu" olarak ulaşabilir. Length-prefix çerçeveleme,
 * her mesajın başına uzunluğunu yazıp bu sınırı korur. OBYS
 * protokolünün temel taşıdır.
 *
 * Bölüm 12 Ünite 4 Konu 1
 *
 * Derleme:  g++ -std=c++20 -O2 -Wall -Wextra -o 01_length_prefix 01_length_prefix_kodla_coz.cpp
 * Çalıştır: ./01_length_prefix
 */

#include <arpa/inet.h>  // htonl, ntohl

#include <cstdint>
#include <cstring>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

// ---------------------------------------------------------------
// 1) Kodlayıcı: string -> [4 bayt uzunluk][payload]
// ---------------------------------------------------------------
std::vector<uint8_t> kodla(const std::string& mesaj) {
    const uint32_t uzunlukKonak  = static_cast<uint32_t>(mesaj.size());
    const uint32_t uzunlukAg     = htonl(uzunlukKonak);

    std::vector<uint8_t> cerceve;
    cerceve.reserve(4 + mesaj.size());

    // 4 baytlık big-endian uzunluk önekini ekle
    const uint8_t* uzPtr = reinterpret_cast<const uint8_t*>(&uzunlukAg);
    cerceve.insert(cerceve.end(), uzPtr, uzPtr + 4);

    // Asıl mesajı ekle
    cerceve.insert(cerceve.end(), mesaj.begin(), mesaj.end());

    return cerceve;
}

// ---------------------------------------------------------------
// 2) Çözücü: akış üzerinden parça parça beslenir, tam mesaj
//    geldiğinde onu döndürür.
// ---------------------------------------------------------------
class Cozucu {
public:
    // Yeni veri ekle (TCP'den gelen ham baytlar gibi düşün)
    void besle(const uint8_t* veri, size_t boyut) {
        m_tampon.insert(m_tampon.end(), veri, veri + boyut);
    }
    void besle(const std::vector<uint8_t>& parca) {
        besle(parca.data(), parca.size());
    }

    // Tampon yeterliyse bir tam mesajı çıkar ve döndür.
    // Tampon eksikse nullopt döner; daha sonra besle() ile
    // veri eklenmeye devam edilebilir.
    std::optional<std::string> sonrakiMesaj() {
        // En az 4 bayt uzunluk önekı için yer var mı?
        if (m_tampon.size() < 4) return std::nullopt;

        // Önekı oku (henüz çıkarmadan)
        uint32_t uzunlukAg = 0;
        std::memcpy(&uzunlukAg, m_tampon.data(), 4);
        const uint32_t uzunluk = ntohl(uzunlukAg);

        // Aşırı büyük mesajlara karşı sağlamlık (16 MiB sınırı)
        if (uzunluk > 16u * 1024u * 1024u) {
            throw std::runtime_error("Çerçeve uzunluğu çok büyük");
        }

        // Tüm payload geldi mi?
        if (m_tampon.size() < 4 + uzunluk) {
            return std::nullopt;
        }

        // Payload'ı çıkar
        std::string mesaj(reinterpret_cast<const char*>(m_tampon.data() + 4),
                          uzunluk);

        // İşlenen baytları tampondan sil
        m_tampon.erase(m_tampon.begin(), m_tampon.begin() + 4 + uzunluk);

        return mesaj;
    }

    size_t tamponBoyutu() const { return m_tampon.size(); }

private:
    std::vector<uint8_t> m_tampon;
};

// ---------------------------------------------------------------
// 3) Demo: parça parça besleme ile gerçek bir TCP akışını taklit et
// ---------------------------------------------------------------
int main() {
    // Önce iki mesajı kodla
    std::vector<uint8_t> akis;
    auto ekleAkisa = [&](const std::vector<uint8_t>& cerceve) {
        akis.insert(akis.end(), cerceve.begin(), cerceve.end());
    };

    ekleAkisa(kodla("Merhaba sunucu"));
    ekleAkisa(kodla("Bu ikinci mesaj."));
    ekleAkisa(kodla("Üçüncü mesaj — Türkçe karakterler: çğıöşü ÇĞİÖŞÜ"));

    std::cout << "Toplam akış boyutu: " << akis.size() << " bayt\n";
    std::cout << "----------------------------------------\n";

    // Şimdi çözücüyü 7 baytlık parçalarla besle
    // (gerçek TCP'de recv her seferinde değişken miktarda veri döner)
    Cozucu cozucu;
    constexpr size_t PARCA = 7;
    int mesajSayaci = 0;

    for (size_t i = 0; i < akis.size(); i += PARCA) {
        size_t parcaBoyut = std::min(PARCA, akis.size() - i);
        cozucu.besle(akis.data() + i, parcaBoyut);

        std::cout << "[besle] " << parcaBoyut << " bayt eklendi, "
                  << "tampon=" << cozucu.tamponBoyutu() << "\n";

        // Bir veya birden çok mesaj çıkmış olabilir; hepsini al
        while (auto mesaj = cozucu.sonrakiMesaj()) {
            ++mesajSayaci;
            std::cout << "  [mesaj " << mesajSayaci << "] \"" << *mesaj << "\"\n";
        }
    }

    std::cout << "----------------------------------------\n";
    std::cout << "Toplam çözülen mesaj: " << mesajSayaci << "\n";
    std::cout << "Tamponda kalan bayt: " << cozucu.tamponBoyutu() << "\n";

    // Hatalı veri testi (sahte aşırı büyük çerçeve)
    std::cout << "\n[TEST] Hatalı çerçeve denemesi:\n";
    Cozucu hataCozucu;
    uint8_t kotuOnek[4] = {0xFF, 0xFF, 0xFF, 0xFF};  // ~4 GB
    hataCozucu.besle(kotuOnek, 4);
    try {
        (void)hataCozucu.sonrakiMesaj();
    } catch (const std::exception& ex) {
        std::cout << "  Beklenen hata yakalandı: " << ex.what() << "\n";
    }

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
Toplam akış boyutu: 89 bayt
----------------------------------------
[besle] 7 bayt eklendi, tampon=7
[besle] 7 bayt eklendi, tampon=14
  [mesaj 1] "Merhaba sunucu"
[besle] 7 bayt eklendi, tampon=4
[besle] 7 bayt eklendi, tampon=11
[besle] 7 bayt eklendi, tampon=18
  [mesaj 2] "Bu ikinci mesaj."
[besle] 7 bayt eklendi, tampon=5
[besle] 7 bayt eklendi, tampon=12
[besle] 7 bayt eklendi, tampon=19
[besle] 7 bayt eklendi, tampon=26
[besle] 7 bayt eklendi, tampon=33
[besle] 5 bayt eklendi, tampon=38
  [mesaj 3] "Üçüncü mesaj — Türkçe karakterler: çğıöşü ÇĞİÖŞÜ"
----------------------------------------
Toplam çözülen mesaj: 3
Tamponda kalan bayt: 0

[TEST] Hatalı çerçeve denemesi:
  Beklenen hata yakalandı: Çerçeve uzunluğu çok büyük
*/

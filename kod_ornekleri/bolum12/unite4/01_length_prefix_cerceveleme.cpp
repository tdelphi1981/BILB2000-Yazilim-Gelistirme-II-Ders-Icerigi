/**
 * Length-Prefix Çerçeveleme: Encoder + Decoder
 *
 * TCP bayt akımı içinde mesaj sınırlarını length-prefix yöntemiyle
 * korumak için encoder/decoder uygulaması. 4 bayt big-endian uzunluk
 * + N bayt payload formatı kullanır.
 *
 * Öğrenilecek kavramlar:
 * - htonl / ntohl ile bayt sırası dönüşümü
 * - Akümülatör buffer ile kısmi veri biriktirme
 * - Parser durum makinesi (HEADER_BEKLE / PAYLOAD_BEKLE)
 * - DOS koruması olarak çerçeve boyutu üst sınırı
 *
 * Bölüm: 12 - Ağ Programlamaya Giriş
 * Ünite: 4 - Protokol Tasarımı
 *
 * Derleme: g++ -std=c++20 -O2 -o length_prefix 01_length_prefix_cerceveleme.cpp
 * Çalıştırma: ./length_prefix
 */

#include <arpa/inet.h>      // htonl, ntohl
#include <cstdint>
#include <cstring>          // memcpy
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

//----------------------------------------------------------------------
// Encoder: payload'ı length-prefix çerçeveye sarar
//----------------------------------------------------------------------
std::vector<std::uint8_t> cerceveSar(std::span<const std::uint8_t> payload) {
    std::vector<std::uint8_t> cerceve;
    cerceve.reserve(4 + payload.size());

    // 4 bayt uzunluk alanı (big-endian / network byte order)
    std::uint32_t boyut_he = static_cast<std::uint32_t>(payload.size());
    std::uint32_t boyut_be = htonl(boyut_he);

    // htonl sonucunu bayt bayt yazıyoruz (strict aliasing'e dikkat)
    auto* p = reinterpret_cast<const std::uint8_t*>(&boyut_be);
    cerceve.insert(cerceve.end(), p, p + 4);

    // Payload'ı ekle
    cerceve.insert(cerceve.end(), payload.begin(), payload.end());
    return cerceve;
}

//----------------------------------------------------------------------
// Decoder: akümülatör buffer + parser durum makinesi
//----------------------------------------------------------------------
class CerceveDecoder {
    std::vector<std::uint8_t> buf_;
    std::size_t maks_payload_;       // DOS koruma sınırı

public:
    explicit CerceveDecoder(std::size_t maks = 16 * 1024 * 1024)
        : maks_payload_(maks) {}

    // Yeni gelen baytları ekler ve içeriden çıkarılabilen tüm tam
    // çerçeveleri verilen callback'e sırayla yollar.
    template <typename Callback>
    void eklenenVeri(std::span<const std::uint8_t> yeni, Callback cb) {
        // 1) Yeni veriyi tampona kopyala
        buf_.insert(buf_.end(), yeni.begin(), yeni.end());

        // 2) Tampon içinde tam çerçeve(ler) arıyoruz
        while (true) {
            // HEADER_BEKLE: 4 bayttan az ise dur
            if (buf_.size() < 4) return;

            // Uzunluğu çek (big-endian -> host)
            std::uint32_t boyut_be;
            std::memcpy(&boyut_be, buf_.data(), 4);
            std::uint32_t boyut = ntohl(boyut_be);

            if (boyut > maks_payload_) {
                // Aşırı büyük: protokol ihlali, bağlantıyı kes
                throw std::runtime_error("Cerceve cok buyuk; baglanti kesildi");
            }

            // PAYLOAD_BEKLE: yeterince bayt yoksa dur
            if (buf_.size() < 4 + boyut) return;

            // Tam çerçeve hazır: callback'e ver
            cb(std::span<const std::uint8_t>(buf_.data() + 4, boyut));

            // Tükettiğimiz baytları tampondan sil
            buf_.erase(buf_.begin(), buf_.begin() + 4 + boyut);
        }
    }
};

//----------------------------------------------------------------------
// Test programı: round-trip
//----------------------------------------------------------------------
int main() {
    using namespace std::string_view_literals;

    // 1) Üç farklı mesajı sırayla sar ve tek bir akım oluştur
    std::vector<std::uint8_t> akim;
    for (auto m : { "Merhaba"sv,
                    "OBYS"sv,
                    "Ogrenci 12345 ekle"sv,
                    "Bayt akimi mesaj degil; cerceveleme uygulama isi."sv }) {
        std::span<const std::uint8_t> p(
            reinterpret_cast<const std::uint8_t*>(m.data()), m.size());
        auto cerceve = cerceveSar(p);
        akim.insert(akim.end(), cerceve.begin(), cerceve.end());
    }

    std::cout << "Toplam akim boyutu: " << akim.size() << " bayt\n";
    std::cout << "----------------------------------------\n";

    // 2) Akımı küçük parçalarla decoder'a ver (recv'in kısmi okuma davranışı)
    CerceveDecoder dec;
    int sayac = 0;
    constexpr std::size_t parca_boyutu = 3;
    for (std::size_t i = 0; i < akim.size(); i += parca_boyutu) {
        std::size_t adet = std::min(parca_boyutu, akim.size() - i);
        std::span<const std::uint8_t> parca(akim.data() + i, adet);
        dec.eklenenVeri(parca, [&](std::span<const std::uint8_t> mesaj) {
            std::string s(mesaj.begin(), mesaj.end());
            std::cout << ++sayac << ". mesaj (" << mesaj.size() << " bayt): "
                      << s << '\n';
        });
    }

    // 3) DOS koruma testi: cok buyuk header
    std::cout << "----------------------------------------\n";
    std::cout << "DOS koruma testi: 100 MB iddiali header\n";
    CerceveDecoder dec2(16 * 1024 * 1024);   // 16 MB sınır
    std::uint32_t buyuk = htonl(100u * 1024u * 1024u);   // 100 MB iddia
    auto* bp = reinterpret_cast<const std::uint8_t*>(&buyuk);
    std::vector<std::uint8_t> kotuHeader(bp, bp + 4);
    try {
        dec2.eklenenVeri(kotuHeader, [](auto){});
        std::cout << "[HATA] Decoder buyuk cerceveyi reddetmeliydi!\n";
    } catch (const std::exception& e) {
        std::cout << "[OK] Decoder reddetti: " << e.what() << '\n';
    }

    return 0;
}

/*
BEKLENEN ÇIKTI:
---------------
Toplam akim boyutu: 94 bayt
----------------------------------------
1. mesaj (7 bayt): Merhaba
2. mesaj (4 bayt): OBYS
3. mesaj (18 bayt): Ogrenci 12345 ekle
4. mesaj (49 bayt): Bayt akimi mesaj degil; cerceveleme uygulama isi.
----------------------------------------
DOS koruma testi: 100 MB iddiali header
[OK] Decoder reddetti: Cerceve cok buyuk; baglanti kesildi
*/

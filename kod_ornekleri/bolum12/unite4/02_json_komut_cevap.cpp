/**
 * JSON Komut/Cevap Protokolü -- Standalone Demo
 *
 * Qt'siz, salt-stdlib bir demo. OBYS Protocol v1'in komut/cevap
 * şablonunu küçük bir el-yazımı JSON üreticisi ile gösterir.
 * Gerçek üretimde Qt'nin QJsonDocument/QJsonObject sınıfları
 * tercih edilir (kitabın 2. konusuna bak).
 *
 * Öğrenilecek kavramlar:
 * - Komut/cevap format şablonu (protokol, komut, id, veri)
 * - Korelasyon ID ile istek-cevap eşleştirme
 * - Versiyon kontrolü ile uyumsuz protokol surumlerinin reddi
 * - Length-prefix + JSON birleşik encoder'ı
 *
 * Bölüm: 12 - Ağ Programlamaya Giriş
 * Ünite: 4 - Protokol Tasarımı
 *
 * Derleme: g++ -std=c++20 -O2 -o json_komut 02_json_komut_cevap.cpp
 * Çalıştırma: ./json_komut
 */

#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

//----------------------------------------------------------------------
// Mini JSON String Escape (sadece demo amaçlı)
//----------------------------------------------------------------------
std::string jsonEscape(std::string_view s) {
    std::string out;
    out.reserve(s.size() + 2);
    out.push_back('"');
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out.push_back(c);
        }
    }
    out.push_back('"');
    return out;
}

//----------------------------------------------------------------------
// Komut Üretici: OBYS Protocol v1 şablonu
//----------------------------------------------------------------------
std::string ogrenciEkleKomutu(int korelasyon_id, int ogrenci_id,
                               std::string_view isim, double gpa) {
    std::ostringstream oss;
    oss << "{"
        << "\"protokol\":\"obys/1\","
        << "\"komut\":\"ogrenci_ekle\","
        << "\"id\":" << korelasyon_id << ","
        << "\"veri\":{"
        <<   "\"id\":" << ogrenci_id << ","
        <<   "\"isim\":" << jsonEscape(isim) << ","
        <<   "\"gpa\":" << gpa
        << "}"
        << "}";
    return oss.str();
}

std::string basarili_cevap(int korelasyon_id, std::string_view veri_json) {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << korelasyon_id << ","
        << "\"durum\":\"ok\","
        << "\"veri\":" << veri_json
        << "}";
    return oss.str();
}

std::string hatali_cevap(int korelasyon_id, std::string_view kod,
                          std::string_view mesaj) {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << korelasyon_id << ","
        << "\"durum\":\"hata\","
        << "\"veri\":{\"kod\":" << jsonEscape(kod) << "},"
        << "\"mesaj\":" << jsonEscape(mesaj)
        << "}";
    return oss.str();
}

//----------------------------------------------------------------------
// Length-Prefix Çerçeveleme (Konu 1'den)
//----------------------------------------------------------------------
std::vector<std::uint8_t> jsonCerceveSar(std::string_view json) {
    std::vector<std::uint8_t> cerceve;
    cerceve.reserve(4 + json.size());

    std::uint32_t boyut_be = htonl(static_cast<std::uint32_t>(json.size()));
    auto* p = reinterpret_cast<const std::uint8_t*>(&boyut_be);
    cerceve.insert(cerceve.end(), p, p + 4);
    cerceve.insert(cerceve.end(),
                   reinterpret_cast<const std::uint8_t*>(json.data()),
                   reinterpret_cast<const std::uint8_t*>(json.data()) + json.size());
    return cerceve;
}

void cerceveYazdir(const std::vector<std::uint8_t>& c, const std::string& etiket) {
    std::cout << etiket << " (" << c.size() << " bayt toplam, "
              << (c.size() - 4) << " bayt payload):\n  ";
    // Header: 4 bayt hex
    for (std::size_t i = 0; i < 4 && i < c.size(); ++i) {
        printf("%02X ", c[i]);
    }
    std::cout << "| ";
    // Payload: ASCII
    for (std::size_t i = 4; i < c.size(); ++i) {
        std::cout << static_cast<char>(c[i]);
    }
    std::cout << "\n\n";
}

//----------------------------------------------------------------------
// Demo
//----------------------------------------------------------------------
int main() {
    std::cout << "=== OBYS Protocol v1 -- Komut/Cevap Örnekleri ===\n\n";

    // 1) ogrenci_ekle istegi
    auto istek = ogrenciEkleKomutu(/*korelasyon=*/42, /*id=*/12345,
                                    "Ali Yilmaz", 3.45);
    std::cout << "ISTEK JSON:\n  " << istek << "\n\n";
    auto istek_cerceve = jsonCerceveSar(istek);
    cerceveYazdir(istek_cerceve, "ISTEK CERCEVESI");

    // 2) Basarili cevap
    auto cevap = basarili_cevap(42, "{\"id\":12345}");
    std::cout << "CEVAP JSON (basarili):\n  " << cevap << "\n\n";
    auto cevap_cerceve = jsonCerceveSar(cevap);
    cerceveYazdir(cevap_cerceve, "CEVAP CERCEVESI");

    // 3) Hatali cevap (mukerrer ID)
    auto hata = hatali_cevap(42, "DUPLICATE",
                              "12345 numarali ogrenci zaten kayitli");
    std::cout << "CEVAP JSON (hata):\n  " << hata << "\n\n";

    // 4) Login istegi (token alacagi cevap)
    auto login = ogrenciEkleKomutu(0, 0, "", 0.0);   // sadece sablon gosterimi
    std::cout << "[NOT] Login icin gercek implementasyonda ayri bir uretici\n"
              << "      fonksiyonu yazilir: kullanici_adi + parola_hash.\n\n";

    // 5) Korelasyon ID gostergesi: 3 paralel istek
    std::cout << "=== Korelasyon ID Demo ===\n";
    std::cout << "Istemci 3 isteği paralel gönderir:\n";
    for (int id = 100; id < 103; ++id) {
        auto k = ogrenciEkleKomutu(id, 90000 + id, "Test Ogrenci", 3.0);
        std::cout << "  Istek id=" << id << ": " << k.substr(0, 50) << "...\n";
    }
    std::cout << "Sunucu cevapları farklı sırada dönebilir; istemci\n"
              << "korelasyon ID üzerinden eşleştirir (102, 100, 101 gibi).\n";

    return 0;
}

/*
BEKLENEN ÇIKTI (özet):
----------------------
=== OBYS Protocol v1 -- Komut/Cevap Örnekleri ===

ISTEK JSON:
  {"protokol":"obys/1","komut":"ogrenci_ekle","id":42,"veri":{"id":12345,...

ISTEK CERCEVESI (NN bayt toplam, MM bayt payload):
  00 00 00 NN | {"protokol":"obys/1",...

CEVAP JSON (basarili):
  {"id":42,"durum":"ok","veri":{"id":12345}}

CEVAP JSON (hata):
  {"id":42,"durum":"hata","veri":{"kod":"DUPLICATE"},"mesaj":"12345 ..."}

=== Korelasyon ID Demo ===
Istemci 3 isteği paralel gönderir:
  Istek id=100: ...
  Istek id=101: ...
  Istek id=102: ...
*/

/**
 * Kimlik Doğrulama İstemcisi - LOGIN Akışı ve Oturum Jetonu
 *
 * Bu istemci, sunucuya önce LOGIN komutu gönderir; başarılı olursa
 * dönen oturum jetonunu (token) saklar ve sonraki komutlarda
 * "token" alanı ile birlikte yollar. Parola önce küçük bir hash
 * fonksiyonundan geçirilir. NOT: Burada gösterilen FNV-1a hash
 * GERÇEK güvenli bir kimlik doğrulama yöntemi DEĞİLDİR; sadece
 * "parolayı düz metin yollamak yerine sabit boyda bir özet
 * yollamak" fikrini kavramsal olarak göstermek için seçilmiştir.
 * Üretim ortamında bcrypt/argon2 ya da TLS + güvenli protokoller
 * kullanılır.
 *
 * OBYS bağlamında: Öğrenci uygulaması, kullanıcı adı/parola ile
 * sunucuya kimlik doğrulatır; sonraki tüm istekler oturum jetonu
 * ile gider.
 *
 * Öğrenilecek kavramlar:
 * - LOGIN -> LOGIN_OK akışı
 * - Token saklama
 * - Parola hash'i (kavramsal)
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 2 - OBYS İstemci Geliştirme
 *
 * Derleme: g++ -std=c++20 -o ornek 02_kimlik_dogrulama_istemci.cpp
 * Çalıştırma: ./ornek 127.0.0.1 5555 ogr001 sifre123
 */

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr std::size_t TAMPON_BOYUT = 2048;

// Basit FNV-1a 64-bit hash - sadece kavramsal
std::string fnv1a_hash(const std::string& girdi) {
    constexpr std::uint64_t FNV_OFFSET = 1469598103934665603ULL;
    constexpr std::uint64_t FNV_PRIME = 1099511628211ULL;

    std::uint64_t h = FNV_OFFSET;
    for (unsigned char c : girdi) {
        h ^= c;
        h *= FNV_PRIME;
    }

    std::ostringstream s;
    s << std::hex << std::setw(16) << std::setfill('0') << h;
    return s.str();
}

// Basit JSON alan ayıklama (saf demo)
std::string alan_oku(const std::string& json, const std::string& anahtar) {
    const std::string desen = "\"" + anahtar + "\":\"";
    const auto baslangic = json.find(desen);
    if (baslangic == std::string::npos) return "";
    const auto deger_basi = baslangic + desen.size();
    const auto deger_sonu = json.find('"', deger_basi);
    if (deger_sonu == std::string::npos) return "";
    return json.substr(deger_basi, deger_sonu - deger_basi);
}

bool gonder_ve_oku(int sok, const std::string& istek, std::string& yanit) {
    const std::string paket = istek + "\n";
    if (::send(sok, paket.data(), paket.size(), 0) < 0) return false;

    char tampon[TAMPON_BOYUT];
    const ssize_t alinan = ::recv(sok, tampon, TAMPON_BOYUT - 1, 0);
    if (alinan <= 0) return false;
    tampon[alinan] = '\0';
    yanit.assign(tampon);
    return true;
}

int main(int argc, char* argv[]) {
    const std::string sunucu_ip = (argc > 1) ? argv[1] : "127.0.0.1";
    const int port = (argc > 2) ? std::stoi(argv[2]) : 5555;
    const std::string kullanici = (argc > 3) ? argv[3] : "ogr001";
    const std::string parola = (argc > 4) ? argv[4] : "sifre123";

    const int sok = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sok < 0) return 1;

    sockaddr_in sunucu{};
    sunucu.sin_family = AF_INET;
    sunucu.sin_port = htons(port);
    ::inet_pton(AF_INET, sunucu_ip.c_str(), &sunucu.sin_addr);

    if (::connect(sok,
                  reinterpret_cast<sockaddr*>(&sunucu),
                  sizeof(sunucu)) < 0) {
        std::cerr << "connect() hatası.\n";
        ::close(sok);
        return 1;
    }

    // 1) Parolayı hash'le
    const std::string parola_ozet = fnv1a_hash(parola);
    std::cout << "[İstemci] Parola özeti: " << parola_ozet << '\n';

    // 2) LOGIN gönder
    std::ostringstream login;
    login << "{\"komut\":\"LOGIN\",\"kullanici\":\"" << kullanici
          << "\",\"parola_hash\":\"" << parola_ozet << "\"}";

    std::string yanit;
    if (!gonder_ve_oku(sok, login.str(), yanit)) {
        std::cerr << "LOGIN gönderilemedi.\n";
        ::close(sok);
        return 1;
    }
    std::cout << "[İstemci] LOGIN yanıtı: " << yanit;

    // 3) LOGIN_OK ise token'ı yakala
    const std::string durum = alan_oku(yanit, "durum");
    if (durum != "LOGIN_OK") {
        std::cerr << "[İstemci] Kimlik doğrulanamadı.\n";
        ::close(sok);
        return 1;
    }
    const std::string token = alan_oku(yanit, "token");
    std::cout << "[İstemci] Oturum açıldı. Token: " << token << '\n';

    // 4) Token ile listeleme isteği gönder
    std::ostringstream istek;
    istek << "{\"komut\":\"OGRENCI_LISTE\",\"token\":\"" << token << "\"}";
    if (gonder_ve_oku(sok, istek.str(), yanit)) {
        std::cout << "[İstemci] LİSTE yanıtı: " << yanit;
    }

    ::close(sok);
    return 0;
}

/*
BEKLENEN ÇIKTI (örnek - sunucu davranışına bağlı):
---------------------------------------------------
[İstemci] Parola özeti: 4f9b3a...
[İstemci] LOGIN yanıtı: {"durum":"LOGIN_OK","token":"abc123"}
[İstemci] Oturum açıldı. Token: abc123
[İstemci] LİSTE yanıtı: {"durum":"OK","kayitlar":[...]}
*/

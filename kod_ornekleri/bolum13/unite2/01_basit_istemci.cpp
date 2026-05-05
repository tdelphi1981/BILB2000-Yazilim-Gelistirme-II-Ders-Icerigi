/**
 * Basit TCP İstemci - JSON Komut Gönder
 *
 * Bu istemci, POSIX socket() + connect() ile bir sunucuya bağlanır,
 * basit bir JSON komut gönderir ve sunucudan gelen yanıtı okuyup
 * ekrana basar. Üretim ortamında nlohmann/json gibi bir kütüphane
 * tercih edilir; burada didaktik amaçla string olarak elle kuruyoruz.
 *
 * OBYS bağlamında: Öğrenci uygulaması, merkez sunucuya bağlanıp
 * "öğrenci listesini ver" komutunu gönderir.
 *
 * Öğrenilecek kavramlar:
 * - socket(), connect()
 * - send(), recv()
 * - JSON tabanlı protokol gönderimi
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 2 - OBYS İstemci Geliştirme
 *
 * Derleme: g++ -std=c++20 -o ornek 01_basit_istemci.cpp
 * Çalıştırma: ./ornek 127.0.0.1 5555
 */

#include <cstring>
#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr std::size_t TAMPON_BOYUT = 2048;

int main(int argc, char* argv[]) {
    const std::string sunucu_ip = (argc > 1) ? argv[1] : "127.0.0.1";
    const int port = (argc > 2) ? std::stoi(argv[2]) : 5555;

    const int sok = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sok < 0) {
        std::cerr << "socket() hatası: " << std::strerror(errno) << '\n';
        return 1;
    }

    sockaddr_in sunucu{};
    sunucu.sin_family = AF_INET;
    sunucu.sin_port = htons(port);

    if (::inet_pton(AF_INET, sunucu_ip.c_str(), &sunucu.sin_addr) <= 0) {
        std::cerr << "Geçersiz IP adresi: " << sunucu_ip << '\n';
        ::close(sok);
        return 1;
    }

    std::cout << "[İstemci] " << sunucu_ip << ":" << port
              << " adresine bağlanıyor...\n";

    if (::connect(sok,
                  reinterpret_cast<sockaddr*>(&sunucu),
                  sizeof(sunucu)) < 0) {
        std::cerr << "connect() hatası: " << std::strerror(errno) << '\n';
        ::close(sok);
        return 1;
    }
    std::cout << "[İstemci] Bağlandı.\n";

    // Basit JSON komut - elle kurulmuş
    const std::string komut =
        R"({"komut":"OGRENCI_LISTE","sayfa":1,"limit":10})";

    // Önce mesaj uzunluğunu metin olarak gönderen basit protokol:
    // <komut>\n şeklinde satır satır.
    const std::string paket = komut + "\n";

    if (::send(sok, paket.data(), paket.size(), 0) < 0) {
        std::cerr << "send() hatası: " << std::strerror(errno) << '\n';
        ::close(sok);
        return 1;
    }
    std::cout << "[İstemci] Gönderildi: " << komut << '\n';

    // Yanıtı oku
    char tampon[TAMPON_BOYUT];
    const ssize_t alinan = ::recv(sok, tampon, TAMPON_BOYUT - 1, 0);
    if (alinan > 0) {
        tampon[alinan] = '\0';
        std::cout << "[İstemci] Sunucu yanıtı: " << tampon;
    } else if (alinan == 0) {
        std::cout << "[İstemci] Sunucu bağlantıyı kapattı.\n";
    } else {
        std::cerr << "[İstemci] recv() hatası: "
                  << std::strerror(errno) << '\n';
    }

    ::close(sok);
    std::cout << "[İstemci] Soket kapandı.\n";
    return 0;
}

/*
BEKLENEN ÇIKTI (sunucu açıkken):
---------------------------------
[İstemci] 127.0.0.1:5555 adresine bağlanıyor...
[İstemci] Bağlandı.
[İstemci] Gönderildi: {"komut":"OGRENCI_LISTE","sayfa":1,"limit":10}
[İstemci] Sunucu yanıtı: {"durum":"OK","kayitlar":[...]}
[İstemci] Soket kapandı.
*/

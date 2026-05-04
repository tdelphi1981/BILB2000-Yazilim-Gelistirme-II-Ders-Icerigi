/**
 * TCP Echo İstemci — getaddrinfo + connect ile sunucuya bağlanma
 *
 * Bu programda:
 *  - getaddrinfo() ile hostname + port çözümlemesi
 *  - socket / connect akışı (bağlı listede ilk başarılı adres ile)
 *  - stdin'den satır okuyup sunucuya gönderme
 *  - sunucudan gelen yanıtı recv() ile alıp ekrana yazma
 *  - EOF (Ctrl-D) ile temiz çıkış
 *
 * OBYS bağlamı: Bu istemci, "01_tcp_echo_sunucu" ile birlikte
 * çalışacak şekilde yazılmıştır. Klavyeden yazdığın her satırı
 * sunucuya gönderir, sunucu aynısını geri yollar ve istemci ekrana
 * basar. Böylece OBYS protokolüne geçmeden önce ham TCP akışını
 * deneyimlersin.
 *
 * Bölüm 12 Ünite 2 Konu 3
 *
 * Derleme:  g++ -std=c++20 -O2 -Wall -Wextra -o 02_tcp_echo_istemci 02_tcp_echo_istemci.cpp
 * Çalıştır: ./02_tcp_echo_istemci localhost 9000
 *           (sonra satır yaz, Enter; çıkmak için Ctrl-D)
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <csignal>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Argüman kontrolü
    if (argc < 3) {
        std::cerr << "Kullanım: " << argv[0] << " <hostname> <port>\n";
        std::cerr << "Örnek:   " << argv[0] << " localhost 9000\n";
        return 1;
    }

    const std::string hostname = argv[1];
    const std::string port     = argv[2];

    // Karşı taraf kapanırsa SIGPIPE almamak için
    std::signal(SIGPIPE, SIG_IGN);

    // 1) getaddrinfo: hostname + port çözümle
    addrinfo ipuclari{};
    ipuclari.ai_family   = AF_UNSPEC;     // IPv4 + IPv6
    ipuclari.ai_socktype = SOCK_STREAM;   // TCP

    addrinfo* sonuclar = nullptr;
    int durum = ::getaddrinfo(hostname.c_str(), port.c_str(), &ipuclari, &sonuclar);
    if (durum != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(durum) << "\n";
        return 1;
    }

    // 2) socket + connect: ilk başarılı adresi kullan
    int sokFd = -1;
    for (addrinfo* p = sonuclar; p != nullptr; p = p->ai_next) {
        sokFd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sokFd < 0) continue;

        if (::connect(sokFd, p->ai_addr, p->ai_addrlen) == 0) {
            // Bağlantı başarılı
            break;
        }

        std::perror("connect");
        ::close(sokFd);
        sokFd = -1;
    }

    ::freeaddrinfo(sonuclar);

    if (sokFd < 0) {
        std::cerr << "Hiçbir adrese bağlanılamadı.\n";
        return 1;
    }

    std::cout << "Sunucuya bağlandı: " << hostname << ":" << port << "\n";
    std::cout << "Satır yaz, Enter ile gönder. Çıkmak için Ctrl-D.\n";

    // 3) stdin'den satır oku, sunucuya gönder, yanıtı oku
    std::string satir;
    while (std::getline(std::cin, satir)) {
        // Satır sonu eklemeden gönderelim, sonradan kendimiz '\n' ekleyeceğiz
        satir.push_back('\n');

        // send: tüm veri yazılana kadar tekrar dene
        const char* veri = satir.data();
        size_t kalan     = satir.size();
        while (kalan > 0) {
            ssize_t yazilan = ::send(sokFd, veri, kalan, 0);
            if (yazilan <= 0) {
                std::perror("send");
                ::close(sokFd);
                return 1;
            }
            veri  += yazilan;
            kalan -= static_cast<size_t>(yazilan);
        }

        // recv: sunucudan tek seferde gelen yanıtı al
        char tampon[4096];
        ssize_t okunan = ::recv(sokFd, tampon, sizeof(tampon) - 1, 0);
        if (okunan <= 0) {
            if (okunan == 0) {
                std::cout << "Sunucu bağlantıyı kapattı.\n";
            } else {
                std::perror("recv");
            }
            break;
        }
        tampon[okunan] = '\0';
        std::cout << "Yanıt: " << tampon;
    }

    std::cout << "İstemci kapanıyor.\n";
    ::close(sokFd);
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
Sunucuya bağlandı: localhost:9000
Satır yaz, Enter ile gönder. Çıkmak için Ctrl-D.
merhaba sunucu
Yanıt: merhaba sunucu
ikinci satır
Yanıt: ikinci satır
^D
İstemci kapanıyor.
*/

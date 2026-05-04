/**
 * TCP Echo Sunucu — Tek istemcili POSIX soket örneği
 *
 * Bu programda:
 *  - socket / bind / listen / accept / recv / send / close akışı
 *  - SO_REUSEADDR ile bind hatasını önleme (sunucu tekrar başlatma)
 *  - recv döngüsü ile EOF (istemci kapattı) tespiti
 *  - sockaddr_in yapısı ve htons / INADDR_ANY kullanımı
 *
 * OBYS bağlamı: Bir merkezi sunucu uygulaması, bağlanan istemcilerden
 * gelen mesajları olduğu gibi geri yansıtarak ağ katmanını test eder.
 * Bu echo davranışı, "ping-pong" veya "alındı onayı" mantığının en
 * basit hali olarak düşünülebilir.
 *
 * Bölüm 12 Ünite 2 Konu 2
 *
 * Derleme:  g++ -std=c++20 -O2 -Wall -Wextra -o 01_tcp_echo_sunucu 01_tcp_echo_sunucu.cpp
 * Çalıştır: ./01_tcp_echo_sunucu 9000
 *           (başka terminal) telnet localhost 9000
 *           ya da:           nc localhost 9000
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <csignal>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

namespace {
// Ctrl-C ile temiz çıkış için bayrak
std::atomic<bool> kapat{false};

void sinyalKesici(int) {
    kapat.store(true);
}
}  // namespace

int main(int argc, char* argv[]) {
    // Port numarası argümandan alınır, varsayılan 9000
    uint16_t port = 9000;
    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
    }

    // Ctrl-C için sinyal yakalayıcı
    std::signal(SIGINT, sinyalKesici);
    // Karşı taraf bağlantıyı kapatınca SIGPIPE almamak için
    std::signal(SIGPIPE, SIG_IGN);

    // 1) socket: TCP üzerinde IPv4 dinleme soketi oluştur
    int dinleyiciFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (dinleyiciFd < 0) {
        std::perror("socket");
        return 1;
    }

    // 2) SO_REUSEADDR: sunucu kapanıp tekrar açılırken
    //    "Address already in use" hatasını önler
    int evet = 1;
    if (::setsockopt(dinleyiciFd, SOL_SOCKET, SO_REUSEADDR, &evet, sizeof(evet)) < 0) {
        std::perror("setsockopt(SO_REUSEADDR)");
        ::close(dinleyiciFd);
        return 1;
    }

    // 3) bind: soketi yerel adrese ve porta bağla
    sockaddr_in sunucuAdresi{};
    sunucuAdresi.sin_family      = AF_INET;
    sunucuAdresi.sin_addr.s_addr = htonl(INADDR_ANY);  // tüm arayüzler
    sunucuAdresi.sin_port        = htons(port);

    if (::bind(dinleyiciFd, reinterpret_cast<sockaddr*>(&sunucuAdresi),
               sizeof(sunucuAdresi)) < 0) {
        std::perror("bind");
        ::close(dinleyiciFd);
        return 1;
    }

    // 4) listen: pasif dinleme moduna geç (backlog = 5)
    if (::listen(dinleyiciFd, 5) < 0) {
        std::perror("listen");
        ::close(dinleyiciFd);
        return 1;
    }

    std::cout << "TCP echo sunucu " << port << " portunda dinliyor...\n";
    std::cout << "Çıkmak için Ctrl-C.\n";

    // Ana sunucu döngüsü: tek istemci modeli (her seferinde 1 istemci)
    while (!kapat.load()) {
        sockaddr_in istemciAdresi{};
        socklen_t istemciAdresiBoyut = sizeof(istemciAdresi);

        // 5) accept: yeni bir bağlantı bekler, bloklayan çağrı
        int istemciFd = ::accept(
            dinleyiciFd,
            reinterpret_cast<sockaddr*>(&istemciAdresi),
            &istemciAdresiBoyut
        );

        if (istemciFd < 0) {
            // accept Ctrl-C ile kesildiyse temiz çık
            if (kapat.load()) break;
            std::perror("accept");
            continue;
        }

        // İstemcinin adresini öğren ve logla
        char istemciIp[INET_ADDRSTRLEN] = {0};
        ::inet_ntop(AF_INET, &istemciAdresi.sin_addr, istemciIp, sizeof(istemciIp));
        std::cout << "Bağlandı: " << istemciIp
                  << ":" << ntohs(istemciAdresi.sin_port) << "\n";

        // 6) recv döngüsü: istemci EOF gönderene kadar oku ve geri yolla
        constexpr size_t TAMPON_BOYUT = 4096;
        char tampon[TAMPON_BOYUT];

        while (true) {
            ssize_t okunan = ::recv(istemciFd, tampon, sizeof(tampon), 0);
            if (okunan == 0) {
                // İstemci bağlantıyı düzgünce kapattı (EOF)
                std::cout << "İstemci ayrıldı.\n";
                break;
            }
            if (okunan < 0) {
                std::perror("recv");
                break;
            }

            // Aldığımız veriyi olduğu gibi geri yolla (echo)
            // send tüm veriyi yazana kadar parça parça gidebilir
            ssize_t toplamYazilan = 0;
            while (toplamYazilan < okunan) {
                ssize_t yazilan = ::send(istemciFd, tampon + toplamYazilan,
                                         okunan - toplamYazilan, 0);
                if (yazilan <= 0) {
                    std::perror("send");
                    break;
                }
                toplamYazilan += yazilan;
            }
            std::cout << "Echo: " << okunan << " bayt yansıtıldı.\n";
        }

        // 7) close: istemci soketini kapat
        ::close(istemciFd);
    }

    std::cout << "Sunucu kapanıyor.\n";
    ::close(dinleyiciFd);
    return 0;
}

/*
BEKLENEN CIKTI (sunucu tarafı):
-------------------------------
TCP echo sunucu 9000 portunda dinliyor...
Çıkmak için Ctrl-C.
Bağlandı: 127.0.0.1:54321
Echo: 8 bayt yansıtıldı.
İstemci ayrıldı.
^C
Sunucu kapanıyor.

İstemci tarafı (nc localhost 9000):
-----------------------------------
$ echo "merhaba" | nc localhost 9000
merhaba
*/

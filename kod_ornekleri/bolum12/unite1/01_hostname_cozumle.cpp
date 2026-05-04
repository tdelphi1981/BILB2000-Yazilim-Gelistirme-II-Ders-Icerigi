/**
 * Hostname Çözümleme — getaddrinfo ile DNS Sorgusu
 *
 * Bu programda:
 *  - getaddrinfo() çağrısı ile bir alan adının (hostname)
 *    tüm IP adreslerini öğreniyoruz
 *  - IPv4 (AF_INET) ve IPv6 (AF_INET6) ayrımı yapıyoruz
 *  - inet_ntop() ile binary adresi okunabilir metne çeviriyoruz
 *  - freeaddrinfo() ile bağlı listeyi serbest bırakıyoruz
 *  - Hata durumunda gai_strerror() ile anlamlı mesaj alıyoruz
 *
 * OBYS bağlamı: Bir öğrenci uygulaması, "obys.universite.edu.tr"
 * gibi bir alan adı için sunucunun IP adresini öğrenmek zorundadır.
 * Bu işlemi DNS çözümlemesi (resolve) yapar.
 *
 * Bölüm 12 Ünite 1 Konu 1
 *
 * Derleme:  g++ -std=c++20 -O2 -Wall -Wextra -o 01_hostname_cozumle 01_hostname_cozumle.cpp
 * Çalıştır: ./01_hostname_cozumle www.example.com
 *           ./01_hostname_cozumle www.google.com
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Argüman kontrolü: en az bir hostname bekleniyor
    std::string hostname = (argc >= 2) ? argv[1] : "www.example.com";

    std::cout << "Çözümlenecek alan adı: " << hostname << "\n";
    std::cout << "----------------------------------------\n";

    // İpuçları (hints): hangi tip adresleri istediğimizi belirtir
    // AF_UNSPEC: hem IPv4 hem IPv6 dön
    // SOCK_STREAM: TCP bağlantıları için (çoğu uygulama için yeterlidir)
    addrinfo ipuclari{};
    ipuclari.ai_family   = AF_UNSPEC;
    ipuclari.ai_socktype = SOCK_STREAM;

    // Sonuç bağlı listesinin başını tutacak işaretçi
    addrinfo* sonuclar = nullptr;

    // DNS sorgusu yap
    int durum = getaddrinfo(hostname.c_str(), nullptr, &ipuclari, &sonuclar);
    if (durum != 0) {
        std::cerr << "getaddrinfo hatası: " << gai_strerror(durum) << "\n";
        return 1;
    }

    // Sonuçları döngü ile gez (ai_next üzerinden bağlı liste)
    int sayac = 0;
    for (addrinfo* p = sonuclar; p != nullptr; p = p->ai_next) {
        ++sayac;

        // IP adresini metin olarak yazdırmak için tampon
        char ipMetin[INET6_ADDRSTRLEN] = {0};
        const char* aile = "?";
        const void* adresIsaretci = nullptr;

        if (p->ai_family == AF_INET) {
            // IPv4: sin_addr alanını al
            auto* ipv4 = reinterpret_cast<sockaddr_in*>(p->ai_addr);
            adresIsaretci = &ipv4->sin_addr;
            aile = "IPv4";
        } else if (p->ai_family == AF_INET6) {
            // IPv6: sin6_addr alanını al
            auto* ipv6 = reinterpret_cast<sockaddr_in6*>(p->ai_addr);
            adresIsaretci = &ipv6->sin6_addr;
            aile = "IPv6";
        } else {
            continue;
        }

        // Binary adresi metne çevir
        if (inet_ntop(p->ai_family, adresIsaretci, ipMetin, sizeof(ipMetin)) == nullptr) {
            std::perror("inet_ntop");
            continue;
        }

        std::cout << "  [" << sayac << "] " << aile << "\t" << ipMetin << "\n";
    }

    // Bağlı listeyi serbest bırak (her getaddrinfo başarılı çağrısında zorunludur)
    freeaddrinfo(sonuclar);

    if (sayac == 0) {
        std::cout << "Hiç adres bulunamadı.\n";
        return 1;
    }

    std::cout << "----------------------------------------\n";
    std::cout << "Toplam " << sayac << " adres listelendi.\n";
    return 0;
}

/*
BEKLENEN CIKTI (örnek, gerçek IP'ler değişebilir):
--------------------------------------------------
Çözümlenecek alan adı: www.example.com
----------------------------------------
  [1] IPv4    93.184.215.14
  [2] IPv6    2606:2800:21f:cb07:6820:80da:af6b:8b2c
----------------------------------------
Toplam 2 adres listelendi.
*/

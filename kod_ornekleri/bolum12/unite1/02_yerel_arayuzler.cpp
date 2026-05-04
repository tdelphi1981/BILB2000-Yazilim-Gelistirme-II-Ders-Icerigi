/**
 * Yerel Ağ Arayüzleri — getifaddrs ile Arayüz Listesi
 *
 * Bu programda:
 *  - getifaddrs() ile yerel makinenin tüm ağ arayüzlerini listeliyoruz
 *  - Her arayüzün IPv4 ve IPv6 adreslerini gösteriyoruz
 *  - loopback (lo / lo0) arayüzünü diğerlerinden ayırıyoruz
 *  - freeifaddrs() ile bellek yönetimini RAII benzeri şekilde yapıyoruz
 *
 * OBYS bağlamı: Bir sunucu uygulaması başlarken kendi IP'sini
 * öğrenmek isteyebilir (loglama, kullanıcıya gösterme amacıyla).
 * "Sunucu 192.168.1.42:9000 üzerinde dinleniyor" mesajı için
 * önce yerel arayüzleri taramamız gerekir.
 *
 * Bölüm 12 Ünite 1 Konu 2
 *
 * Derleme:  g++ -std=c++20 -O2 -Wall -Wextra -o 02_yerel_arayuzler 02_yerel_arayuzler.cpp
 * Çalıştır: ./02_yerel_arayuzler
 */

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>
#include <string>

int main() {
    ifaddrs* arayuzler = nullptr;

    // Yerel arayüzleri sorgula
    if (getifaddrs(&arayuzler) == -1) {
        std::perror("getifaddrs");
        return 1;
    }

    std::cout << "Yerel ağ arayüzleri:\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "Arayüz\tTip\tDurum\tAdres\n";
    std::cout << "------------------------------------------------------------\n";

    int sayac = 0;
    for (ifaddrs* p = arayuzler; p != nullptr; p = p->ifa_next) {
        // Adresi olmayan arayüzleri atla
        if (p->ifa_addr == nullptr) continue;

        const sa_family_t aile = p->ifa_addr->sa_family;
        // Sadece IPv4 ve IPv6 ile ilgileniyoruz
        if (aile != AF_INET && aile != AF_INET6) continue;

        char ipMetin[INET6_ADDRSTRLEN] = {0};
        const void* adresIsaretci = nullptr;
        const char* tip = "?";

        if (aile == AF_INET) {
            auto* ipv4 = reinterpret_cast<sockaddr_in*>(p->ifa_addr);
            adresIsaretci = &ipv4->sin_addr;
            tip = "IPv4";
        } else {
            auto* ipv6 = reinterpret_cast<sockaddr_in6*>(p->ifa_addr);
            adresIsaretci = &ipv6->sin6_addr;
            tip = "IPv6";
        }

        if (inet_ntop(aile, adresIsaretci, ipMetin, sizeof(ipMetin)) == nullptr) {
            continue;
        }

        // Loopback (kendine bağlı) arayüzü işaretleyelim
        const bool loopbackMi = (p->ifa_flags & IFF_LOOPBACK) != 0;
        const bool aktifMi    = (p->ifa_flags & IFF_UP) != 0;

        std::cout << p->ifa_name << "\t"
                  << tip << "\t"
                  << (aktifMi ? "UP" : "DOWN") << "\t"
                  << ipMetin
                  << (loopbackMi ? "  (loopback)" : "")
                  << "\n";
        ++sayac;
    }

    // Bağlı listeyi serbest bırak
    freeifaddrs(arayuzler);

    std::cout << "------------------------------------------------------------\n";
    std::cout << "Toplam " << sayac << " adres listelendi.\n";

    if (sayac == 0) {
        std::cout << "Hiç ağ arayüzü bulunamadı.\n";
        return 1;
    }

    return 0;
}

/*
BEKLENEN CIKTI (makineye göre değişir):
---------------------------------------
Yerel ağ arayüzleri:
------------------------------------------------------------
Arayüz  Tip     Durum   Adres
------------------------------------------------------------
lo0     IPv4    UP      127.0.0.1  (loopback)
lo0     IPv6    UP      ::1  (loopback)
en0     IPv4    UP      192.168.1.42
en0     IPv6    UP      fe80::1c4d:9aff:feab:cdef
------------------------------------------------------------
Toplam 4 adres listelendi.
*/

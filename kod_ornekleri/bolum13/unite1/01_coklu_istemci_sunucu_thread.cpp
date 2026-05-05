/**
 * Çoklu İstemci Echo Sunucusu - std::thread + POSIX Sockets
 *
 * Bu program, POSIX socket API'si ile basit bir TCP echo sunucusu kurar.
 * Her gelen istemci için ayrı bir std::thread oluşturulur ve istemcinin
 * gönderdiği mesajlar geri yollanır. Sunucu kapanırken tüm thread'ler
 * join edilir, böylece kaynak sızıntısı yaşanmaz.
 *
 * OBYS bağlamında: Her öğrenci (istemci) merkez sunucuya bağlanır,
 * sunucu da her öğrenciye dedike bir işleyici thread atar.
 *
 * Öğrenilecek kavramlar:
 * - POSIX socket(), bind(), listen(), accept()
 * - std::thread ile çoklu istemci yönetimi
 * - std::atomic ile çalışma bayrağı
 * - RAII ile soket kapatma
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 1 - OBYS Sunucu Tasarımı
 *
 * Derleme: g++ -std=c++20 -pthread -o ornek 01_coklu_istemci_sunucu_thread.cpp
 * Çalıştırma: ./ornek    (test: telnet localhost 5555)
 */

#include <atomic>
#include <csignal>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT = 5555;
constexpr int BACKLOG = 16;
constexpr std::size_t TAMPON_BOYUT = 1024;

std::atomic<bool> calisiyor{true};
std::mutex cikti_kilidi;

// Bir istemciyi sonuna kadar dinleyen işleyici fonksiyon
void istemciyi_isle(int istemci_fd, int istemci_no) {
    {
        std::lock_guard kilit(cikti_kilidi);
        std::cout << "[Sunucu] İstemci #" << istemci_no << " bağlandı.\n";
    }

    char tampon[TAMPON_BOYUT];
    while (calisiyor.load()) {
        const ssize_t alinan = ::recv(istemci_fd, tampon, TAMPON_BOYUT - 1, 0);
        if (alinan <= 0) break;
        tampon[alinan] = '\0';

        // Yankı (echo) - aynı veriyi geri gönder
        ::send(istemci_fd, tampon, static_cast<std::size_t>(alinan), 0);

        std::lock_guard kilit(cikti_kilidi);
        std::cout << "[Sunucu] İstemci #" << istemci_no << " gönderdi: "
                  << tampon;
    }

    ::close(istemci_fd);
    std::lock_guard kilit(cikti_kilidi);
    std::cout << "[Sunucu] İstemci #" << istemci_no << " ayrıldı.\n";
}

int main() {
    // SIGPIPE'ı yok say (istemci aniden kapanırsa süreç ölmesin)
    std::signal(SIGPIPE, SIG_IGN);

    const int dinleme_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (dinleme_fd < 0) {
        std::cerr << "socket() hatası: " << std::strerror(errno) << '\n';
        return 1;
    }

    int yeniden_kullan = 1;
    ::setsockopt(dinleme_fd, SOL_SOCKET, SO_REUSEADDR,
                 &yeniden_kullan, sizeof(yeniden_kullan));

    sockaddr_in adres{};
    adres.sin_family = AF_INET;
    adres.sin_addr.s_addr = INADDR_ANY;
    adres.sin_port = htons(PORT);

    if (::bind(dinleme_fd,
               reinterpret_cast<sockaddr*>(&adres),
               sizeof(adres)) < 0) {
        std::cerr << "bind() hatası: " << std::strerror(errno) << '\n';
        ::close(dinleme_fd);
        return 1;
    }

    if (::listen(dinleme_fd, BACKLOG) < 0) {
        std::cerr << "listen() hatası: " << std::strerror(errno) << '\n';
        ::close(dinleme_fd);
        return 1;
    }

    std::cout << "[Sunucu] Port " << PORT << " üzerinde dinliyor...\n";
    std::cout << "[Sunucu] Çıkmak için Ctrl+C kullanın.\n";

    std::vector<std::thread> isciler;
    int istemci_sayaci = 0;

    while (calisiyor.load()) {
        sockaddr_in istemci_adres{};
        socklen_t adres_uzunluk = sizeof(istemci_adres);
        const int istemci_fd = ::accept(
            dinleme_fd,
            reinterpret_cast<sockaddr*>(&istemci_adres),
            &adres_uzunluk);

        if (istemci_fd < 0) {
            if (!calisiyor.load()) break;
            continue;
        }

        ++istemci_sayaci;
        isciler.emplace_back(istemciyi_isle, istemci_fd, istemci_sayaci);

        // Demo amaçlı: 3 istemci sonrası kapan
        if (istemci_sayaci >= 3) {
            calisiyor.store(false);
        }
    }

    ::close(dinleme_fd);

    // Tüm işçi thread'leri bekle
    for (auto& t : isciler) {
        if (t.joinable()) t.join();
    }

    std::cout << "[Sunucu] Düzgün şekilde kapandı.\n";
    return 0;
}

/*
BEKLENEN ÇIKTI (3 istemci bağlandıktan sonra):
----------------------------------------------
[Sunucu] Port 5555 üzerinde dinliyor...
[Sunucu] İstemci #1 bağlandı.
[Sunucu] İstemci #1 gönderdi: merhaba
[Sunucu] İstemci #1 ayrıldı.
...
[Sunucu] Düzgün şekilde kapandı.
*/

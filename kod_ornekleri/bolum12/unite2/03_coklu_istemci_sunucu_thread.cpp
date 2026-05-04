/**
 * Çoklu İstemcili TCP Sunucu — Thread-per-Connection Modeli
 *
 * Bu programda:
 *  - Her accept başarılı olduğunda yeni bir std::thread başlatıyoruz
 *  - Thread, ilgili istemciyle echo protokolünü yürütür ve detach edilir
 *  - SO_REUSEADDR ile sunucu yeniden başlatılabilir
 *  - Bağlı istemci sayısını std::atomic ile takip ediyoruz
 *  - Ctrl-C ile temiz kapanış (dinleyici soket kapatılır, thread'ler bitince çıkılır)
 *
 * OBYS bağlamı: Çok istemcili bir sunucuda her bağlantıyı bir
 * iş parçacığı (worker thread) ile servis etmek, en basit eşzamanlılık
 * modelidir. OBYS sunucumuzun bir sonraki adımı budur: birden çok
 * öğrenci uygulamasının aynı anda merkezi sunucuya bağlanabilmesi.
 *
 * Bölüm 12 Ünite 2 Konu 4
 *
 * Derleme:  g++ -std=c++20 -O2 -pthread -Wall -Wextra -o 03_coklu_sunucu 03_coklu_istemci_sunucu_thread.cpp
 * Çalıştır: ./03_coklu_sunucu 9001
 *           (başka terminallerde) nc localhost 9001
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <csignal>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

namespace {
std::atomic<bool>   kapat{false};
std::atomic<int>    aktifBaglanti{0};
std::atomic<int>    sonrakiId{1};
std::mutex          coutKilit;  // cout'a yazışları senkronize eder

void sinyalKesici(int) {
    kapat.store(true);
}

// Thread-safe yazdırma yardımcısı
template <typename... Args>
void log(Args&&... args) {
    std::lock_guard<std::mutex> g(coutKilit);
    (std::cout << ... << args) << "\n";
}
}  // namespace

// Bir istemciye servis veren iş parçacığı
void istemciyiServisleEt(int istemciFd, std::string istemciIp, uint16_t istemciPort) {
    const int benimId = sonrakiId.fetch_add(1);
    aktifBaglanti.fetch_add(1);

    log("[#", benimId, "] bağlandı: ", istemciIp, ":", istemciPort,
        " (aktif: ", aktifBaglanti.load(), ")");

    constexpr size_t TAMPON_BOYUT = 4096;
    char tampon[TAMPON_BOYUT];

    while (true) {
        ssize_t okunan = ::recv(istemciFd, tampon, sizeof(tampon), 0);
        if (okunan == 0) {
            log("[#", benimId, "] istemci ayrıldı (EOF).");
            break;
        }
        if (okunan < 0) {
            log("[#", benimId, "] recv hatası.");
            break;
        }

        // Echo: gelen veriyi olduğu gibi geri yaz
        ssize_t toplamYazilan = 0;
        while (toplamYazilan < okunan) {
            ssize_t yazilan = ::send(istemciFd, tampon + toplamYazilan,
                                     okunan - toplamYazilan, 0);
            if (yazilan <= 0) {
                log("[#", benimId, "] send hatası, bağlantı düşürüldü.");
                goto bitir;
            }
            toplamYazilan += yazilan;
        }
    }

bitir:
    ::close(istemciFd);
    aktifBaglanti.fetch_sub(1);
    log("[#", benimId, "] kapandı (aktif: ", aktifBaglanti.load(), ")");
}

int main(int argc, char* argv[]) {
    uint16_t port = 9001;
    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
    }

    std::signal(SIGINT, sinyalKesici);
    std::signal(SIGPIPE, SIG_IGN);

    // Dinleyici soketi oluştur
    int dinleyiciFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (dinleyiciFd < 0) {
        std::perror("socket");
        return 1;
    }

    int evet = 1;
    ::setsockopt(dinleyiciFd, SOL_SOCKET, SO_REUSEADDR, &evet, sizeof(evet));

    sockaddr_in adres{};
    adres.sin_family      = AF_INET;
    adres.sin_addr.s_addr = htonl(INADDR_ANY);
    adres.sin_port        = htons(port);

    if (::bind(dinleyiciFd, reinterpret_cast<sockaddr*>(&adres), sizeof(adres)) < 0) {
        std::perror("bind");
        ::close(dinleyiciFd);
        return 1;
    }

    // Backlog 32: kuyruğa kabul edilen ama henüz accept edilmemiş istek sayısı
    if (::listen(dinleyiciFd, 32) < 0) {
        std::perror("listen");
        ::close(dinleyiciFd);
        return 1;
    }

    log("Çoklu istemcili sunucu ", port, " portunda dinliyor. Ctrl-C ile çık.");

    // Ana accept döngüsü
    while (!kapat.load()) {
        sockaddr_in istemciAdresi{};
        socklen_t istemciAdresiBoyut = sizeof(istemciAdresi);

        int istemciFd = ::accept(
            dinleyiciFd,
            reinterpret_cast<sockaddr*>(&istemciAdresi),
            &istemciAdresiBoyut
        );

        if (istemciFd < 0) {
            if (kapat.load()) break;
            std::perror("accept");
            continue;
        }

        char ipMetin[INET_ADDRSTRLEN] = {0};
        ::inet_ntop(AF_INET, &istemciAdresi.sin_addr, ipMetin, sizeof(ipMetin));
        uint16_t istemciPort = ntohs(istemciAdresi.sin_port);

        // Yeni istemci için ayrı bir thread başlat ve detach et
        // Detach: thread bittiğinde kendi kaynaklarını otomatik temizler
        std::thread isci(istemciyiServisleEt, istemciFd,
                         std::string(ipMetin), istemciPort);
        isci.detach();
    }

    log("Sunucu kapanıyor. Aktif bağlantı: ", aktifBaglanti.load());
    ::close(dinleyiciFd);

    // Aktif thread'lerin bitmesi için kısa bir grace period
    using namespace std::chrono_literals;
    for (int i = 0; i < 50 && aktifBaglanti.load() > 0; ++i) {
        std::this_thread::sleep_for(50ms);
    }

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
Çoklu istemcili sunucu 9001 portunda dinliyor. Ctrl-C ile çık.
[#1] bağlandı: 127.0.0.1:51234 (aktif: 1)
[#2] bağlandı: 127.0.0.1:51235 (aktif: 2)
[#1] istemci ayrıldı (EOF).
[#1] kapandı (aktif: 1)
[#2] istemci ayrıldı (EOF).
[#2] kapandı (aktif: 0)
^C
Sunucu kapanıyor. Aktif bağlantı: 0
*/

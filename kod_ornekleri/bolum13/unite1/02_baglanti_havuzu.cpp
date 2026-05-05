/**
 * Bağlantı Havuzu - Üretici/Tüketici Deseni
 *
 * Bu örnek, basit bir bağlantı havuzu (connection pool) gösterir.
 * "Kabul edici" thread (üretici) gelen istemci file descriptor'larını
 * paylaşılan bir kuyruğa atar; sabit sayıdaki "işçi" thread'ler
 * (tüketici) kuyruktan iş çekip yanıtlar. Bu desen, Bölüm 10'da
 * gördüğümüz thread havuzu fikrinin ağ servislerine uyarlanmasıdır.
 *
 * OBYS bağlamında: Sınırsız thread açmak yerine, sabit sayıda işçiyi
 * önceden hazırlarız; gelen öğrenci istekleri kuyruğa girer ve uygun
 * işçi onları işler.
 *
 * Öğrenilecek kavramlar:
 * - std::queue + std::mutex + std::condition_variable
 * - Üretici/tüketici desen
 * - Sabit havuz boyutu ile thread reuse
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 1 - OBYS Sunucu Tasarımı
 *
 * Derleme: g++ -std=c++20 -pthread -o ornek 02_baglanti_havuzu.cpp
 * Çalıştırma: ./ornek
 */

#include <atomic>
#include <condition_variable>
#include <cstring>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT = 5556;
constexpr int ISCI_SAYISI = 4;
constexpr int BACKLOG = 32;

// Paylaşılan iş kuyruğu (client_fd değerleri)
std::queue<int> is_kuyrugu;
std::mutex kuyruk_kilidi;
std::condition_variable kuyruk_sinyali;
std::atomic<bool> sunucu_calisiyor{true};

// İşçi thread - kuyruktan iş çekip yanıtlar
void isci_dongusu(int isci_no) {
    while (true) {
        int istemci_fd = -1;
        {
            std::unique_lock kilit(kuyruk_kilidi);
            kuyruk_sinyali.wait(kilit, [] {
                return !is_kuyrugu.empty() || !sunucu_calisiyor.load();
            });

            if (!sunucu_calisiyor.load() && is_kuyrugu.empty()) return;

            istemci_fd = is_kuyrugu.front();
            is_kuyrugu.pop();
        }

        // İş yap: basit cevap gönder
        const std::string mesaj = "İşçi #" + std::to_string(isci_no)
                                + " seni karşıladı!\n";
        ::send(istemci_fd, mesaj.data(), mesaj.size(), 0);

        std::cout << "[İşçi " << isci_no << "] istemci fd=" << istemci_fd
                  << " işlendi.\n";
        ::close(istemci_fd);
    }
}

int main() {
    const int dinleme_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    int yeniden_kullan = 1;
    ::setsockopt(dinleme_fd, SOL_SOCKET, SO_REUSEADDR,
                 &yeniden_kullan, sizeof(yeniden_kullan));

    sockaddr_in adres{};
    adres.sin_family = AF_INET;
    adres.sin_addr.s_addr = INADDR_ANY;
    adres.sin_port = htons(PORT);

    if (::bind(dinleme_fd,
               reinterpret_cast<sockaddr*>(&adres),
               sizeof(adres)) < 0
        || ::listen(dinleme_fd, BACKLOG) < 0) {
        std::cerr << "bind/listen hatası: " << std::strerror(errno) << '\n';
        ::close(dinleme_fd);
        return 1;
    }

    std::cout << "[Sunucu] Bağlantı havuzu başlatılıyor (port " << PORT
              << ", " << ISCI_SAYISI << " işçi)\n";

    // İşçi havuzunu kur
    std::vector<std::thread> havuz;
    havuz.reserve(ISCI_SAYISI);
    for (int i = 1; i <= ISCI_SAYISI; ++i) {
        havuz.emplace_back(isci_dongusu, i);
    }

    // Demo: 5 istemci kabul ettikten sonra dur
    int kabul_edilen = 0;
    while (kabul_edilen < 5) {
        const int istemci_fd = ::accept(dinleme_fd, nullptr, nullptr);
        if (istemci_fd < 0) continue;

        {
            std::lock_guard kilit(kuyruk_kilidi);
            is_kuyrugu.push(istemci_fd);
        }
        kuyruk_sinyali.notify_one();
        ++kabul_edilen;
        std::cout << "[Kabul Edici] istek #" << kabul_edilen
                  << " kuyruğa eklendi.\n";
    }

    // Sunucuyu durdur
    sunucu_calisiyor.store(false);
    kuyruk_sinyali.notify_all();
    for (auto& t : havuz) {
        if (t.joinable()) t.join();
    }
    ::close(dinleme_fd);

    std::cout << "[Sunucu] Havuz kapatıldı.\n";
    return 0;
}

/*
BEKLENEN ÇIKTI:
---------------
[Sunucu] Bağlantı havuzu başlatılıyor (port 5556, 4 işçi)
[Kabul Edici] istek #1 kuyruğa eklendi.
[İşçi 1] istemci fd=N işlendi.
...
[Sunucu] Havuz kapatıldı.
*/

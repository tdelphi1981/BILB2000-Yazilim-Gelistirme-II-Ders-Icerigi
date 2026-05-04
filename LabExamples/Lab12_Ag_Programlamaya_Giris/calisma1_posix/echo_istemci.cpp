/**
 * Lab 12 - Çalışma 1: POSIX TCP Echo İstemcisi
 *
 * Bu dosyadaki "// GOREV" yorumlarını uygun kodla değiştirin.
 *
 * Hedef: localhost:9000 sunucusuna bağlanıp stdin'den okunan satırları
 *        gönderen ve gelen cevabı stdout'a yazan basit bir istemci.
 *
 * Kullanılan API:
 *   socket(), connect(), send(), recv(), close()
 *
 * Derleme:
 *   g++ -std=c++20 -O2 -o echo_istemci echo_istemci.cpp
 *
 * Test:
 *   ./echo_sunucu      (bir terminalde, GOREV-1 ve GOREV-2 tamamlandiktan sonra)
 *   ./echo_istemci     (bu terminalde; yaz "merhaba" -> "merhaba" geri gelir)
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

constexpr int PORT = 9000;
constexpr const char* SUNUCU_IP = "127.0.0.1";

int main() {
    // ============================================================
    // GOREV 1: Soket olustur ve sunucuya baglan.
    //
    // Adimlar:
    //   1) int s = ::socket(AF_INET, SOCK_STREAM, 0);
    //      Hata kontrolu.
    //
    //   2) sockaddr_in adres{};
    //      adres.sin_family = AF_INET;
    //      adres.sin_port   = htons(PORT);
    //      ::inet_pton(AF_INET, SUNUCU_IP, &adres.sin_addr);
    //
    //   3) ::connect(s, reinterpret_cast<sockaddr*>(&adres), sizeof(adres));
    //      Hata ise close + return.
    //
    //   4) std::cout << "Sunucuya baglandi. Mesaj yazin (Ctrl+D ile cik):\n";
    // ============================================================
    int s = -1;  // TODO

    // GOREV 1 buraya...

    if (s < 0) {
        std::cerr << "Baglanti kurulamadi (GOREV 1 tamamlanmamis).\n";
        return 1;
    }

    // ============================================================
    // GOREV 2: stdin'den satir oku, sokete gonder, cevabi yazdir.
    //
    // Adimlar:
    //   std::string satir;
    //   while (std::getline(std::cin, satir)) {
    //       satir.push_back('\n');
    //       // Tum baytlari yaz (kismi send'a karsi dongu)
    //       size_t toplam = 0;
    //       while (toplam < satir.size()) {
    //           ssize_t m = ::send(s, satir.data() + toplam,
    //                              satir.size() - toplam, 0);
    //           if (m < 0) { /* hata */ break; }
    //           toplam += m;
    //       }
    //
    //       // Cevabi oku (1 recv yetebilir; daha guvenli icin
    //       // satir tamamlanana kadar dongu).
    //       char tampon[1024];
    //       ssize_t n = ::recv(s, tampon, sizeof(tampon), 0);
    //       if (n > 0) {
    //           std::cout << "Sunucu> ";
    //           std::cout.write(tampon, n);
    //           std::cout.flush();
    //       } else if (n == 0) {
    //           std::cout << "Sunucu kapatti.\n";
    //           break;
    //       } else {
    //           if (errno == EINTR) continue;
    //           std::cerr << "recv: " << std::strerror(errno) << '\n';
    //           break;
    //       }
    //   }
    // ============================================================

    // GOREV 2 buraya...

    ::close(s);
    return 0;
}

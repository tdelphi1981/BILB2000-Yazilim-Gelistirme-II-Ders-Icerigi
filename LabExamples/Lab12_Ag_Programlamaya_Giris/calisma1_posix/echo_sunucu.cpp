/**
 * Lab 12 - Çalışma 1: POSIX TCP Echo Sunucu
 *
 * Bu dosyadaki "// GOREV" yorumlarını uygun kodla değiştirin.
 *
 * Hedef: 9000 portunda TCP bağlantısı kabul edip, istemcinin gönderdiği
 *        baytları aynen geri yollayan bir echo sunucusu yazmak.
 *
 * Kullanılan API:
 *   socket(), setsockopt(), bind(), listen(), accept(), recv(), send(), close()
 *
 * Derleme:
 *   g++ -std=c++20 -O2 -o echo_sunucu echo_sunucu.cpp
 *
 * Test:
 *   ./echo_sunucu          (bir terminalde)
 *   nc localhost 9000      (baska terminalde; yazdigin satir geri gelmeli)
 *   telnet localhost 9000  (alternatif)
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>

constexpr int PORT = 9000;
constexpr int BACKLOG = 16;

int main() {
    // ============================================================
    // GOREV 1: Dinleme soketini olustur, SO_REUSEADDR set et,
    //          INADDR_ANY:9000 adresine bind et, listen() cagir.
    //
    // Adimlar:
    //   1) int dinleme = ::socket(AF_INET, SOCK_STREAM, 0);
    //      Hata kontrolu (-1 ise strerror(errno) yazdir, return 1).
    //
    //   2) int evet = 1;
    //      ::setsockopt(dinleme, SOL_SOCKET, SO_REUSEADDR, &evet, sizeof(evet));
    //
    //   3) sockaddr_in adres{};
    //      adres.sin_family      = AF_INET;
    //      adres.sin_port        = htons(PORT);
    //      adres.sin_addr.s_addr = htonl(INADDR_ANY);
    //      ::bind(dinleme, reinterpret_cast<sockaddr*>(&adres), sizeof(adres));
    //
    //   4) ::listen(dinleme, BACKLOG);
    //
    //   5) std::cout << "Echo sunucu " << PORT << " portunda dinliyor...\n";
    // ============================================================
    int dinleme = -1;  // TODO: gercek soket fd'si ile degistirin

    // GOREV 1 buraya...

    if (dinleme < 0) {
        std::cerr << "Sunucu kurulamadi (GOREV 1 tamamlanmamis).\n";
        return 1;
    }

    // ============================================================
    // GOREV 2: accept() dongusu ve recv/send echo mantigi.
    //
    // Adimlar:
    //   while (true) {
    //       sockaddr_in iadres{};
    //       socklen_t ilen = sizeof(iadres);
    //       int istemci = ::accept(dinleme, ..., &ilen);
    //       if (istemci < 0) {
    //           if (errno == EINTR) continue;
    //           perror veya strerror; continue;
    //       }
    //       // Istemci IP/port log et (inet_ntop, ntohs)
    //
    //       char tampon[1024];
    //       while (true) {
    //           ssize_t n = ::recv(istemci, tampon, sizeof(tampon), 0);
    //           if (n > 0) {
    //               // Kismi yazmaya karsi dongulu send
    //               ssize_t yazildi = 0;
    //               while (yazildi < n) {
    //                   ssize_t m = ::send(istemci, tampon + yazildi,
    //                                      n - yazildi, 0);
    //                   if (m < 0) { yazildi = -1; break; }
    //                   yazildi += m;
    //               }
    //               if (yazildi < 0) break;
    //           } else if (n == 0) {
    //               std::cout << "Istemci kapatti.\n";
    //               break;
    //           } else {
    //               if (errno == EINTR) continue;
    //               break;
    //           }
    //       }
    //       ::close(istemci);
    //   }
    // ============================================================

    // GOREV 2 buraya...

    ::close(dinleme);
    return 0;
}

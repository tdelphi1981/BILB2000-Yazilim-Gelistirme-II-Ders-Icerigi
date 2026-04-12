/**
 * Islem ve Izlek Kimligi
 *
 * Bu program, calistigi islemin PID'sini ve ana izlegin kimligini gosterir.
 * Islem ve izlek kavramlarinin temel tanitimi icin kullanilir.
 *
 * Ogrenilecek kavramlar:
 * - getpid() ile islem kimligi
 * - std::this_thread::get_id() ile izlek kimligi
 * - std::thread::hardware_concurrency() ile cekirdek sayisi
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 1 - Eszamanlilik Kavramlari ve OpenMP
 *
 * Derleme: g++ -std=c++20 -o 01_islem_izlek_kimligi 01_islem_izlek_kimligi.cpp
 * Calistirma: ./01_islem_izlek_kimligi
 */

#include <iostream>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {
    // Islem kimligi (PID)
#ifdef _WIN32
    auto pid = GetCurrentProcessId();
#else
    auto pid = getpid();
#endif

    std::cout << "=== Islem ve Izlek Bilgileri ===" << '\n';
    std::cout << "Islem kimligi (PID): " << pid << '\n';

    // Ana izlek kimligi
    std::cout << "Ana izlek kimligi  : "
              << std::this_thread::get_id() << '\n';

    // Donanim eszamanliligi
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "Desteklenen eszamanli izlek sayisi: " << n << '\n';

    if (n == 0) {
        std::cout << "[UYARI] Sistem bu bilgiyi saglayamiyor." << '\n';
    } else if (n <= 2) {
        std::cout << "-> Dusuk cekirdek sayisi, paralellik sinirli." << '\n';
    } else {
        std::cout << "-> Cok cekirdekli sistem, paralellik icin uygun." << '\n';
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Islem ve Izlek Bilgileri ===
 * Islem kimligi (PID): 12345
 * Ana izlek kimligi  : 0x1234abcd
 * Desteklenen eszamanli izlek sayisi: 8
 * -> Cok cekirdekli sistem, paralellik icin uygun.
 */

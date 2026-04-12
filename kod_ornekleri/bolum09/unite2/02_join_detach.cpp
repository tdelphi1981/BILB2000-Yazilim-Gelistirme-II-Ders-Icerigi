/**
 * join() ve detach() - Izlek Yasam Dongusu Yonetimi
 *
 * Bu program join() ve detach() arasindaki farki gosterir.
 * Ayrica joinable() kontrolu ve guvenli sonlandirma kalibini icerir.
 *
 * Ogrenilecek kavramlar:
 * - join() ile izlek bekleme
 * - detach() ile bagimsiz izlek
 * - joinable() kontrolu
 * - Guvenli izlek sonlandirma kalibi
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 2 - std::thread Temelleri
 *
 * Derleme: g++ -std=c++20 -pthread -o 02_join_detach 02_join_detach.cpp
 * Calistirma: ./02_join_detach
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

// join() ornegi: Not hesaplama
void not_hesapla(const std::string& ogrenci) {
    std::cout << "[Hesap] " << ogrenci
              << " icin not hesaplaniyor...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "[Hesap] " << ogrenci
              << " not ortalamasi: 3.45\n";
}

// detach() ornegi: Log yazma
void log_yaz(std::string mesaj) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "[LOG] " << mesaj << '\n';
}

int main() {
    std::cout << "=== join() ve detach() Ornekleri ===\n\n";

    // --- join() ornegi ---
    std::cout << "--- join() Ornegi ---\n";
    std::cout << "[Ana] Hesaplama baslatiliyor...\n";

    std::thread t1(not_hesapla, "Ali Yilmaz");
    t1.join();  // Ana izlek bekler

    std::cout << "[Ana] Hesaplama tamamlandi, sonuc kullanilabilir.\n\n";

    // --- detach() ornegi ---
    std::cout << "--- detach() Ornegi ---\n";
    {
        std::thread t2(log_yaz, "Ogrenci tablosu acildi");
        t2.detach();  // Izlek serbest birakildi
        std::cout << "[Ana] Log izlegi serbest birakildi, devam.\n";
    }

    // detach edilen izlegin tamamlanmasi icin biraz bekle
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    // --- joinable() kontrolu ---
    std::cout << "\n--- joinable() Kontrolu ---\n";

    std::thread t3([]() {
        std::cout << "[Izlek] Calisiyorum.\n";
    });

    std::cout << "join oncesi joinable: " << std::boolalpha
              << t3.joinable() << '\n';  // true

    if (t3.joinable()) {
        t3.join();
    }

    std::cout << "join sonrasi joinable: " << std::boolalpha
              << t3.joinable() << '\n';  // false

    // --- Bos thread ---
    std::cout << "\n--- Bos Thread ---\n";
    std::thread t4;  // Varsayilan yapici, izlek yok
    std::cout << "Bos thread joinable: " << std::boolalpha
              << t4.joinable() << '\n';  // false

    std::cout << "\n[Ana] Program sona eriyor.\n";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === join() ve detach() Ornekleri ===
 *
 * --- join() Ornegi ---
 * [Ana] Hesaplama baslatiliyor...
 * [Hesap] Ali Yilmaz icin not hesaplaniyor...
 * [Hesap] Ali Yilmaz not ortalamasi: 3.45
 * [Ana] Hesaplama tamamlandi, sonuc kullanilabilir.
 *
 * --- detach() Ornegi ---
 * [Ana] Log izlegi serbest birakildi, devam.
 * [LOG] Ogrenci tablosu acildi
 *
 * --- joinable() Kontrolu ---
 * join oncesi joinable: true
 * [Izlek] Calisiyorum.
 * join sonrasi joinable: false
 *
 * --- Bos Thread ---
 * Bos thread joinable: false
 *
 * [Ana] Program sona eriyor.
 */

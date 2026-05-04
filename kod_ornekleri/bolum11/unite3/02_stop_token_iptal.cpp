/**
 * std::stop_token / std::jthread ile İşbirlikçi İptal
 *
 * Bölüm 11 Ünite 3 Konu 2
 *
 * Bu örnekte:
 *  - jthread içinde stop_token ile iptal kontrolü
 *  - stop_callback ile condition_variable uyandırma
 *  - Yarım veri politikası (RAII ile temizlik)
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 02_stop 02_stop_token_iptal.cpp
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stop_token>
#include <thread>
#include <vector>

class GeciciDosyaSahibi {
    std::string yol_;
    bool tamamlandi_ = false;
public:
    explicit GeciciDosyaSahibi(std::string y) : yol_(std::move(y)) {
        std::cout << "[+] Gecici acildi: " << yol_ << '\n';
    }
    ~GeciciDosyaSahibi() {
        if (!tamamlandi_) {
            std::cout << "[!] Iptal -> Gecici siliniyor: " << yol_ << '\n';
        } else {
            std::cout << "[OK] Tamamlandi: " << yol_ << '\n';
        }
    }
    void tamamla() { tamamlandi_ = true; }
    GeciciDosyaSahibi(const GeciciDosyaSahibi&) = delete;
    GeciciDosyaSahibi& operator=(const GeciciDosyaSahibi&) = delete;
};

void uzunIs(std::stop_token st, const std::string& ad) {
    GeciciDosyaSahibi gecici("/tmp/" + ad + ".tmp");

    for (int i = 0; i < 100; ++i) {
        if (st.stop_requested()) {
            std::cout << ad << ": Iptal istegi geldi (i=" << i << ")\n";
            return;     // Gecici otomatik silinir (RAII)
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    gecici.tamamla();
    std::cout << ad << ": Tamamlandi\n";
}

// stop_callback ile bekleyeni uyandırma
void cvIleBekle(std::stop_token st) {
    std::condition_variable cv;
    std::mutex m;
    bool durum = false;

    std::stop_callback uyandirici(st, [&] {
        std::lock_guard<std::mutex> g(m);
        cv.notify_all();
    });

    std::unique_lock<std::mutex> kilit(m);
    auto sonuc = cv.wait_for(kilit, std::chrono::seconds(10),
        [&] { return durum || st.stop_requested(); });

    if (st.stop_requested()) {
        std::cout << "[CV] Iptal ile uyandi\n";
    } else if (sonuc) {
        std::cout << "[CV] Durum tamamlandi\n";
    } else {
        std::cout << "[CV] Zaman asimi\n";
    }
}

int main() {
    std::cout << "1) Test: jthread destructor join + iptal\n";
    {
        std::jthread t1(uzunIs, "is1");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        t1.request_stop();
        // jthread destructor join'leyecek
    }

    std::cout << "\n2) Test: Tamamlanmasina izin ver\n";
    {
        std::jthread t2(uzunIs, "is2");
        // 5 saniye bekle (100 * 50ms = 5s)
        std::this_thread::sleep_for(std::chrono::seconds(6));
    }

    std::cout << "\n3) Test: stop_callback ile cv uyandirma\n";
    {
        std::jthread t3(cvIleBekle);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        t3.request_stop();
    }

    std::cout << "\nTum testler tamam.\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
1) Test: jthread destructor join + iptal
[+] Gecici acildi: /tmp/is1.tmp
is1: Iptal istegi geldi (i=4)
[!] Iptal -> Gecici siliniyor: /tmp/is1.tmp

2) Test: Tamamlanmasina izin ver
[+] Gecici acildi: /tmp/is2.tmp
is2: Tamamlandi
[OK] Tamamlandi: /tmp/is2.tmp

3) Test: stop_callback ile cv uyandirma
[CV] Iptal ile uyandi

Tum testler tamam.
*/

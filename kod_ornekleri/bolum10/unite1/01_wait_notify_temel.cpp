/**
 * Wait/Notify Temel Kullanımı
 *
 * Bu program, std::condition_variable ile iki izlek arasında
 * basit bir wait/notify haberleşmesini gösterir.
 *
 * Öğrenilecek kavramlar:
 * - std::condition_variable ile bekleme
 * - std::unique_lock ile koşul değişkeni kullanımı
 * - Predicate (yüklem) parametresi ile sahte uyanma koruması
 * - notify_one ile uyandırma
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 1 - condition_variable ile Koordinasyon
 *
 * Derleme: g++ -std=c++20 -pthread -o 01_wait_notify 01_wait_notify_temel.cpp
 * Çalıştırma: ./01_wait_notify
 */

#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
bool veriHazir = false;

void tuketici() {
    std::cout << "[Tüketici] veriyi bekliyorum...\n";
    std::unique_lock<std::mutex> kilit(mtx);
    // Predicate ile sahte uyanmalara karşı korun
    cv.wait(kilit, [] { return veriHazir; });
    std::cout << "[Tüketici] veri geldi, işliyorum\n";
}

void uretici() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[Üretici] veri hazırlanıyor\n";
    {
        std::lock_guard<std::mutex> kilit(mtx);
        veriHazir = true;
    }
    std::cout << "[Üretici] notify_one çağrılıyor\n";
    cv.notify_one();
}

int main() {
    std::thread t1(tuketici);
    std::thread t2(uretici);

    t1.join();
    t2.join();

    std::cout << "Program sonu\n";
    return 0;
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [Tüketici] veriyi bekliyorum...
 * [Üretici] veri hazırlanıyor
 * [Üretici] notify_one çağrılıyor
 * [Tüketici] veri geldi, işliyorum
 * Program sonu
 */

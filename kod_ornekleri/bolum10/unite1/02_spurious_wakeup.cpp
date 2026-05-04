/**
 * Sahte Uyanma (Spurious Wakeup) Korumalı wait
 *
 * Bu program, predicate'siz wait ile predicate'li wait arasındaki
 * farkı göstererek neden her zaman predicate kullanmamız gerektiğini
 * vurgular. Birden fazla tüketici tek bir bildirim için yarışır.
 *
 * Öğrenilecek kavramlar:
 * - Sahte uyanma (spurious wakeup) sorunu
 * - while döngüsü vs predicate farkı
 * - notify_all ve yarış
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 1 - condition_variable ile Koordinasyon
 *
 * Derleme: g++ -std=c++20 -pthread -o 02_spurious 02_spurious_wakeup.cpp
 * Çalıştırma: ./02_spurious
 */

#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
int kuyruktakiVeri = 0;

void tuketici_yanlis(int id) {
    std::unique_lock<std::mutex> kilit(mtx);
    // YANLIŞ: Predicate yok, sahte uyanmada yanlış davranır
    cv.wait(kilit);
    // Bu noktada kuyruktakiVeri > 0 olduğunu varsayıyoruz
    // ama garantisi yok!
    std::cout << "[Yanlış tüketici " << id << "] uyandı, veri="
              << kuyruktakiVeri << "\n";
}

void tuketici_dogru(int id) {
    std::unique_lock<std::mutex> kilit(mtx);
    // DOĞRU: Predicate ile gerçek koşul kontrolü
    cv.wait(kilit, [] { return kuyruktakiVeri > 0; });
    --kuyruktakiVeri;
    std::cout << "[Doğru tüketici " << id << "] işi aldı, kalan="
              << kuyruktakiVeri << "\n";
}

int main() {
    std::cout << "=== Doğru wait kullanımı ===\n";

    // 3 tüketici, 1 iş -- sadece biri uyanıp iş alır
    std::vector<std::thread> tuketiciler;
    for (int i = 1; i <= 3; ++i) {
        tuketiciler.emplace_back(tuketici_dogru, i);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Tek iş ekle ve notify_all
    {
        std::lock_guard<std::mutex> kilit(mtx);
        kuyruktakiVeri = 1;
    }
    cv.notify_all();

    // 100 ms bekle, kalan bekleyenleri iş ekleyerek kurtar
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    {
        std::lock_guard<std::mutex> kilit(mtx);
        kuyruktakiVeri = 2;
    }
    cv.notify_all();

    for (auto& t : tuketiciler) t.join();

    std::cout << "\nNot: predicate sayesinde sahte uyanan tüketiciler\n";
    std::cout << "gerçek veri olmadığında tekrar uyudu.\n";
    return 0;
}

/*
 * BEKLENEN ÇIKTI (sıra değişebilir):
 * -----------------------------------
 * === Doğru wait kullanımı ===
 * [Doğru tüketici 1] işi aldı, kalan=0
 * [Doğru tüketici 2] işi aldı, kalan=1
 * [Doğru tüketici 3] işi aldı, kalan=0
 *
 * Not: predicate sayesinde sahte uyanan tüketiciler
 * gerçek veri olmadığında tekrar uyudu.
 */

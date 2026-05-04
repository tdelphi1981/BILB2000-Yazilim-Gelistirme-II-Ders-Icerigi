/**
 * Lab 10 - Çalışma 1: condition_variable ile Üretici/Tüketici
 *
 * Bu dosyadaki GOREV yorumlarını uygun kodla değiştirin.
 *
 * Derleme: g++ -std=c++20 -pthread -o calisma1 calisma1.cpp
 * Çalıştırma: ./calisma1
 */

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <chrono>

std::queue<int> kuyruk;
std::mutex mtx;
std::condition_variable cv;

void uretici() {
    for (int i = 1; i <= 5; ++i) {
        // GOREV 1: kuyruga i degerini ekle (mutex korumali)
        //   - std::lock_guard ile mtx kilitle
        //   - kuyruk.push(i)

        // GOREV 2: bir tuketici uyandir
        //   - cv.notify_one()

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void tuketici() {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::mutex> kilit(mtx);

        // GOREV 3: kuyruk bos olmayana kadar bekle (predicate ile)
        //   - cv.wait(kilit, [] { return !kuyruk.empty(); });

        int deger = kuyruk.front();
        kuyruk.pop();
        kilit.unlock();
        std::cout << "Alindi: " << deger << "\n";
    }
}

int main() {
    std::thread t1(uretici);
    std::thread t2(tuketici);
    t1.join();
    t2.join();
    std::cout << "Bitti\n";
    return 0;
}

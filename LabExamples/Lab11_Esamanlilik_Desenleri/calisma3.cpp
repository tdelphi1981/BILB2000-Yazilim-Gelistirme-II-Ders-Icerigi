/**
 * Lab 11 - Çalışma 3: Paralel CSV İçe Aktarma + jthread + stop_token
 *
 * Bu dosyadaki GOREV yorumlarını uygun kodla değiştirin.
 *
 * Derleme: g++ -std=c++20 -pthread -O2 -o calisma3 calisma3.cpp
 * Çalıştırma: ./calisma3
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <sstream>
#include <stop_token>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// Calisma 1'den ThreadSafeQueue (ozet sürüm)
template <typename T>
class TSQueue {
    mutable std::mutex m_;
    std::condition_variable cv_;
    std::queue<T> q_;
    bool kapali_ = false;
public:
    void push(T d) {
        { std::lock_guard<std::mutex> g(m_); q_.push(std::move(d)); }
        cv_.notify_one();
    }
    bool waitAndPop(T& cikti) {
        std::unique_lock<std::mutex> g(m_);
        cv_.wait(g, [this] { return !q_.empty() || kapali_; });
        if (q_.empty()) return false;
        cikti = std::move(q_.front()); q_.pop();
        return true;
    }
    void kapat() {
        { std::lock_guard<std::mutex> g(m_); kapali_ = true; }
        cv_.notify_all();
    }
    bool kapaliMi() const {
        std::lock_guard<std::mutex> g(m_); return kapali_;
    }
};

struct Ogrenci {
    int numara;
    std::string ad;
    std::string bolum;
};

Ogrenci parse(const std::string& s) {
    Ogrenci o;
    std::istringstream iss(s);
    std::string a;
    std::getline(iss, a, ','); o.numara = std::stoi(a);
    std::getline(iss, o.ad, ',');
    std::getline(iss, o.bolum, ',');
    return o;
}

void testVerisiOlustur(const std::string& yol, int n) {
    std::ofstream f(yol);
    f << "numara,ad,bolum,gpa\n";
    for (int i = 1; i <= n; ++i) {
        f << (100000 + i) << ",Ogrenci_" << i
          << ",Bilgisayar," << (2.0 + (i % 200) * 0.01) << '\n';
    }
}

// =============================================================================
// CSV PARALEL OKUMA - GÖREVLER
// =============================================================================
void csvParalelOku(
    const std::string& yol,
    std::shared_mutex& sm,
    std::unordered_map<int, std::shared_ptr<Ogrenci>>& depo,
    std::stop_source ss)
{
    TSQueue<std::string> kuyruk;
    std::atomic<std::size_t> sayac{0};

    // GOREV 1: Üretici thread (jthread)
    //   - std::ifstream ile yol'u ac
    //   - basligi atla
    //   - while (getline) -> kuyruk.push
    //   - st.stop_requested() varsa cik
    //   - sonunda kuyruk.kapat()


    // GOREV 2: N tüketici thread (jthread)
    //   - hardware_concurrency kadar
    //   - while (kuyruk.waitAndPop(satir))
    //   -   try { parse, mutex altinda depo'ya ekle, sayac++ } catch ...


    // Ana thread: 500 ms aralıklarla ilerleme yazdırır
    while (!ss.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Islenen: " << sayac.load() << '\n';
        if (kuyruk.kapaliMi()) break;
    }

    // jthread'ler destructor'da otomatik join
}

int main() {
    const std::string yol = "/tmp/calisma3.csv";
    constexpr int N = 50'000;

    testVerisiOlustur(yol, N);

    std::shared_mutex sm;
    std::unordered_map<int, std::shared_ptr<Ogrenci>> depo;
    std::stop_source ss;

    auto t0 = std::chrono::steady_clock::now();
    csvParalelOku(yol, sm, depo, ss);
    auto t1 = std::chrono::steady_clock::now();

    auto sure = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    std::cout << "Yuklenen: " << depo.size() << '\n';
    std::cout << "Sure: " << sure.count() << " ms\n";
    return 0;
}

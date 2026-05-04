/**
 * Paralel CSV Yazma - Tek Yazıcı Deseni
 *
 * Bölüm 11 Ünite 2 Konu 2
 *
 * Bu örnekte:
 *  - N işçi paralel olarak satırları serileştirir
 *  - Tek yazıcı thread sırayla dosyaya yazar
 *  - Atomic indeks ile iş dağıtımı
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 02_yaz 02_paralel_yazma.cpp
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

template <typename T>
class TSQueue {
    mutable std::mutex m_;
    std::condition_variable cv_;
    std::queue<T> kuyruk_;
    bool kapali_ = false;
public:
    void push(T d) {
        { std::lock_guard<std::mutex> g(m_); kuyruk_.push(std::move(d)); }
        cv_.notify_one();
    }
    bool waitAndPop(T& cikti) {
        std::unique_lock<std::mutex> g(m_);
        cv_.wait(g, [this] { return !kuyruk_.empty() || kapali_; });
        if (kuyruk_.empty()) return false;
        cikti = std::move(kuyruk_.front());
        kuyruk_.pop();
        return true;
    }
    void kapat() {
        { std::lock_guard<std::mutex> g(m_); kapali_ = true; }
        cv_.notify_all();
    }
};

struct Ogrenci {
    int numara;
    std::string ad;
    std::string bolum;
    double gpa;
};

std::string seriCSV(const Ogrenci& o) {
    std::ostringstream oss;
    oss << o.numara << ',' << o.ad << ',' << o.bolum << ','
        << std::fixed << o.gpa << '\n';
    return oss.str();
}

void csvDisaAktarParalel(const std::string& yol,
                          const std::vector<std::shared_ptr<Ogrenci>>& ogrenciler) {
    TSQueue<std::string> kuyruk;
    std::atomic<std::size_t> indeks{0};
    auto isciSayisi = std::max(2u, std::thread::hardware_concurrency() - 1);

    std::vector<std::thread> isciler;
    for (unsigned i = 0; i < isciSayisi; ++i) {
        isciler.emplace_back([&] {
            while (true) {
                auto idx = indeks.fetch_add(1);
                if (idx >= ogrenciler.size()) break;
                kuyruk.push(seriCSV(*ogrenciler[idx]));
            }
        });
    }

    std::thread yazici([&] {
        std::ofstream dosya(yol);
        dosya << "numara,ad,bolum,gpa\n";
        std::string satir;
        while (kuyruk.waitAndPop(satir)) {
            dosya << satir;
        }
    });

    for (auto& t : isciler) t.join();
    kuyruk.kapat();
    yazici.join();
}

int main() {
    constexpr int N = 100'000;

    std::vector<std::shared_ptr<Ogrenci>> ogrenciler;
    ogrenciler.reserve(N);
    for (int i = 1; i <= N; ++i) {
        ogrenciler.push_back(std::make_shared<Ogrenci>(
            Ogrenci{100000 + i, "Ad_" + std::to_string(i), "Bolum", 2.5 + (i % 100) * 0.01}));
    }

    auto t0 = std::chrono::steady_clock::now();
    csvDisaAktarParalel("/tmp/disa_aktar_b11.csv", ogrenciler);
    auto t1 = std::chrono::steady_clock::now();

    auto sure = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    std::cout << N << " kayit yazildi: " << sure.count() << " ms\n";
    std::cout << "Hiz: " << (N * 1000LL / sure.count()) << " satir/sn\n";

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
100000 kayit yazildi: 200-500 ms
Hiz: ~200K-500K satir/sn
*/

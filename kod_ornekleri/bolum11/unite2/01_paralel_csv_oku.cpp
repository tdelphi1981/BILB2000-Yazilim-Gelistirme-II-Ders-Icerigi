/**
 * Paralel CSV Okuma - Üretici-Tüketici Boru Hattı
 *
 * Bölüm 11 Ünite 2 Konu 1
 *
 * Bu örnekte:
 *  - 1 üretici thread dosyadan satır okur
 *  - N tüketici thread satırları paralel parse eder
 *  - Sonuçlar thread-safe depoya eklenir
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 01_csv 01_paralel_csv_oku.cpp
 *
 * Test: Önce test_csv.csv oluşturulur, sonra okunur.
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <shared_mutex>

// ThreadSafeQueue (Konu 1'den)
template <typename T>
class ThreadSafeQueue {
    mutable std::mutex m_;
    std::condition_variable cv_;
    std::queue<T> kuyruk_;
    bool kapali_ = false;
public:
    void push(T d) {
        {
            std::lock_guard<std::mutex> g(m_);
            kuyruk_.push(std::move(d));
        }
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

// Thread-safe depo
class Depo {
    mutable std::shared_mutex sm_;
    std::unordered_map<int, std::shared_ptr<Ogrenci>> kayitlar_;
public:
    void ekle(int id, std::shared_ptr<Ogrenci> o) {
        std::unique_lock<std::shared_mutex> g(sm_);
        kayitlar_[id] = std::move(o);
    }
    std::size_t boyut() const {
        std::shared_lock<std::shared_mutex> g(sm_);
        return kayitlar_.size();
    }
};

// CSV parse
Ogrenci parse(const std::string& satir) {
    Ogrenci o;
    std::istringstream iss(satir);
    std::string alan;

    std::getline(iss, alan, ',');
    o.numara = std::stoi(alan);
    std::getline(iss, o.ad, ',');
    std::getline(iss, o.bolum, ',');
    std::getline(iss, alan, ',');
    o.gpa = std::stod(alan);

    return o;
}

// Test verisi oluştur
void testVerisiOlustur(const std::string& yol, int n) {
    std::ofstream f(yol);
    f << "numara,ad,bolum,gpa\n";
    for (int i = 1; i <= n; ++i) {
        f << (100000 + i) << ",Ogrenci_" << i << ",Bilgisayar,"
          << (2.0 + (i % 200) * 0.01) << '\n';
    }
}

void csvIceAktar(const std::string& yol, Depo& depo) {
    ThreadSafeQueue<std::string> kuyruk;
    std::atomic<std::size_t> hataSayisi{0};

    // Üretici
    std::thread uretici([&] {
        std::ifstream dosya(yol);
        std::string satir;
        std::getline(dosya, satir);  // başlık
        while (std::getline(dosya, satir)) {
            kuyruk.push(std::move(satir));
        }
        kuyruk.kapat();
    });

    // Tüketiciler
    auto isciSayisi = std::max(2u, std::thread::hardware_concurrency() - 1);
    std::vector<std::thread> isciler;
    for (unsigned i = 0; i < isciSayisi; ++i) {
        isciler.emplace_back([&] {
            std::string satir;
            while (kuyruk.waitAndPop(satir)) {
                try {
                    auto o = parse(satir);
                    depo.ekle(o.numara, std::make_shared<Ogrenci>(std::move(o)));
                } catch (...) {
                    hataSayisi.fetch_add(1);
                }
            }
        });
    }

    uretici.join();
    for (auto& t : isciler) t.join();

    std::cout << "Hata sayisi: " << hataSayisi.load() << '\n';
}

int main() {
    const std::string yol = "/tmp/test_csv_b11.csv";
    constexpr int N = 50'000;

    testVerisiOlustur(yol, N);

    Depo depo;

    auto t0 = std::chrono::steady_clock::now();
    csvIceAktar(yol, depo);
    auto t1 = std::chrono::steady_clock::now();

    auto sure = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "Yuklenen ogrenci: " << depo.boyut() << '\n';
    std::cout << "Sure: " << sure.count() << " ms\n";
    std::cout << "Hiz: " << (depo.boyut() * 1000 / sure.count()) << " satir/sn\n";

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
Hata sayisi: 0
Yuklenen ogrenci: 50000
Sure: 200-400 ms
Hiz: ~150K satir/sn
*/

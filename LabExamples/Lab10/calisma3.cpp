/**
 * Lab 10 - Çalışma 3: Basit Thread Pool İnşası
 *
 * TODO işaretli yerleri doldurarak ThreadPool sınıfını tamamlayın.
 *
 * Derleme: g++ -std=c++20 -pthread -o calisma3 calisma3.cpp
 * Çalıştırma: ./calisma3
 */

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <iostream>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t n) {
        if (n == 0) n = 1;
        // TODO (Gorev 3.1): n kadar isci yarat
        //   for i in 0..n:
        //     isciler_.emplace_back(&ThreadPool::isciDongusu, this);
    }

    ~ThreadPool() {
        // TODO (Gorev 3.4): Destructor
        //   1. { lock_guard, kapaniyor_ = true; }
        //   2. cv_.notify_all();
        //   3. for each isci: join
    }

    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>
    {
        using R = std::invoke_result_t<F, Args...>;

        // TODO (Gorev 3.3): submit icini yaz
        //   auto gorev = std::make_shared<std::packaged_task<R()>>(
        //       std::bind(std::forward<F>(f),
        //                 std::forward<Args>(args)...));
        //   std::future<R> gelecek = gorev->get_future();
        //   { lock_guard; kuyruk_.push([gorev]{ (*gorev)(); }); }
        //   cv_.notify_one();
        //   return gelecek;

        std::future<R> bos;  // Yer tutucu; doldurulacak
        return bos;
    }

private:
    void isciDongusu() {
        // TODO (Gorev 3.2): Isci dongusu
        //   while (true):
        //     std::function<void()> gorev;
        //     { unique_lock kilit(mtx_);
        //       cv_.wait(kilit, [this] {
        //         return !kuyruk_.empty() || kapaniyor_; });
        //       if (kuyruk_.empty() && kapaniyor_) return;
        //       gorev = std::move(kuyruk_.front());
        //       kuyruk_.pop();
        //     }
        //     gorev();
    }

    std::vector<std::thread> isciler_;
    std::queue<std::function<void()>> kuyruk_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool kapaniyor_ = false;
};

int main() {
    ThreadPool havuz(4);

    // TODO (Gorev 3.4): 16 gorev submit et
    //   std::vector<std::future<int>> f;
    //   for i in 0..16:
    //     f.push_back(havuz.submit([i] { return i * i; }));
    //   long long toplam = 0;
    //   for (auto& x : f) toplam += x.get();
    //   std::cout << "Toplam kareler: " << toplam << "\n";
    //   // Beklenen: 1240

    return 0;
}

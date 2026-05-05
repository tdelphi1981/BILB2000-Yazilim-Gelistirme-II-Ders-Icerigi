/**
 * İşlem Logu - Append-only Audit Log (Üretici/Tüketici)
 *
 * Bu örnek, sunucu içinde gerçekleşen kritik işlemleri (kullanıcı
 * girişi, kayıt güncelleme, silme, vb.) eşzamanlı olarak ASCII log
 * dosyasına yazan minimal bir audit-log altyapısı kurar. Birden fazla
 * üretici thread, log kayıtlarını paylaşılan kuyruğa atar; tek bir
 * tüketici thread kuyruğu boşaltıp dosyaya yazar. Böylece dosya I/O
 * kilidi bir noktaya çekilir, üretici thread'ler asla disk için
 * beklemez.
 *
 * OBYS bağlamında: Her dersi düzenleme, not girme, öğrenci silme gibi
 * işlemler audit_log.txt dosyasına zaman damgalı olarak işlenir.
 *
 * Öğrenilecek kavramlar:
 * - condition_variable ile uyandırma
 * - Üretici/tüketici desen
 * - Tek tüketici dosya yazımı = sıralı, kilitsiz I/O
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 3 - Paylaşılan Durum Yönetimi
 *
 * Derleme: g++ -std=c++20 -pthread -o ornek 02_islem_log.cpp
 * Çalıştırma: ./ornek
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct LogKaydi {
    std::chrono::system_clock::time_point zaman;
    std::string seviye;
    std::string mesaj;
};

class AuditLog {
public:
    explicit AuditLog(const std::string& dosya_adi)
        : dosya_(dosya_adi, std::ios::app) {
        if (!dosya_.is_open()) {
            throw std::runtime_error("Log dosyası açılamadı: " + dosya_adi);
        }
        tuketici_ = std::thread(&AuditLog::yazici_dongusu, this);
    }

    ~AuditLog() {
        kapat();
    }

    void kayit_at(const std::string& seviye, const std::string& mesaj) {
        {
            std::lock_guard kilit(kilit_);
            kuyruk_.push({std::chrono::system_clock::now(), seviye, mesaj});
        }
        sinyal_.notify_one();
    }

    void kapat() {
        if (calisiyor_.exchange(false) == false) return;
        sinyal_.notify_all();
        if (tuketici_.joinable()) tuketici_.join();
        if (dosya_.is_open()) dosya_.close();
    }

private:
    void yazici_dongusu() {
        while (calisiyor_.load() || !kuyruk_bos()) {
            std::unique_lock kilit(kilit_);
            sinyal_.wait(kilit, [this] {
                return !kuyruk_.empty() || !calisiyor_.load();
            });

            while (!kuyruk_.empty()) {
                const auto kayit = std::move(kuyruk_.front());
                kuyruk_.pop();
                kilit.unlock();
                yaz(kayit);
                kilit.lock();
            }
        }
    }

    bool kuyruk_bos() {
        std::lock_guard k(kilit_);
        return kuyruk_.empty();
    }

    void yaz(const LogKaydi& k) {
        const std::time_t t = std::chrono::system_clock::to_time_t(k.zaman);
        std::tm tm_buf{};
        ::localtime_r(&t, &tm_buf);

        std::ostringstream s;
        s << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S")
          << " [" << k.seviye << "] " << k.mesaj << '\n';
        dosya_ << s.str();
        dosya_.flush();
    }

    std::ofstream dosya_;
    std::queue<LogKaydi> kuyruk_;
    std::mutex kilit_;
    std::condition_variable sinyal_;
    std::atomic<bool> calisiyor_{true};
    std::thread tuketici_;
};

int main() {
    AuditLog log("audit_log.txt");

    auto uretici = [&log](int isci_no) {
        for (int i = 1; i <= 10; ++i) {
            log.kayit_at("INFO",
                "İşçi " + std::to_string(isci_no)
                + " işlem #" + std::to_string(i) + " bitirdi.");
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    };

    std::vector<std::thread> ureticiler;
    for (int i = 1; i <= 4; ++i) {
        ureticiler.emplace_back(uretici, i);
    }
    for (auto& t : ureticiler) t.join();

    log.kapat();
    std::cout << "audit_log.txt dosyasına 40 kayıt yazıldı.\n";
    return 0;
}

/*
BEKLENEN ÇIKTI:
---------------
audit_log.txt dosyasına 40 kayıt yazıldı.

audit_log.txt içeriği:
2026-05-05 14:30:00 [INFO] İşçi 1 işlem #1 bitirdi.
2026-05-05 14:30:00 [INFO] İşçi 2 işlem #1 bitirdi.
...
*/

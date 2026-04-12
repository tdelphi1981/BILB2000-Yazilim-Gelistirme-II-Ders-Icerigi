/**
 * std::unique_lock ile Esnek Kilitleme
 *
 * Bu program, std::unique_lock'un ertelemeli kilitleme (deferred locking)
 * ve manuel kilit acma/kapama ozelliklerini gosterir. OBYS senaryosunda
 * ogrenci verisine okuma ve yazma erisimine kosullu kilitleme uygulanir.
 *
 * Ogrenilecek kavramlar:
 * - std::unique_lock ve std::defer_lock
 * - Manuel lock() / unlock()
 * - try_lock() ile deneme kilitleme
 * - Okuma vs yazma erisimi icin kosullu kilitleme
 *
 * Bolum: 09 - Cok Kanalli Programlamaya Giris
 * Unite: 3 - std::mutex ve Senkronizasyon
 *
 * Derleme: g++ -std=c++20 -pthread -o 03_unique_lock 03_unique_lock.cpp
 * Calistirma: ./03_unique_lock
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <chrono>

// OBYS ogrenci verisi
struct OgrenciKayit {
    int numara;
    std::string isim;
    double not_ortalamasi;
};

// Paylasilan veri ve mutex
std::vector<OgrenciKayit> kayitlar;
std::mutex veri_mutex;

// -----------------------------------------------------------
// 1. Ertelemeli kilitleme ile yazma islemi
// -----------------------------------------------------------
void ogrenci_ekle(int numara, const std::string& isim, double ort) {
    // Kilidi hemen alma, ertele
    std::unique_lock<std::mutex> kilit(veri_mutex, std::defer_lock);

    // Oncelik: veriyi hazirla (kilit gereksiz)
    OgrenciKayit yeni{numara, isim, ort};
    std::cout << "  [HAZIRLA] " << isim << " kaydi hazirlandi" << '\n';

    // Simdi kilitle ve yaz
    kilit.lock();
    kayitlar.push_back(yeni);
    std::cout << "  [YAZ] " << isim << " eklendi (kilit acildi)" << '\n';
    // kilit kapsam sonunda otomatik acilir
}

// -----------------------------------------------------------
// 2. Manuel kilit acma ile okuma islemi
// -----------------------------------------------------------
void ogrenci_oku(int numara) {
    std::unique_lock<std::mutex> kilit(veri_mutex);

    // Veriyi bul (kilit altinda)
    OgrenciKayit bulunan{0, "", 0.0};
    bool bulundu = false;

    for (const auto& k : kayitlar) {
        if (k.numara == numara) {
            bulunan = k;
            bulundu = true;
            break;
        }
    }

    // Kritik bolge bitti, kilidi erken ac
    kilit.unlock();

    // Artik kilitsiz calismaya devam (ekrana yazdirma kilitsiz)
    if (bulundu) {
        std::cout << "  [OKU] #" << bulunan.numara << " "
                  << bulunan.isim << " GPA: "
                  << bulunan.not_ortalamasi << '\n';
    } else {
        std::cout << "  [OKU] #" << numara << " bulunamadi" << '\n';
    }
}

// -----------------------------------------------------------
// 3. try_lock ile deneme kilitleme
// -----------------------------------------------------------
void not_guncelle(int numara, double yeni_not) {
    std::unique_lock<std::mutex> kilit(veri_mutex, std::defer_lock);

    // Kilidi deneyerek al
    if (kilit.try_lock()) {
        for (auto& k : kayitlar) {
            if (k.numara == numara) {
                double eski = k.not_ortalamasi;
                k.not_ortalamasi = yeni_not;
                std::cout << "  [GUNCELLE] #" << numara
                          << " GPA: " << eski << " -> " << yeni_not
                          << '\n';
                return;
            }
        }
        std::cout << "  [GUNCELLE] #" << numara << " bulunamadi" << '\n';
    } else {
        // Kilit alinamazsa beklemeden gec
        std::cout << "  [GUNCELLE] #" << numara
                  << " kilit mesgul, atlanacak" << '\n';
    }
}

int main() {
    // ================================================================
    // Senaryo 1: Ertelemeli kilitleme ile veri ekleme
    // ================================================================
    std::cout << "=== 1. Ertelemeli Kilitleme (defer_lock) ===" << '\n';

    std::thread t1(ogrenci_ekle, 2001, "Ali Yilmaz", 3.45);
    std::thread t2(ogrenci_ekle, 2002, "Ayse Kaya", 3.82);
    std::thread t3(ogrenci_ekle, 2003, "Mehmet Demir", 2.91);

    t1.join();
    t2.join();
    t3.join();

    // ================================================================
    // Senaryo 2: Manuel kilit acma ile okuma
    // ================================================================
    std::cout << "\n=== 2. Manuel Kilit Acma (unlock) ===" << '\n';

    std::thread t4(ogrenci_oku, 2001);
    std::thread t5(ogrenci_oku, 2002);
    std::thread t6(ogrenci_oku, 9999); // Bulunmayacak

    t4.join();
    t5.join();
    t6.join();

    // ================================================================
    // Senaryo 3: try_lock ile not guncelleme
    // ================================================================
    std::cout << "\n=== 3. Deneme Kilitleme (try_lock) ===" << '\n';

    std::thread t7(not_guncelle, 2001, 3.65);
    std::thread t8(not_guncelle, 2002, 3.90);

    t7.join();
    t8.join();

    // ================================================================
    // Sonuc tablosu
    // ================================================================
    std::cout << "\n=== Guncel Kayitlar ===" << '\n';
    for (const auto& k : kayitlar) {
        std::cout << "  #" << k.numara << " " << k.isim
                  << " GPA: " << k.not_ortalamasi << '\n';
    }

    // ================================================================
    // unique_lock vs lock_guard karsilastirmasi
    // ================================================================
    std::cout << "\n--- unique_lock vs lock_guard ---" << '\n';
    std::cout << "  lock_guard : Basit, her zaman kilitle (RAII)" << '\n';
    std::cout << "  unique_lock: Ertelemeli, manuel, try_lock destegi" << '\n';
    std::cout << "  Kural: Basit durumlarda lock_guard, "
              << "esneklik gerekiyorsa unique_lock kullan." << '\n';

    return 0;
}

/*
 * BEKLENEN CIKTI (sira degisir):
 * ------------------------------
 * === 1. Ertelemeli Kilitleme (defer_lock) ===
 *   [HAZIRLA] Ali Yilmaz kaydi hazirlandi
 *   [YAZ] Ali Yilmaz eklendi (kilit acildi)
 *   [HAZIRLA] Ayse Kaya kaydi hazirlandi
 *   [YAZ] Ayse Kaya eklendi (kilit acildi)
 *   [HAZIRLA] Mehmet Demir kaydi hazirlandi
 *   [YAZ] Mehmet Demir eklendi (kilit acildi)
 *
 * === 2. Manuel Kilit Acma (unlock) ===
 *   [OKU] #2001 Ali Yilmaz GPA: 3.45
 *   [OKU] #2002 Ayse Kaya GPA: 3.82
 *   [OKU] #9999 bulunamadi
 *
 * === 3. Deneme Kilitleme (try_lock) ===
 *   [GUNCELLE] #2001 GPA: 3.45 -> 3.65
 *   [GUNCELLE] #2002 GPA: 3.82 -> 3.9
 *
 * === Guncel Kayitlar ===
 *   #2001 Ali Yilmaz GPA: 3.65
 *   #2002 Ayse Kaya GPA: 3.9
 *   #2003 Mehmet Demir GPA: 2.91
 *
 * --- unique_lock vs lock_guard ---
 *   lock_guard : Basit, her zaman kilitle (RAII)
 *   unique_lock: Ertelemeli, manuel, try_lock destegi
 *   Kural: Basit durumlarda lock_guard, esneklik gerekiyorsa unique_lock kullan.
 */

/**
 * Lab 01 - Çalışma 4 Başlangıç Kodu
 * unordered_map ve Uyarlayıcılar
 *
 * Bu dosya Lab 01 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - std::map vs std::unordered_map performans farkı
 * - Hash tabloları ve O(1) arama
 * - std::stack ile geri alma (undo) mekanizması
 * - std::priority_queue ile öncelik sırası
 *
 * Lab: 01 - STL Kapsayıcılara Giriş
 * Çalışma: 4 - unordered_map ve Uyarlayıcılar (25 dk)
 *
 * Derleme: g++ -std=c++20 -o calisma4 calisma4.cpp
 * Çalıştırma: ./calisma4
 */

#include <iostream>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>
#include <string>
#include <chrono>

int main() {
    const int N = 500000;

    std::cout << "=== Calisma 4: unordered_map ve Uyarlayicilar ===\n\n";

    // map vs unordered_map arama karşılaştırması
    std::map<int, int> m;
    std::unordered_map<int, int> um;

    for (int i = 0; i < N; ++i) {
        m[i] = i * 2;
        um[i] = i * 2;
    }

    // map arama
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) m.find(i);
    auto t2 = std::chrono::high_resolution_clock::now();

    // unordered_map arama
    auto t3 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) um.find(i);
    auto t4 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> d1 = t2 - t1;
    std::chrono::duration<double, std::milli> d2 = t4 - t3;

    std::cout << "--- map vs unordered_map Arama (N=" << N << ") ---\n";
    std::cout << "map arama:           " << d1.count() << " ms\n";
    std::cout << "unordered_map arama: " << d2.count() << " ms\n";
    std::cout << "Hiz farki: " << d1.count() / d2.count() << "x\n";

    // === GOREV 4.1 - Performans Analizi (5 dk) ===
    std::cout << "\n[GOREV 4.1]\n";
    // TODO: a) Programı çalıştırın ve unordered_map'in kaç kat
    //          daha hızlı olduğunu kaydedin
    //
    // TODO: b) N değerini 1000'e düşürün ve farkı tekrar ölçün.
    //          Fark hâlâ belirgin mi? Neden?
    //          (İpucu: Küçük N değerlerinde hash hesaplama maliyeti
    //           log(N) ile karşılaştırıldığında önemli olabilir)
    // ============================================

    // === GOREV 4.2 - stack ile Geri Alma (10 dk) ===
    std::cout << "\n[GOREV 4.2]\n";
    // TODO: a) std::stack<std::string> islem_gecmisi tanımlayın
    //
    // TODO: b) Her öğrenci ekleme/silme işlemini stack'e kaydedin:
    //          islem_gecmisi.push("Eklendi: 2001 Ali Yilmaz");
    //          islem_gecmisi.push("Eklendi: 2002 Ayse Kaya");
    //          islem_gecmisi.push("Silindi: 2003 Mehmet Demir");
    //
    // TODO: c) geriAl() fonksiyonu yazın: son işlemi gösterip
    //          stack'ten çıkarsın
    //          [!] pop() değer döndürmez! Önce top() ile okuyun.
    //          void geriAl(std::stack<std::string>& gecmis) {
    //              if (gecmis.empty()) { ... return; }
    //              std::cout << "Geri alindi: " << gecmis.top() << '\n';
    //              gecmis.pop();
    //          }
    //
    // TODO: d) 3 işlem yapın, 2 kez geri alın, stack durumunu yazdırın
    // ============================================

    // === GOREV 4.3 - Bonus: priority_queue ile Acil İşlem Sırası (10 dk) ===
    std::cout << "\n[GOREV 4.3]\n";
    // TODO: a) Talep struct'ı tanımlayın:
    //          struct Talep {
    //              int oncelik;  // yüksek = daha acil
    //              std::string aciklama;
    //              bool operator<(const Talep& t) const {
    //                  return oncelik < t.oncelik;
    //              }
    //          };
    //
    // TODO: b) 5 talep ekleyin (farklı önceliklerle):
    //          std::priority_queue<Talep> talepler;
    //          talepler.push({3, "Not duzeltme talebi"});
    //          talepler.push({5, "Mezuniyet basvurusu"});
    //          ...
    //
    // TODO: c) Tüm talepleri öncelik sırasında işleyin:
    //          while (!talepler.empty()) {
    //              std::cout << talepler.top().aciklama << '\n';
    //              talepler.pop();
    //          }
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 4: unordered_map ve Uyarlayicilar ===

--- map vs unordered_map Arama (N=500000) ---
map arama:           XX.XX ms
unordered_map arama: X.XX ms
Hiz farki: X.Xx

[GOREV 4.1]

[GOREV 4.2]

[GOREV 4.3]
*/

/**
 * OBYS String Islemleri - Kapsamli Entegrasyon Ornegi
 *
 * Bu program string_view, std::format, from_chars ve STL
 * algoritmalarini bir OBYS senaryosunda birlestirir.
 * Unite 2'nin capstone (butunlestirme) ornegidir.
 *
 * Ogrenilecek kavramlar:
 * - string_view ile arama ve filtreleme
 * - std::format ile raporlama ve tablo olusturma
 * - from_chars ile girdi dogrulama
 * - STL algoritmalari ile veri isleme
 * - Hepsinin bir OBYS senaryosunda birlesmesi
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 2 - String Islemleri
 *
 * Derleme: g++ -std=c++20 -o 04_obys_string_islemleri 04_obys_string_islemleri.cpp
 * Calistirma: ./04_obys_string_islemleri
 */

#include <iostream>
#include <string>
#include <string_view>
#include <format>
#include <vector>
#include <algorithm>
#include <charconv>
#include <numeric>

struct Ogrenci {
    int numara;
    std::string isim;
    std::string bolum;
    double gpa;
};

// --- string_view ile arama (kopyalama yok) ---
std::vector<const Ogrenci*> ara(const std::vector<Ogrenci>& liste,
                                 std::string_view anahtar) {
    std::vector<const Ogrenci*> sonuclar;
    for (const auto& o : liste) {
        // Isim veya bolum icinde arama
        if (o.isim.find(anahtar) != std::string::npos ||
            o.bolum.find(anahtar) != std::string::npos) {
            sonuclar.push_back(&o);
        }
    }
    return sonuclar;
}

// --- Girdi dogrulama (from_chars tabanli) ---
struct DogrulamaSonucu {
    bool gecerli;
    std::string mesaj;
};

DogrulamaSonucu numara_dogrula(std::string_view sv) {
    if (sv.empty()) return {false, "Bos girdi"};
    bool hepsi_rakam = std::all_of(sv.begin(), sv.end(),
        [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
    if (!hepsi_rakam) return {false, "Sadece rakam girilmeli"};
    int val = 0;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);
    if (ec != std::errc{}) return {false, "Sayi donusumu basarisiz"};
    if (val < 1000 || val > 9999) return {false, "1000-9999 araliginda olmali"};
    return {true, std::format("Gecerli: {}", val)};
}

// --- std::format ile tablo basligini olustur ---
void tablo_basligi_yazdir() {
    std::cout << std::format("  {:<5} | {:<18} | {:<14} | {:>5}\n",
                             "No", "Isim", "Bolum", "GPA");
    std::cout << std::format("  {:-<5}-+-{:-<18}-+-{:-<14}-+-{:->5}\n",
                             "", "", "", "");
}

void ogrenci_satiri_yazdir(const Ogrenci& o) {
    std::cout << std::format("  {:<5} | {:<18} | {:<14} | {:>5.2f}\n",
                             o.numara, o.isim, o.bolum, o.gpa);
}

int main() {
    std::cout << "=== OBYS String Islemleri - Tam Entegrasyon ===\n\n";

    // --- Ogrenci veritabani ---
    std::vector<Ogrenci> ogrenciler = {
        {1001, "Ali Yilmaz",     "Bilgisayar",  3.45},
        {1002, "Ayse Kaya",      "Bilgisayar",  3.80},
        {1003, "Mehmet Demir",   "Elektrik",    2.90},
        {1004, "Zeynep Can",     "Bilgisayar",  3.65},
        {1005, "Can Arslan",     "Makine",      2.10},
        {1006, "Elif Yildiz",    "Bilgisayar",  3.92},
        {1007, "Burak Ozturk",   "Elektrik",    2.75},
        {1008, "Selin Aktas",    "Bilgisayar",  3.50},
    };

    // --- 1. Girdi dogrulama ---
    std::cout << "--- 1. Girdi Dogrulama ---\n";
    std::vector<std::string_view> test_numaralari = {"1001", "abc", "50", "5555"};
    for (auto sv : test_numaralari) {
        auto sonuc = numara_dogrula(sv);
        std::cout << std::format("  \"{}\": {} {}\n",
                                 sv,
                                 sonuc.gecerli ? "[OK]" : "[HATA]",
                                 sonuc.mesaj);
    }

    // --- 2. Tam ogrenci listesi (format ile tablo) ---
    std::cout << "\n--- 2. Ogrenci Listesi ---\n";
    tablo_basligi_yazdir();
    for (const auto& o : ogrenciler) {
        ogrenci_satiri_yazdir(o);
    }

    // --- 3. string_view ile arama ---
    std::cout << "\n--- 3. Arama Sonuclari ---\n";

    // "Bilgisayar" araması
    std::string_view arama_terimi = "Bilgisayar";
    auto sonuclar = ara(ogrenciler, arama_terimi);

    std::cout << std::format("  Arama: \"{}\" ({} sonuc)\n",
                             arama_terimi, sonuclar.size());
    tablo_basligi_yazdir();
    for (const auto* o : sonuclar) {
        ogrenci_satiri_yazdir(*o);
    }

    // "Ali" araması
    std::cout << "\n";
    arama_terimi = "Ali";
    sonuclar = ara(ogrenciler, arama_terimi);
    std::cout << std::format("  Arama: \"{}\" ({} sonuc)\n",
                             arama_terimi, sonuclar.size());
    for (const auto* o : sonuclar) {
        std::cout << std::format("    {} - {}\n", o->numara, o->isim);
    }

    // --- 4. Istatistik raporu (format ile) ---
    std::cout << "\n--- 4. Istatistik Raporu ---\n";

    double toplam = std::accumulate(ogrenciler.begin(), ogrenciler.end(), 0.0,
        [](double sum, const Ogrenci& o) { return sum + o.gpa; });
    double ortalama = toplam / static_cast<double>(ogrenciler.size());

    auto [min_it, max_it] = std::minmax_element(
        ogrenciler.begin(), ogrenciler.end(),
        [](const Ogrenci& a, const Ogrenci& b) { return a.gpa < b.gpa; });

    int basarili = std::count_if(ogrenciler.begin(), ogrenciler.end(),
        [](const Ogrenci& o) { return o.gpa >= 2.0; });

    std::cout << std::format("  {:=^42}\n", " ISTATISTIKLER ");
    std::cout << std::format("  {:<25} {:>5}\n",
                             "Toplam ogrenci:", ogrenciler.size());
    std::cout << std::format("  {:<25} {:>5.2f}\n",
                             "Genel ortalama:", ortalama);
    std::cout << std::format("  {:<25} {} ({:.2f})\n",
                             "En yuksek GPA:", max_it->isim, max_it->gpa);
    std::cout << std::format("  {:<25} {} ({:.2f})\n",
                             "En dusuk GPA:", min_it->isim, min_it->gpa);
    std::cout << std::format("  {:<25} {:>5}\n",
                             "Basarili (GPA >= 2.0):", basarili);
    std::cout << std::format("  {:<25} {:>5}\n",
                             "Basarisiz (GPA < 2.0):",
                             static_cast<int>(ogrenciler.size()) - basarili);

    // --- 5. Onur listesi (sirali, formatli) ---
    std::cout << "\n--- 5. Onur Listesi (GPA >= 3.5) ---\n";

    // Onur ogrencilerini kopyala ve sirala
    std::vector<const Ogrenci*> onur;
    for (const auto& o : ogrenciler) {
        if (o.gpa >= 3.5) {
            onur.push_back(&o);
        }
    }
    std::sort(onur.begin(), onur.end(),
        [](const Ogrenci* a, const Ogrenci* b) { return a->gpa > b->gpa; });

    int sira = 1;
    for (const auto* o : onur) {
        std::cout << std::format("  {}. {} - {} (GPA: {:.2f})\n",
                                 sira++, o->numara, o->isim, o->gpa);
    }

    // --- 6. Bolum bazli ozet ---
    std::cout << "\n--- 6. Bolum Bazli Ozet ---\n";
    std::cout << std::format("  {:<14} | {:>5} | {:>8}\n",
                             "Bolum", "Sayi", "Ort GPA");
    std::cout << std::format("  {:-<14}-+-{:->5}-+-{:->8}\n", "", "", "");

    for (std::string_view bolum : {"Bilgisayar", "Elektrik", "Makine"}) {
        int sayi = 0;
        double btoplam = 0.0;
        for (const auto& o : ogrenciler) {
            if (o.bolum == bolum) {
                sayi++;
                btoplam += o.gpa;
            }
        }
        if (sayi > 0) {
            std::cout << std::format("  {:<14} | {:>5} | {:>8.2f}\n",
                                     bolum, sayi, btoplam / sayi);
        }
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === OBYS String Islemleri - Tam Entegrasyon ===
 *
 * --- 1. Girdi Dogrulama ---
 *   "1001": [OK] Gecerli: 1001
 *   "abc": [HATA] Sadece rakam girilmeli
 *   "50": [HATA] 1000-9999 araliginda olmali
 *   "5555": [OK] Gecerli: 5555
 *
 * --- 2. Ogrenci Listesi ---
 *   No    | Isim               | Bolum          |   GPA
 *   ------+--------------------+----------------+------
 *   1001  | Ali Yilmaz         | Bilgisayar     |  3.45
 *   1002  | Ayse Kaya          | Bilgisayar     |  3.80
 *   1003  | Mehmet Demir       | Elektrik       |  2.90
 *   1004  | Zeynep Can         | Bilgisayar     |  3.65
 *   1005  | Can Arslan         | Makine         |  2.10
 *   1006  | Elif Yildiz        | Bilgisayar     |  3.92
 *   1007  | Burak Ozturk       | Elektrik       |  2.75
 *   1008  | Selin Aktas        | Bilgisayar     |  3.50
 *
 * --- 3. Arama Sonuclari ---
 *   Arama: "Bilgisayar" (5 sonuc)
 *   No    | Isim               | Bolum          |   GPA
 *   ------+--------------------+----------------+------
 *   1001  | Ali Yilmaz         | Bilgisayar     |  3.45
 *   1002  | Ayse Kaya          | Bilgisayar     |  3.80
 *   1004  | Zeynep Can         | Bilgisayar     |  3.65
 *   1006  | Elif Yildiz        | Bilgisayar     |  3.92
 *   1008  | Selin Aktas        | Bilgisayar     |  3.50
 *
 *   Arama: "Ali" (1 sonuc)
 *     1001 - Ali Yilmaz
 *
 * --- 4. Istatistik Raporu ---
 *   ============= ISTATISTIKLER ==============
 *   Toplam ogrenci:               8
 *   Genel ortalama:            3.26
 *   En yuksek GPA:       Elif Yildiz (3.92)
 *   En dusuk GPA:        Can Arslan (2.10)
 *   Basarili (GPA >= 2.0):        8
 *   Basarisiz (GPA < 2.0):       0
 *
 * --- 5. Onur Listesi (GPA >= 3.5) ---
 *   1. 1006 - Elif Yildiz (GPA: 3.92)
 *   2. 1002 - Ayse Kaya (GPA: 3.80)
 *   3. 1004 - Zeynep Can (GPA: 3.65)
 *   4. 1008 - Selin Aktas (GPA: 3.50)
 *
 * --- 6. Bolum Bazli Ozet ---
 *   Bolum          | Sayi | Ort GPA
 *   ---------------+------+---------
 *   Bilgisayar     |     5 |     3.66
 *   Elektrik       |     2 |     2.82
 *   Makine         |     1 |     2.10
 */

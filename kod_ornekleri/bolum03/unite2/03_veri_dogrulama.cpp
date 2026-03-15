/**
 * Veri Dogrulama - String Tabanli Girdi Dogrulama
 *
 * Bu program modern C++ ile veri dogrulama tekniklerini gosterir.
 * STL algoritmalari, from_chars/to_chars ve regex kullanarak
 * giris verilerinin gecerliligi kontrol edilir.
 *
 * Ogrenilecek kavramlar:
 * - std::all_of, any_of, none_of ile karakter dogrulama
 * - std::from_chars / std::to_chars kullanimi
 * - from_chars ile hata yonetimi
 * - std::regex ile eposta ve telefon dogrulama
 * - OBYS: Girdi dogrulama katmani
 * - DogrulamaSonucu struct deseni
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 2 - String Islemleri
 *
 * Derleme: g++ -std=c++20 -o 03_veri_dogrulama 03_veri_dogrulama.cpp
 * Calistirma: ./03_veri_dogrulama
 */

#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <charconv>
#include <regex>
#include <vector>
#include <system_error>

// --- Dogrulama sonucu deseni ---
struct DogrulamaSonucu {
    bool gecerli;
    std::string mesaj;
};

// --- 1. STL algoritmalari ile karakter dogrulama ---
bool sadece_harf(std::string_view sv) {
    return std::all_of(sv.begin(), sv.end(),
        [](char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == ' '; });
}

bool rakam_iceriyor(std::string_view sv) {
    return std::any_of(sv.begin(), sv.end(),
        [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
}

bool bosluk_yok(std::string_view sv) {
    return std::none_of(sv.begin(), sv.end(),
        [](char c) { return std::isspace(static_cast<unsigned char>(c)); });
}

// --- OBYS: Ogrenci numarasi dogrulama ---
DogrulamaSonucu numara_dogrula(std::string_view numara_str) {
    // Bos olmamali
    if (numara_str.empty()) {
        return {false, "Ogrenci numarasi bos olamaz"};
    }
    // Sadece rakam olmali
    bool hepsi_rakam = std::all_of(numara_str.begin(), numara_str.end(),
        [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
    if (!hepsi_rakam) {
        return {false, "Numara sadece rakam icermeli"};
    }
    // from_chars ile sayiya cevir ve aralik kontrol et
    int numara = 0;
    auto [ptr, ec] = std::from_chars(
        numara_str.data(), numara_str.data() + numara_str.size(), numara);
    if (ec != std::errc{}) {
        return {false, "Gecersiz sayi formati"};
    }
    if (numara < 1000 || numara > 9999) {
        return {false, "Numara 1000-9999 araliginda olmali"};
    }
    return {true, "Gecerli ogrenci numarasi"};
}

// --- OBYS: Not dogrulama ---
DogrulamaSonucu not_dogrula(std::string_view not_str) {
    double not_degeri = 0.0;
    // from_chars double icin kullanim
    auto [ptr, ec] = std::from_chars(
        not_str.data(), not_str.data() + not_str.size(), not_degeri);
    if (ec != std::errc{}) {
        return {false, "Gecersiz not formati"};
    }
    if (not_degeri < 0.0 || not_degeri > 4.0) {
        return {false, "Not 0.00 - 4.00 araliginda olmali"};
    }
    return {true, "Gecerli not degeri"};
}

// --- OBYS: Isim dogrulama ---
DogrulamaSonucu isim_dogrula(std::string_view isim) {
    if (isim.empty()) {
        return {false, "Isim bos olamaz"};
    }
    if (isim.size() < 3) {
        return {false, "Isim en az 3 karakter olmali"};
    }
    if (!sadece_harf(isim)) {
        return {false, "Isim sadece harf ve bosluk icermeli"};
    }
    // En az bir bosluk olmali (ad soyad)
    if (isim.find(' ') == std::string_view::npos) {
        return {false, "Ad ve soyad boslukla ayrilmali"};
    }
    return {true, "Gecerli isim"};
}

void sonuc_yazdir(std::string_view alan, std::string_view deger,
                  const DogrulamaSonucu& sonuc) {
    std::cout << "  " << alan << ": \"" << deger << "\" -> "
              << (sonuc.gecerli ? "[OK] " : "[HATA] ")
              << sonuc.mesaj << "\n";
}

int main() {
    std::cout << "=== Veri Dogrulama ===\n\n";

    // --- 1. all_of, any_of, none_of ---
    std::cout << "--- 1. Karakter Dogrulama ---\n";
    std::cout << "  sadece_harf(\"Ali Yilmaz\"): "
              << (sadece_harf("Ali Yilmaz") ? "Evet" : "Hayir") << "\n";
    std::cout << "  sadece_harf(\"Ali123\"): "
              << (sadece_harf("Ali123") ? "Evet" : "Hayir") << "\n";
    std::cout << "  rakam_iceriyor(\"abc123\"): "
              << (rakam_iceriyor("abc123") ? "Evet" : "Hayir") << "\n";
    std::cout << "  bosluk_yok(\"kompakt\"): "
              << (bosluk_yok("kompakt") ? "Evet" : "Hayir") << "\n";

    // --- 2. from_chars / to_chars ---
    std::cout << "\n--- 2. from_chars / to_chars ---\n";

    // from_chars: string -> sayi (hizli, locale-bagimsiz)
    std::string_view sayi_str = "12345";
    int sayi = 0;
    auto [p1, e1] = std::from_chars(
        sayi_str.data(), sayi_str.data() + sayi_str.size(), sayi);
    std::cout << "  from_chars(\"" << sayi_str << "\"): " << sayi << "\n";

    // to_chars: sayi -> string
    char tampon[32];
    auto [p2, e2] = std::to_chars(tampon, tampon + sizeof(tampon), 98765);
    std::cout << "  to_chars(98765): "
              << std::string_view(tampon, p2 - tampon) << "\n";

    // Hata durumu
    std::string_view hatali = "abc";
    int hatali_sayi = 0;
    auto [p3, e3] = std::from_chars(
        hatali.data(), hatali.data() + hatali.size(), hatali_sayi);
    if (e3 != std::errc{}) {
        std::cout << "  from_chars(\"abc\"): Hata - gecersiz girdi\n";
    }

    // --- 3. regex ile dogrulama ---
    std::cout << "\n--- 3. Regex Dogrulama ---\n";

    std::regex eposta_regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    std::regex telefon_regex(R"(\d{3}-\d{3}-\d{4})");

    auto eposta_kontrol = [&](const std::string& e) {
        bool gecerli = std::regex_match(e, eposta_regex);
        std::cout << "  Eposta \"" << e << "\": "
                  << (gecerli ? "[OK]" : "[HATA]") << "\n";
    };

    auto telefon_kontrol = [&](const std::string& t) {
        bool gecerli = std::regex_match(t, telefon_regex);
        std::cout << "  Telefon \"" << t << "\": "
                  << (gecerli ? "[OK]" : "[HATA]") << "\n";
    };

    eposta_kontrol("ali@universite.edu.tr");
    eposta_kontrol("gecersiz-eposta");
    telefon_kontrol("532-123-4567");
    telefon_kontrol("12345");

    // --- 4. OBYS: Girdi dogrulama katmani ---
    std::cout << "\n--- 4. OBYS: Girdi Dogrulama ---\n";

    // Numara dogrulama testleri
    sonuc_yazdir("Numara", "1001", numara_dogrula("1001"));
    sonuc_yazdir("Numara", "abc",  numara_dogrula("abc"));
    sonuc_yazdir("Numara", "99",   numara_dogrula("99"));
    sonuc_yazdir("Numara", "",     numara_dogrula(""));

    std::cout << "\n";

    // Isim dogrulama testleri
    sonuc_yazdir("Isim", "Ali Yilmaz",  isim_dogrula("Ali Yilmaz"));
    sonuc_yazdir("Isim", "Ali",          isim_dogrula("Ali"));
    sonuc_yazdir("Isim", "Ali123 Kaya",  isim_dogrula("Ali123 Kaya"));
    sonuc_yazdir("Isim", "",             isim_dogrula(""));

    std::cout << "\n";

    // Not dogrulama testleri
    sonuc_yazdir("Not", "3.50", not_dogrula("3.50"));
    sonuc_yazdir("Not", "5.00", not_dogrula("5.00"));
    sonuc_yazdir("Not", "xyz",  not_dogrula("xyz"));

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Veri Dogrulama ===
 *
 * --- 1. Karakter Dogrulama ---
 *   sadece_harf("Ali Yilmaz"): Evet
 *   sadece_harf("Ali123"): Hayir
 *   rakam_iceriyor("abc123"): Evet
 *   bosluk_yok("kompakt"): Evet
 *
 * --- 2. from_chars / to_chars ---
 *   from_chars("12345"): 12345
 *   to_chars(98765): 98765
 *   from_chars("abc"): Hata - gecersiz girdi
 *
 * --- 3. Regex Dogrulama ---
 *   Eposta "ali@universite.edu.tr": [OK]
 *   Eposta "gecersiz-eposta": [HATA]
 *   Telefon "532-123-4567": [OK]
 *   Telefon "12345": [HATA]
 *
 * --- 4. OBYS: Girdi Dogrulama ---
 *   Numara: "1001" -> [OK] Gecerli ogrenci numarasi
 *   Numara: "abc" -> [HATA] Numara sadece rakam icermeli
 *   Numara: "99" -> [HATA] Numara 1000-9999 araliginda olmali
 *   Numara: "" -> [HATA] Ogrenci numarasi bos olamaz
 *
 *   Isim: "Ali Yilmaz" -> [OK] Gecerli isim
 *   Isim: "Ali" -> [HATA] Ad ve soyad boslukla ayrilmali
 *   Isim: "Ali123 Kaya" -> [HATA] Isim sadece harf ve bosluk icermeli
 *   Isim: "" -> [HATA] Isim bos olamaz
 *
 *   Not: "3.50" -> [OK] Gecerli not degeri
 *   Not: "5.00" -> [HATA] Not 0.00 - 4.00 araliginda olmali
 *   Not: "xyz" -> [HATA] Gecersiz not formati
 */

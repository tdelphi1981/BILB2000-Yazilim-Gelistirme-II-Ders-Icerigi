/**
 * string_view Temelleri - Sahiplik Almadan String Erisimi
 *
 * Bu program std::string_view kullanimini gosterir. string_view,
 * bir string verisine sahiplik almadan salt-okunur erisim saglar.
 * Kopyalama maliyetini ortadan kaldirarak performansi arttirir.
 *
 * Ogrenilecek kavramlar:
 * - string_view olusturma (string, C-string, literal)
 * - Temel islemler: substr(), find(), starts_with(), ends_with()
 * - remove_prefix(), remove_suffix()
 * - Fonksiyon parametresi olarak string_view
 * - Dangling string_view tehlikesi
 * - OBYS: string_view ile ogrenci arama
 *
 * Bolum: 03 - Modern C++ ve STL Uygulamalari
 * Unite: 2 - String Islemleri
 *
 * Derleme: g++ -std=c++20 -o 01_string_view_temel 01_string_view_temel.cpp
 * Calistirma: ./01_string_view_temel
 */

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// --- string_view deger ile alinir (ucuz kopyalama: pointer + size) ---
void bilgi_yazdir(std::string_view sv) {
    std::cout << "  [" << sv << "] (uzunluk: " << sv.size() << ")\n";
}

// --- OBYS: string_view parametreli arama fonksiyonu ---
struct Ogrenci {
    int numara;
    std::string isim;
    std::string bolum;
};

// string_view ile arama - gereksiz kopyalama yok
bool isim_iceriyor(const Ogrenci& o, std::string_view aranan) {
    return o.isim.find(aranan) != std::string::npos;
}

int main() {
    std::cout << "=== string_view Temelleri ===\n\n";

    // --- 1. Olusturma yontemleri ---
    std::cout << "--- 1. Olusturma Yontemleri ---\n";

    std::string str = "Yazilim Gelistirme";
    std::string_view sv1{str};               // std::string'den
    std::string_view sv2{"Merhaba Dunya"};   // C-string'den
    std::string_view sv3 = "C++20 Modern";   // String literal'den

    bilgi_yazdir(sv1);
    bilgi_yazdir(sv2);
    bilgi_yazdir(sv3);

    // --- 2. Temel islemler ---
    std::cout << "\n--- 2. Temel Islemler ---\n";

    std::string_view metin = "Bilgisayar Bilimleri Bolumu";

    // substr: alt dizi al (kopyalamaz, yeni bir view dondurur)
    std::string_view alt = metin.substr(0, 10);
    std::cout << "  substr(0,10): " << alt << "\n";

    // find: konum bul
    auto pos = metin.find("Bilimleri");
    std::cout << "  'Bilimleri' konumu: " << pos << "\n";

    // starts_with / ends_with (C++20)
    std::cout << "  'Bilgi' ile basliyor mu? "
              << (metin.starts_with("Bilgi") ? "Evet" : "Hayir") << "\n";
    std::cout << "  'Bolumu' ile bitiyor mu? "
              << (metin.ends_with("Bolumu") ? "Evet" : "Hayir") << "\n";

    // --- 3. remove_prefix / remove_suffix ---
    std::cout << "\n--- 3. remove_prefix / remove_suffix ---\n";

    std::string_view dosya_yolu = "/home/ogrenci/proje/main.cpp";
    std::cout << "  Orijinal: " << dosya_yolu << "\n";

    // Bastan 6 karakter cikar: "/home/" kismini at
    std::string_view kirpilmis = dosya_yolu;
    kirpilmis.remove_prefix(6);
    std::cout << "  remove_prefix(6): " << kirpilmis << "\n";

    // Sondan 4 karakter cikar: ".cpp" kismini at
    std::string_view uzantisiz = dosya_yolu;
    uzantisiz.remove_suffix(4);
    std::cout << "  remove_suffix(4): " << uzantisiz << "\n";

    // --- 4. Fonksiyon parametresi olarak ---
    std::cout << "\n--- 4. Fonksiyon Parametresi ---\n";

    std::string isim = "Ali Yilmaz";
    const char* c_isim = "Ayse Kaya";

    // Her uc tur de sorunsuz gecilir
    std::cout << "  string'den:";
    bilgi_yazdir(isim);
    std::cout << "  C-string'den:";
    bilgi_yazdir(c_isim);
    std::cout << "  literal'den:";
    bilgi_yazdir("Mehmet Demir");

    // --- 5. Dangling string_view tehlikesi ---
    std::cout << "\n--- 5. Dangling Tehlikesi ---\n";
    std::cout << "  [UYARI] Asagidaki kod tehlikelidir:\n";
    std::cout << "  // std::string_view tehlikeli() {\n";
    std::cout << "  //     std::string gecici = \"Gecici veri\";\n";
    std::cout << "  //     return std::string_view{gecici}; // TEHLIKE!\n";
    std::cout << "  // } // gecici yok edilir, view sarkan referans olur\n";
    std::cout << "  string_view sahiplik almaz, kaynak yasadigi surece gecerlidir.\n";

    // --- 6. OBYS: string_view ile ogrenci arama ---
    std::cout << "\n--- 6. OBYS: Ogrenci Arama ---\n";

    std::vector<Ogrenci> ogrenciler = {
        {1001, "Ali Yilmaz",     "Bilgisayar"},
        {1002, "Ayse Kaya",      "Elektrik"},
        {1003, "Mehmet Demir",   "Bilgisayar"},
        {1004, "Zeynep Can",     "Makine"},
        {1005, "Ali Arslan",     "Bilgisayar"}
    };

    // string_view parametresi: ne string ne de C-string kopyalanir
    std::string_view aranan = "Ali";
    std::cout << "  '" << aranan << "' iceren ogrenciler:\n";
    for (const auto& o : ogrenciler) {
        if (isim_iceriyor(o, aranan)) {
            std::cout << "    " << o.numara << " - " << o.isim
                      << " (" << o.bolum << ")\n";
        }
    }

    // Bolum filtresi de string_view ile
    std::string_view bolum_filtre = "Bilgisayar";
    std::cout << "\n  '" << bolum_filtre << "' bolumu ogrencileri:\n";
    for (const auto& o : ogrenciler) {
        if (o.bolum == bolum_filtre) {
            std::cout << "    " << o.numara << " - " << o.isim << "\n";
        }
    }

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === string_view Temelleri ===
 *
 * --- 1. Olusturma Yontemleri ---
 *   [Yazilim Gelistirme] (uzunluk: 18)
 *   [Merhaba Dunya] (uzunluk: 13)
 *   [C++20 Modern] (uzunluk: 12)
 *
 * --- 2. Temel Islemler ---
 *   substr(0,10): Bilgisayar
 *   'Bilimleri' konumu: 11
 *   'Bilgi' ile basliyor mu? Evet
 *   'Bolumu' ile bitiyor mu? Evet
 *
 * --- 3. remove_prefix / remove_suffix ---
 *   Orijinal: /home/ogrenci/proje/main.cpp
 *   remove_prefix(6): ogrenci/proje/main.cpp
 *   remove_suffix(4): /home/ogrenci/proje/main
 *
 * --- 4. Fonksiyon Parametresi ---
 *   string'den:  [Ali Yilmaz] (uzunluk: 10)
 *   C-string'den:  [Ayse Kaya] (uzunluk: 9)
 *   literal'den:  [Mehmet Demir] (uzunluk: 12)
 *
 * --- 5. Dangling Tehlikesi ---
 *   [UYARI] Asagidaki kod tehlikelidir:
 *   // std::string_view tehlikeli() {
 *   //     std::string gecici = "Gecici veri";
 *   //     return std::string_view{gecici}; // TEHLIKE!
 *   // } // gecici yok edilir, view sarkan referans olur
 *   string_view sahiplik almaz, kaynak yasadigi surece gecerlidir.
 *
 * --- 6. OBYS: Ogrenci Arama ---
 *   'Ali' iceren ogrenciler:
 *     1001 - Ali Yilmaz (Bilgisayar)
 *     1005 - Ali Arslan (Bilgisayar)
 *
 *   'Bilgisayar' bolumu ogrencileri:
 *     1001 - Ali Yilmaz
 *     1003 - Mehmet Demir
 *     1005 - Ali Arslan
 */

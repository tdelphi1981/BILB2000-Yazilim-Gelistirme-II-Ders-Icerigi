/**
 * Girdi Doğrulama - Allow-list Tabanlı Komut Validasyonu
 *
 * Bu program, sunucumuzun aldığı basit JSON-benzeri komutların
 * güvenlik açısından doğrulanmasını gösterir. Üç katmanlı kontrol:
 *  1) Komut allow-list'te mi?
 *  2) Zorunlu alanlar var mı, tipleri doğru mu?
 *  3) Boyut limitleri aşılmış mı? (DoS koruması)
 *
 * nlohmann/json gibi bir kütüphane KULLANMAZ; saf C++ ile manuel
 * stringstream tabanlı parse yapar. Bu, didaktik bir örnektir;
 * üretimde mutlaka olgun bir JSON kütüphanesi tercih edilir.
 *
 * OBYS bağlamında: Sunucu, "OGRENCI_LISTE" komutuna izin verirken
 * "VERITABANI_DROP" gibi tanımsız komutları sessizce reddeder.
 *
 * Öğrenilecek kavramlar:
 * - Allow-list (whitelist) güvenlik prensibi
 * - Boyut limitleri ile DoS savunması
 * - Saf C++ string ayrıştırma
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 4 - Güvenlik ve Tasarım
 *
 * Derleme: g++ -std=c++20 -o ornek 02_input_validation.cpp
 * Çalıştırma: ./ornek
 */

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

constexpr std::size_t MAKS_PAKET_BOYUTU = 1024;
constexpr std::size_t MAKS_ALAN_UZUNLUGU = 128;

const std::set<std::string> izinli_komutlar = {
    "LOGIN", "LOGOUT", "OGRENCI_LISTE", "OGRENCI_GETIR", "PING",
};

// Çok basit bir alan ayıklayıcı: "anahtar":"deger" veya "anahtar":sayi
std::map<std::string, std::string> json_parcala(const std::string& metin) {
    std::map<std::string, std::string> sonuc;
    std::size_t i = 0;
    while (i < metin.size()) {
        // Anahtar: tırnak içinde
        const auto t1 = metin.find('"', i);
        if (t1 == std::string::npos) break;
        const auto t2 = metin.find('"', t1 + 1);
        if (t2 == std::string::npos) break;
        const std::string anahtar = metin.substr(t1 + 1, t2 - t1 - 1);

        // ':' bul
        const auto kolon = metin.find(':', t2);
        if (kolon == std::string::npos) break;

        // Değer
        std::size_t deger_basi = kolon + 1;
        while (deger_basi < metin.size()
               && std::isspace(static_cast<unsigned char>(metin[deger_basi]))) {
            ++deger_basi;
        }
        if (deger_basi >= metin.size()) break;

        std::string deger;
        if (metin[deger_basi] == '"') {
            const auto son = metin.find('"', deger_basi + 1);
            if (son == std::string::npos) break;
            deger = metin.substr(deger_basi + 1, son - deger_basi - 1);
            i = son + 1;
        } else {
            std::size_t son = deger_basi;
            while (son < metin.size() && metin[son] != ',' && metin[son] != '}') {
                ++son;
            }
            deger = metin.substr(deger_basi, son - deger_basi);
            i = son;
        }
        sonuc[anahtar] = deger;
    }
    return sonuc;
}

struct DogrulamaSonuc {
    bool gecerli{false};
    std::string hata;
};

DogrulamaSonuc dogrula(const std::string& paket) {
    // 1) Boyut limiti
    if (paket.size() > MAKS_PAKET_BOYUTU) {
        return {false, "Paket çok büyük (DoS koruması)."};
    }
    if (paket.empty()) {
        return {false, "Boş paket."};
    }

    // 2) Parçala
    const auto alanlar = json_parcala(paket);
    auto it = alanlar.find("komut");
    if (it == alanlar.end()) {
        return {false, "'komut' alanı eksik."};
    }

    // 3) Allow-list
    if (!izinli_komutlar.contains(it->second)) {
        return {false, "İzin verilmeyen komut: " + it->second};
    }

    // 4) Alan başına uzunluk kontrolü
    for (const auto& [anahtar, deger] : alanlar) {
        if (anahtar.size() > MAKS_ALAN_UZUNLUGU
            || deger.size() > MAKS_ALAN_UZUNLUGU) {
            return {false, "Alan değeri çok uzun: " + anahtar};
        }
    }

    // 5) Komuta özel zorunlu alanlar
    if (it->second == "LOGIN") {
        if (!alanlar.contains("kullanici")
            || !alanlar.contains("parola_hash")) {
            return {false, "LOGIN için kullanici ve parola_hash gerekli."};
        }
    }
    if (it->second == "OGRENCI_GETIR") {
        if (!alanlar.contains("numara")) {
            return {false, "OGRENCI_GETIR için numara gerekli."};
        }
    }

    return {true, "Geçerli"};
}

int main() {
    const std::vector<std::string> testler = {
        R"({"komut":"PING"})",
        R"({"komut":"LOGIN","kullanici":"ogr1","parola_hash":"abc"})",
        R"({"komut":"OGRENCI_LISTE","sayfa":"1"})",
        R"({"komut":"OGRENCI_GETIR"})",
        R"({"komut":"VERITABANI_DROP"})",
        R"({"yanlis_alan":"deger"})",
        std::string(2000, 'x'),
    };

    int basarili = 0;
    for (const auto& test : testler) {
        const auto sonuc = dogrula(test);
        const std::string kisa = test.size() > 60
            ? test.substr(0, 60) + "..."
            : test;
        std::cout << (sonuc.gecerli ? "[KABUL] " : "[RED]   ")
                  << kisa << "  -> " << sonuc.hata << '\n';
        if (sonuc.gecerli) ++basarili;
    }
    std::cout << "\nToplam: " << testler.size()
              << ", Geçerli: " << basarili << '\n';
    return 0;
}

/*
BEKLENEN ÇIKTI:
---------------
[KABUL] {"komut":"PING"}  -> Geçerli
[KABUL] {"komut":"LOGIN",...}  -> Geçerli
[KABUL] {"komut":"OGRENCI_LISTE","sayfa":"1"}  -> Geçerli
[RED]   {"komut":"OGRENCI_GETIR"}  -> OGRENCI_GETIR için numara gerekli.
[RED]   {"komut":"VERITABANI_DROP"}  -> İzin verilmeyen komut: VERITABANI_DROP
[RED]   {"yanlis_alan":"deger"}  -> 'komut' alanı eksik.
[RED]   xxx...  -> Paket çok büyük (DoS koruması).
*/

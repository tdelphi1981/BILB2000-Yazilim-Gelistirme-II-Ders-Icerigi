/**
 * stack ile Geri Alma (Undo) Mekanizmasi
 *
 * Bu program, std::stack kapsayicisini kullanarak basit bir
 * geri alma (undo) mekanizmasi uygular. OBYS'de yapilan
 * islemleri geri almak icin stack kullanimi orneklenir.
 *
 * Ogrenilecek kavramlar:
 * - std::stack tanimlama ve kullanimi
 * - LIFO (Son Giren Ilk Cikar) mantigi
 * - push(), top(), pop(), empty(), size() metodlari
 * - Geri alma (undo) tasarim deseni
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 4 - Sirasiz Kapsayicilar ve Uyarlayicilar
 *
 * Derleme: g++ -std=c++20 -o 02_stack_undo 02_stack_undo.cpp
 * Calistirma: ./02_stack_undo
 */

#include <iostream>
#include <stack>
#include <string>
#include <vector>

// Islem turlerini temsil eden yapilar
struct Islem {
    std::string tur;        // "ekle", "sil", "guncelle"
    std::string aciklama;
    int ogrenci_no;
};

// Basit bir OBYS islem yoneticisi
class IslemYoneticisi {
    std::stack<Islem> geri_alma_yigini;
    std::vector<std::string> kayitlar;

public:
    void islem_yap(const Islem& islem) {
        // Islemi kaydet
        kayitlar.push_back(islem.aciklama);
        // Geri alma yiginina ekle
        geri_alma_yigini.push(islem);

        std::cout << "[ISLEM] " << islem.aciklama << '\n';
    }

    bool geri_al() {
        if (geri_alma_yigini.empty()) {
            std::cout << "[UYARI] Geri alinacak islem yok!" << '\n';
            return false;
        }

        const auto& son_islem = geri_alma_yigini.top();
        std::cout << "[GERI AL] " << son_islem.aciklama << '\n';

        // Kayitlardan son islemi kaldir
        if (!kayitlar.empty()) {
            kayitlar.pop_back();
        }

        geri_alma_yigini.pop();
        return true;
    }

    void durum_goster() const {
        std::cout << '\n' << "--- Mevcut Durum ---" << '\n';
        std::cout << "Yapilan islem sayisi: " << kayitlar.size() << '\n';
        std::cout << "Geri alinabilir islem: " << geri_alma_yigini.size() << '\n';

        if (!kayitlar.empty()) {
            std::cout << "Kayitlar:" << '\n';
            for (const auto& kayit : kayitlar) {
                std::cout << "  - " << kayit << '\n';
            }
        }
    }
};

int main() {
    std::cout << "=== OBYS Geri Alma Mekanizmasi ===" << '\n' << '\n';

    IslemYoneticisi yonetici;

    // Islemler yap
    yonetici.islem_yap({"ekle", "Ogrenci 1001 (Ali) eklendi", 1001});
    yonetici.islem_yap({"ekle", "Ogrenci 1002 (Ayse) eklendi", 1002});
    yonetici.islem_yap({"guncelle", "Ogrenci 1001 notu 85 olarak guncellendi", 1001});
    yonetici.islem_yap({"sil", "Ogrenci 1003 silindi", 1003});

    yonetici.durum_goster();

    // Geri alma islemleri
    std::cout << '\n' << "=== Geri Alma Basliyor ===" << '\n';
    yonetici.geri_al();  // Son islem geri alinir
    yonetici.geri_al();  // Bir onceki islem geri alinir

    yonetici.durum_goster();

    // Tum islemleri geri al
    std::cout << '\n' << "=== Kalan Islemler Geri Aliniyor ===" << '\n';
    while (yonetici.geri_al()) {
        // Tum islemler geri alinana kadar devam et
    }

    yonetici.durum_goster();

    // Bos yiginda geri alma denemesi
    std::cout << '\n';
    yonetici.geri_al();

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === OBYS Geri Alma Mekanizmasi ===
 *
 * [ISLEM] Ogrenci 1001 (Ali) eklendi
 * [ISLEM] Ogrenci 1002 (Ayse) eklendi
 * [ISLEM] Ogrenci 1001 notu 85 olarak guncellendi
 * [ISLEM] Ogrenci 1003 silindi
 *
 * --- Mevcut Durum ---
 * Yapilan islem sayisi: 4
 * Geri alinabilir islem: 4
 * Kayitlar:
 *   - Ogrenci 1001 (Ali) eklendi
 *   - Ogrenci 1002 (Ayse) eklendi
 *   - Ogrenci 1001 notu 85 olarak guncellendi
 *   - Ogrenci 1003 silindi
 *
 * === Geri Alma Basliyor ===
 * [GERI AL] Ogrenci 1003 silindi
 * [GERI AL] Ogrenci 1001 notu 85 olarak guncellendi
 *
 * --- Mevcut Durum ---
 * Yapilan islem sayisi: 2
 * Geri alinabilir islem: 2
 * Kayitlar:
 *   - Ogrenci 1001 (Ali) eklendi
 *   - Ogrenci 1002 (Ayse) eklendi
 *
 * === Kalan Islemler Geri Aliniyor ===
 * [GERI AL] Ogrenci 1002 (Ayse) eklendi
 * [GERI AL] Ogrenci 1001 (Ali) eklendi
 *
 * --- Mevcut Durum ---
 * Yapilan islem sayisi: 0
 * Geri alinabilir islem: 0
 *
 * [UYARI] Geri alinacak islem yok!
 */

// ============================================================
// Uygulama 3 - Modern OBYS (Ogrenci Bilgi Yonetim Sistemi)
// ============================================================
// Bu program, ogrenci bilgi sisteminin modern C++20 ile
// yazilmis halini gostermektedir.
//
// Kullanilan modern C++ ozellikleri:
//   - std::map tabanli Depo<K,V> sablonu
//   - std::optional ile guvenli arama (bul)
//   - std::variant ile esnek not tipleri (NotTuru)
//   - std::ranges ve std::views ile sorgulama
//   - std::string_view ile verimli string aktarimi
//   - std::format ile tip-guvenli formatlama
//   - Ozel istisna hiyerarsisi (OBYSHatasi ailesi)
//   - Duz struct yapilar (getter/setter yok)
//
// Bolum: 03 - Unite 5
// Derleme: C++20
// ============================================================

#include <iostream>
#include <format>
#include <filesystem>

#include "depo.h"
#include "varliklar/ogrenci.h"
#include "varliklar/ders.h"
#include "varliklar/ders_kaydi.h"
#include "hatalar/obys_hatasi.h"
#include "servisler/obys_servis.h"
#include "dosyalama.h"

// Dosya isimleri
const std::string DOSYA_OGRENCI = "ogrenciler.bin";
const std::string DOSYA_DERS    = "dersler.bin";
const std::string DOSYA_KAYIT   = "ders_kayitlari.bin";

int main()
{
    // --- Depo nesnelerini olustur ---
    Depo<int, Ogrenci> ogrenciDepo;
    Depo<std::string, Ders> dersDepo;
    Depo<int, DersKaydi> dersKaydiDepo;

    // --- Servis olustur ---
    OBYSServis servis(ogrenciDepo, dersDepo, dersKaydiDepo);

    // ========================================
    // Dosyadan yukle veya hardcoded veri gir
    // ========================================
    bool dosyadan_yuklendi = false;

    if (std::filesystem::exists(DOSYA_OGRENCI) &&
        std::filesystem::exists(DOSYA_DERS) &&
        std::filesystem::exists(DOSYA_KAYIT)) {

        std::cout << "[BILGI] Dosyalardan yükleniyor...\n";
        bool basarili = dosyadan_yukle(DOSYA_OGRENCI, ogrenciDepo)
                     && dosyadan_yukle(DOSYA_DERS, dersDepo)
                     && dosyadan_yukle(DOSYA_KAYIT, dersKaydiDepo);

        if (basarili) {
            dosyadan_yuklendi = true;
            std::cout << std::format(
                "[BILGI] Dosyalardan yüklendi: {} öğrenci, {} ders, {} kayıt\n\n",
                ogrenciDepo.boyut(), dersDepo.boyut(),
                dersKaydiDepo.boyut());
        } else {
            std::cerr << "[HATA] Dosya okuma hatası, veriler sıfırlandı.\n";
            ogrenciDepo.temizle();
            dersDepo.temizle();
            dersKaydiDepo.temizle();
        }
    }

    if (!dosyadan_yuklendi) {
        // ========================================
        // 10 ogrenci ekle
        // ========================================
        std::cout << "[BILGI] Öğrenciler ekleniyor...\n";
        servis.ogrenci_ekle(2021001, "Ahmet",   "Yılmaz",   "2002-03-15", "erkek");
        servis.ogrenci_ekle(2021002, "Ayşe",    "Demir",    "2001-07-22", "kadın");
        servis.ogrenci_ekle(2021003, "Mehmet",   "Kaya",     "2002-01-10", "erkek");
        servis.ogrenci_ekle(2021004, "Fatma",    "Çelik",    "2001-11-05", "kadın");
        servis.ogrenci_ekle(2021005, "Mustafa",  "Öztürk",   "2000-09-18", "erkek");
        servis.ogrenci_ekle(2021006, "Zeynep",   "Arslan",   "2002-05-30", "kadın");
        servis.ogrenci_ekle(2021007, "Emre",     "Şahin",    "2001-12-25", "erkek");
        servis.ogrenci_ekle(2021008, "Elif",     "Aydın",    "2002-08-14", "kadın");
        servis.ogrenci_ekle(2021009, "Burak",    "Yıldız",   "2000-04-02", "erkek");
        servis.ogrenci_ekle(2021010, "Selin",    "Koç",      "2001-06-19", "kadın");
        std::cout << std::format("[BILGI] {} öğrenci eklendi.\n\n",
            ogrenciDepo.boyut());

        // ========================================
        // 10 ders ekle
        // ========================================
        std::cout << "[BILGI] Dersler ekleniyor...\n";
        servis.ders_ekle("BIL101", "Programlamaya Giriş");
        servis.ders_ekle("BIL102", "Nesne Yönelimli Programlama");
        servis.ders_ekle("BIL201", "Veri Yapıları");
        servis.ders_ekle("BIL202", "Algoritmalar");
        servis.ders_ekle("BIL301", "Veritabanı Sistemleri");
        servis.ders_ekle("BIL302", "İşletim Sistemleri");
        servis.ders_ekle("BIL303", "Bilgisayar Ağları");
        servis.ders_ekle("BIL401", "Yazılım Mühendisliği");
        servis.ders_ekle("BIL402", "Yapay Zeka");
        servis.ders_ekle("BIL403", "Siber Güvenlik");
        std::cout << std::format("[BILGI] {} ders eklendi.\n\n",
            dersDepo.boyut());

        // ========================================
        // Notlar ekle - her ogrenciye 5 ders notu
        // ========================================
        std::cout << "[BILGI] Notlar giriliyor...\n";

        // Ogrenci 2021001 - Ahmet Yilmaz
        servis.not_ekle(2021001, "BIL101", 85, 90, 80);
        servis.not_ekle(2021001, "BIL102", 70, 65, 75);
        servis.not_ekle(2021001, "BIL201", 90, 85, 95);
        servis.not_ekle(2021001, "BIL202", 60, 55, 70);
        servis.not_ekle(2021001, "BIL301", 75, 80, 85);

        // Ogrenci 2021002 - Ayse Demir
        servis.not_ekle(2021002, "BIL101", 95, 88, 92);
        servis.not_ekle(2021002, "BIL102", 80, 75, 85);
        servis.not_ekle(2021002, "BIL201", 70, 65, 72);
        servis.not_ekle(2021002, "BIL303", 88, 90, 85);
        servis.not_ekle(2021002, "BIL401", 92, 95, 90);

        // Ogrenci 2021003 - Mehmet Kaya
        servis.not_ekle(2021003, "BIL101", 55, 60, 50);
        servis.not_ekle(2021003, "BIL102", 65, 70, 60);
        servis.not_ekle(2021003, "BIL202", 72, 68, 75);
        servis.not_ekle(2021003, "BIL301", 45, 50, 40);
        servis.not_ekle(2021003, "BIL302", 80, 85, 78);

        // Ogrenci 2021004 - Fatma Celik
        servis.not_ekle(2021004, "BIL101", 78, 82, 75);
        servis.not_ekle(2021004, "BIL201", 85, 90, 88);
        servis.not_ekle(2021004, "BIL202", 90, 92, 87);
        servis.not_ekle(2021004, "BIL302", 65, 70, 60);
        servis.not_ekle(2021004, "BIL403", 88, 85, 90);

        // Ogrenci 2021005 - Mustafa Ozturk
        servis.not_ekle(2021005, "BIL101", 40, 45, 35);
        servis.not_ekle(2021005, "BIL102", 55, 50, 60);
        servis.not_ekle(2021005, "BIL201", 62, 58, 65);
        servis.not_ekle(2021005, "BIL301", 70, 75, 68);
        servis.not_ekle(2021005, "BIL402", 50, 55, 48);

        // Ogrenci 2021006 - Zeynep Arslan
        servis.not_ekle(2021006, "BIL101", 92, 95, 90);
        servis.not_ekle(2021006, "BIL102", 88, 85, 90);
        servis.not_ekle(2021006, "BIL201", 95, 92, 98);
        servis.not_ekle(2021006, "BIL301", 85, 88, 82);
        servis.not_ekle(2021006, "BIL403", 90, 93, 88);

        // Ogrenci 2021007 - Emre Sahin
        servis.not_ekle(2021007, "BIL101", 68, 72, 65);
        servis.not_ekle(2021007, "BIL102", 75, 70, 78);
        servis.not_ekle(2021007, "BIL202", 58, 62, 55);
        servis.not_ekle(2021007, "BIL302", 82, 78, 85);
        servis.not_ekle(2021007, "BIL401", 70, 75, 68);

        // Ogrenci 2021008 - Elif Aydin
        servis.not_ekle(2021008, "BIL101", 88, 85, 92);
        servis.not_ekle(2021008, "BIL201", 76, 80, 72);
        servis.not_ekle(2021008, "BIL202", 82, 78, 85);
        servis.not_ekle(2021008, "BIL303", 90, 88, 92);
        servis.not_ekle(2021008, "BIL402", 85, 82, 88);

        // Ogrenci 2021009 - Burak Yildiz
        servis.not_ekle(2021009, "BIL101", 50, 48, 55);
        servis.not_ekle(2021009, "BIL102", 60, 58, 62);
        servis.not_ekle(2021009, "BIL201", 45, 50, 42);
        servis.not_ekle(2021009, "BIL301", 72, 68, 75);
        servis.not_ekle(2021009, "BIL403", 55, 60, 52);

        // Ogrenci 2021010 - Selin Koc
        servis.not_ekle(2021010, "BIL101", 82, 88, 85);
        servis.not_ekle(2021010, "BIL102", 90, 92, 88);
        servis.not_ekle(2021010, "BIL201", 78, 75, 80);
        servis.not_ekle(2021010, "BIL302", 85, 80, 82);
        servis.not_ekle(2021010, "BIL401", 92, 90, 95);

        std::cout << std::format("[BILGI] {} not kaydı girildi.\n\n",
            dersKaydiDepo.boyut());
    }

    // ========================================
    // Demo: Ders Listesi
    // ========================================
    std::cout << servis.ders_listesi() << "\n";

    // ========================================
    // Demo: Sinif Ortalamasi
    // ========================================
    try {
        float ort = servis.sinif_ortalamasi("BIL101");
        std::cout << std::format(
            "[BILGI] BIL101 - Programlamaya Giriş sınıf ortalaması: {:.2f}\n\n",
            ort);
    } catch (const OBYSHatasi& e) {
        std::cerr << std::format("[HATA] {}\n", e.what());
    }

    // ========================================
    // Demo: Ogrenci Transkript
    // ========================================
    try {
        std::cout << servis.ogrenci_transkript(2021001) << "\n";
    } catch (const OBYSHatasi& e) {
        std::cerr << std::format("[HATA] {}\n", e.what());
    }

    // ========================================
    // Demo: Basarili Ogrenciler
    // ========================================
    try {
        std::cout << servis.basarili_ogrenciler("BIL101") << "\n";
    } catch (const OBYSHatasi& e) {
        std::cerr << std::format("[HATA] {}\n", e.what());
    }

    // ========================================
    // Demo: Hata Yonetimi
    // ========================================
    std::cout << "--- Hata Yönetimi Demosu ---\n";

    // Ayni ogrenciyi tekrar eklemeye calis
    try {
        servis.ogrenci_ekle(2021001, "Test", "Test", "2000-01-01", "erkek");
    } catch (const OgrenciZatenVarHatasi& e) {
        std::cout << std::format("[BEKLENEN HATA] {}\n", e.what());
    }

    // Var olmayan ogrenci transkripti
    try {
        std::cout << servis.ogrenci_transkript(9999999);
    } catch (const OgrenciBulunamadiHatasi& e) {
        std::cout << std::format("[BEKLENEN HATA] {}\n", e.what());
    }

    // Var olmayan ders icin ortalama
    try {
        servis.sinif_ortalamasi("BIL999");
    } catch (const DersBulunamadiHatasi& e) {
        std::cout << std::format("[BEKLENEN HATA] {}\n", e.what());
    }

    // Var olmayan ogrenciye not ekleme
    try {
        servis.not_ekle(9999999, "BIL101", 50, 50, 50);
    } catch (const OgrenciBulunamadiHatasi& e) {
        std::cout << std::format("[BEKLENEN HATA] {}\n", e.what());
    }

    // ========================================
    // Verileri dosyaya kaydet
    // ========================================
    std::cout << "\n[BILGI] Veriler dosyaya kaydediliyor...\n";
    bool kayit_basarili =
        dosyaya_kaydet(DOSYA_OGRENCI, ogrenciDepo) &&
        dosyaya_kaydet(DOSYA_DERS, dersDepo) &&
        dosyaya_kaydet(DOSYA_KAYIT, dersKaydiDepo);

    if (kayit_basarili) {
        std::cout << std::format(
            "[BILGI] {} öğrenci, {} ders, {} kayıt dosyaya yazıldı.\n",
            ogrenciDepo.boyut(), dersDepo.boyut(),
            dersKaydiDepo.boyut());
    } else {
        std::cerr << "[HATA] Dosyaya kaydetme başarısız!\n";
    }

    std::cout << "\n[BILGI] Program başarıyla tamamlandı.\n";
    return 0;
}

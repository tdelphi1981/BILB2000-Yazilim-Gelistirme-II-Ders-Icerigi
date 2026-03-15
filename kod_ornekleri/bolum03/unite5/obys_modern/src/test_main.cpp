// ============================================================
// OBYS Modern - Test Dosyasi
// ============================================================
// Assert tabanli birim testleri.
// Depo, OBYSServis ve hata yonetimi testleri icerir.
//
// Bolum: 03 - Unite 5
// Derleme: C++20
// ============================================================

#include <iostream>
#include <cassert>
#include <string>
#include <format>
#include <optional>
#include <cstdio>
#include <cmath>

#include "depo.h"
#include "varliklar/ogrenci.h"
#include "varliklar/ders.h"
#include "varliklar/ders_kaydi.h"
#include "varliklar/not_turleri.h"
#include "hatalar/obys_hatasi.h"
#include "servisler/obys_servis.h"
#include "dosyalama.h"

// ------------------------------------------------
// Test: Depo ekle ve bul
// ------------------------------------------------
void test_depo_ekle_bul()
{
    Depo<int, std::string> depo;
    depo.ekle(1, "Ahmet");
    depo.ekle(2, "Ayşe");

    assert(depo.boyut() == 2);
    assert(depo.iceriyor_mu(1));
    assert(depo.iceriyor_mu(2));
    assert(!depo.iceriyor_mu(3));

    auto sonuc = depo.bul(1);
    assert(sonuc.has_value());
    assert(sonuc.value() == "Ahmet");

    auto yok = depo.bul(99);
    assert(!yok.has_value());

    // getir testi
    const auto& deger = depo.getir(2);
    assert(deger == "Ayşe");

    std::cout << "  [OK] test_depo_ekle_bul\n";
}

// ------------------------------------------------
// Test: Depo getir - olmayan eleman exception firlatmali
// ------------------------------------------------
void test_depo_getir_exception()
{
    Depo<int, std::string> depo;
    depo.ekle(1, "Test");

    bool firlatildi = false;
    try {
        [[maybe_unused]] const auto& d = depo.getir(999);
    } catch (const std::runtime_error&) {
        firlatildi = true;
    }
    assert(firlatildi);

    std::cout << "  [OK] test_depo_getir_exception\n";
}

// ------------------------------------------------
// Test: Depo sil
// ------------------------------------------------
void test_depo_sil()
{
    Depo<std::string, int> depo;
    depo.ekle("a", 10);
    depo.ekle("b", 20);

    assert(depo.boyut() == 2);

    bool silindi = depo.sil("a");
    assert(silindi);
    assert(depo.boyut() == 1);
    assert(!depo.iceriyor_mu("a"));
    assert(depo.iceriyor_mu("b"));

    // Olmayan elemani silmeye calis
    bool silinemedi = depo.sil("z");
    assert(!silinemedi);

    std::cout << "  [OK] test_depo_sil\n";
}

// ------------------------------------------------
// Test: OBYSServis ogrenci ekleme
// ------------------------------------------------
void test_ogrenci_ekle()
{
    Depo<int, Ogrenci> ogrDepo;
    Depo<std::string, Ders> dersDepo;
    Depo<int, DersKaydi> kayitDepo;
    OBYSServis servis(ogrDepo, dersDepo, kayitDepo);

    servis.ogrenci_ekle(1001, "Ali", "Veli", "2000-01-01", "erkek");
    assert(ogrDepo.boyut() == 1);

    auto ogr = ogrDepo.bul(1001);
    assert(ogr.has_value());
    assert(ogr.value().isim == "Ali");
    assert(ogr.value().soyisim == "Veli");
    assert(ogr.value().numara == 1001);

    std::cout << "  [OK] test_ogrenci_ekle\n";
}

// ------------------------------------------------
// Test: Not ekleme ve ortalama hesabi
// ------------------------------------------------
void test_not_ekle_ve_ortalama()
{
    Depo<int, Ogrenci> ogrDepo;
    Depo<std::string, Ders> dersDepo;
    Depo<int, DersKaydi> kayitDepo;
    OBYSServis servis(ogrDepo, dersDepo, kayitDepo);

    servis.ogrenci_ekle(5001, "Test", "Öğrenci", "2001-06-15", "kadın");
    servis.ders_ekle("TST100", "Test Dersi");
    servis.not_ekle(5001, "TST100", 80, 70, 90);

    assert(kayitDepo.boyut() == 1);

    // Ortalama: 80*0.3 + 70*0.4 + 90*0.3 = 24 + 28 + 27 = 79
    float sinif_ort = servis.sinif_ortalamasi("TST100");
    assert(sinif_ort > 78.9f && sinif_ort < 79.1f);

    std::cout << "  [OK] test_not_ekle_ve_ortalama\n";
}

// ------------------------------------------------
// Test: Hata yonetimi - cift ekleme ve bulunamama
// ------------------------------------------------
void test_hata_yonetimi()
{
    Depo<int, Ogrenci> ogrDepo;
    Depo<std::string, Ders> dersDepo;
    Depo<int, DersKaydi> kayitDepo;
    OBYSServis servis(ogrDepo, dersDepo, kayitDepo);

    servis.ogrenci_ekle(7001, "Deneme", "Kullanıcı", "1999-12-31", "erkek");

    // Ayni ogrenciyi tekrar ekleme -> OgrenciZatenVarHatasi
    bool ogr_zaten_var = false;
    try {
        servis.ogrenci_ekle(7001, "Tekrar", "Deneme", "2000-01-01", "kadın");
    } catch (const OgrenciZatenVarHatasi&) {
        ogr_zaten_var = true;
    }
    assert(ogr_zaten_var);

    // Var olmayan ogrenci transkripti -> OgrenciBulunamadiHatasi
    bool ogr_bulunamadi = false;
    try {
        servis.ogrenci_transkript(9999);
    } catch (const OgrenciBulunamadiHatasi&) {
        ogr_bulunamadi = true;
    }
    assert(ogr_bulunamadi);

    // Var olmayan ders ekleme -> DersBulunamadiHatasi
    bool ders_bulunamadi = false;
    try {
        servis.not_ekle(7001, "YOKDERS", 50, 50, 50);
    } catch (const DersBulunamadiHatasi&) {
        ders_bulunamadi = true;
    }
    assert(ders_bulunamadi);

    // Ayni dersi tekrar ekleme -> DersZatenVarHatasi
    servis.ders_ekle("TST200", "Deneme Dersi");
    bool ders_zaten_var = false;
    try {
        servis.ders_ekle("TST200", "Deneme Dersi Tekrar");
    } catch (const DersZatenVarHatasi&) {
        ders_zaten_var = true;
    }
    assert(ders_zaten_var);

    std::cout << "  [OK] test_hata_yonetimi\n";
}

// ------------------------------------------------
// Test: Variant not sistemi (Muaf tipi)
// ------------------------------------------------
void test_variant_not()
{
    // int not turu
    NotTuru n1 = 85;
    assert(DersKaydi::not_degeri(n1) == 85);

    // string not turu
    NotTuru n2 = std::string("72");
    assert(DersKaydi::not_degeri(n2) == 72);

    // Muaf not turu -> 0 donmeli
    NotTuru n3 = Muaf{"Askerlik nedeniyle muaf"};
    assert(DersKaydi::not_degeri(n3) == 0);

    // DersKaydi ile ortalama testi
    DersKaydi kayit{
        .ogrenci_no = 1,
        .ders_kodu = "TST",
        .vize = 100,
        .final_notu = 100,
        .odev = 100
    };
    // 100*0.3 + 100*0.4 + 100*0.3 = 100.0
    assert(kayit.ortalama() > 99.9f && kayit.ortalama() < 100.1f);

    // Muaf ile ortalama
    DersKaydi muaf_kayit{
        .ogrenci_no = 2,
        .ders_kodu = "TST",
        .vize = Muaf{},
        .final_notu = Muaf{},
        .odev = Muaf{}
    };
    assert(muaf_kayit.ortalama() < 0.1f);

    std::cout << "  [OK] test_variant_not\n";
}

// ------------------------------------------------
// Test: Dosyalama - binary round-trip
// ------------------------------------------------
void test_dosyalama()
{
    const std::string gecici = "_test_depo.bin";

    // --- Depo<int, Ogrenci> round-trip ---
    {
        Depo<int, Ogrenci> kaynak;
        kaynak.ekle(1001, {1001, "Ali", "Veli", "2000-01-01", "erkek"});
        kaynak.ekle(1002, {1002, "Ayşe", "Kara", "2001-05-10", "kadın"});

        assert(dosyaya_kaydet(gecici, kaynak));

        Depo<int, Ogrenci> hedef;
        assert(dosyadan_yukle(gecici, hedef));

        assert(hedef.boyut() == kaynak.boyut());
        assert(hedef.getir(1001).isim == "Ali");
        assert(hedef.getir(1002).soyisim == "Kara");
        std::remove(gecici.c_str());
    }

    // --- variant (NotTuru) round-trip ---
    {
        const std::string vdosya = "_test_variant.bin";

        NotTuru n_int = 85;
        NotTuru n_str = std::string("72");
        NotTuru n_muaf = Muaf{"Askerlik"};

        {
            std::ofstream dos(vdosya, std::ios::binary);
            dosyaya_yaz(dos, n_int);
            dosyaya_yaz(dos, n_str);
            dosyaya_yaz(dos, n_muaf);
        }

        NotTuru r_int, r_str, r_muaf;
        {
            std::ifstream dos(vdosya, std::ios::binary);
            dosyadan_oku(dos, r_int);
            dosyadan_oku(dos, r_str);
            dosyadan_oku(dos, r_muaf);
        }

        assert(r_int.index() == 0);
        assert(std::get<int>(r_int) == 85);
        assert(r_str.index() == 1);
        assert(std::get<std::string>(r_str) == "72");
        assert(r_muaf.index() == 2);
        assert(std::get<Muaf>(r_muaf).sebep == "Askerlik");

        std::remove(vdosya.c_str());
    }

    // --- DersKaydi round-trip: ortalama korunmali ---
    {
        const std::string kdosya = "_test_kayit.bin";

        Depo<int, DersKaydi> kaynak;
        DersKaydi dk{
            .ogrenci_no = 5001,
            .ders_kodu = "TST100",
            .vize = 80,
            .final_notu = 70,
            .odev = 90
        };
        kaynak.ekle(1, dk);

        assert(dosyaya_kaydet(kdosya, kaynak));

        Depo<int, DersKaydi> hedef;
        assert(dosyadan_yukle(kdosya, hedef));

        assert(hedef.boyut() == 1);
        float ort = hedef.getir(1).ortalama();
        // 80*0.3 + 70*0.4 + 90*0.3 = 79
        assert(std::fabs(ort - 79.0f) < 0.1f);

        std::remove(kdosya.c_str());
    }

    std::cout << "  [OK] test_dosyalama\n";
}

// ------------------------------------------------
// Ana fonksiyon - tum testleri calistir
// ------------------------------------------------
int main()
{
    std::cout << "=== OBYS Modern - Birim Testleri ===\n\n";

    test_depo_ekle_bul();
    test_depo_getir_exception();
    test_depo_sil();
    test_ogrenci_ekle();
    test_not_ekle_ve_ortalama();
    test_hata_yonetimi();
    test_variant_not();
    test_dosyalama();

    std::cout << "\n[OK] Tüm testler başarılı!\n";
    return 0;
}

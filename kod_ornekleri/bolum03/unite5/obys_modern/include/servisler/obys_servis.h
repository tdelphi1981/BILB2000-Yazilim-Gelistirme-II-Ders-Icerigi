#pragma once
// OBYSServis - is mantigi katmani
// Depo siniflari uzerinden ogrenci, ders ve not islemleri
// Bolum: 03 - Unite 5
// Derleme: C++20

#include <string>
#include <string_view>
#include "depo.h"
#include "varliklar/ogrenci.h"
#include "varliklar/ders.h"
#include "varliklar/ders_kaydi.h"
#include "hatalar/obys_hatasi.h"

class OBYSServis {
public:
    OBYSServis(Depo<int, Ogrenci>& ogrenciDepo,
               Depo<std::string, Ders>& dersDepo,
               Depo<int, DersKaydi>& dersKaydiDepo);

    // Ogrenci islemleri
    void ogrenci_ekle(int numara,
                      std::string_view isim,
                      std::string_view soyisim,
                      std::string_view dogum_tarihi,
                      std::string_view cinsiyet);

    // Ders islemleri
    void ders_ekle(std::string_view ders_kodu,
                   std::string_view ders_adi);

    // Not islemleri
    void not_ekle(int ogrenci_no,
                  std::string_view ders_kodu,
                  int vize,
                  int final_notu,
                  int odev);

    // Sorgular
    float sinif_ortalamasi(std::string_view ders_kodu) const;
    std::string ogrenci_transkript(int ogrenci_no) const;
    std::string ders_listesi() const;
    std::string basarili_ogrenciler(std::string_view ders_kodu,
                                     float esik = 60.0f) const;

private:
    Depo<int, Ogrenci>& _ogrenciDepo;
    Depo<std::string, Ders>& _dersDepo;
    Depo<int, DersKaydi>& _dersKaydiDepo;

    static int _kayit_sayaci;
};

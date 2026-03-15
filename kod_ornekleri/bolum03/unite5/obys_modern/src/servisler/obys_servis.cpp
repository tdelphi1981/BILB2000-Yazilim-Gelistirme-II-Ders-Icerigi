// OBYSServis - is mantigi uygulamasi
// std::format, ranges ve views kullanimina ornektir
// Bolum: 03 - Unite 5
// Derleme: C++20

#include "servisler/obys_servis.h"

#include <format>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <vector>
#include <sstream>

// Statik kayit sayacini baslat
int OBYSServis::_kayit_sayaci = 0;

OBYSServis::OBYSServis(Depo<int, Ogrenci>& ogrenciDepo,
                         Depo<std::string, Ders>& dersDepo,
                         Depo<int, DersKaydi>& dersKaydiDepo)
    : _ogrenciDepo(ogrenciDepo)
    , _dersDepo(dersDepo)
    , _dersKaydiDepo(dersKaydiDepo)
{}

void OBYSServis::ogrenci_ekle(int numara,
                                std::string_view isim,
                                std::string_view soyisim,
                                std::string_view dogum_tarihi,
                                std::string_view cinsiyet)
{
    // Zaten var mi kontrol et
    if (_ogrenciDepo.iceriyor_mu(numara)) {
        throw OgrenciZatenVarHatasi(
            std::format("{} numarali öğrenci zaten kayıtlı!", numara));
    }

    Ogrenci ogr{
        .numara = numara,
        .isim = std::string(isim),
        .soyisim = std::string(soyisim),
        .dogum_tarihi = std::string(dogum_tarihi),
        .cinsiyet = std::string(cinsiyet)
    };
    _ogrenciDepo.ekle(numara, ogr);
}

void OBYSServis::ders_ekle(std::string_view ders_kodu,
                             std::string_view ders_adi)
{
    std::string kod(ders_kodu);
    if (_dersDepo.iceriyor_mu(kod)) {
        throw DersZatenVarHatasi(
            std::format("{} kodlu ders zaten kayıtlı!", kod));
    }

    Ders d{
        .ders_kodu = kod,
        .ders_adi = std::string(ders_adi)
    };
    _dersDepo.ekle(kod, d);
}

void OBYSServis::not_ekle(int ogrenci_no,
                            std::string_view ders_kodu,
                            int vize,
                            int final_notu,
                            int odev)
{
    // Ogrenci var mi?
    if (!_ogrenciDepo.iceriyor_mu(ogrenci_no)) {
        throw OgrenciBulunamadiHatasi(
            std::format("{} numaralı öğrenci bulunamadı!", ogrenci_no));
    }

    // Ders var mi?
    std::string kod(ders_kodu);
    if (!_dersDepo.iceriyor_mu(kod)) {
        throw DersBulunamadiHatasi(
            std::format("{} kodlu ders bulunamadı!", kod));
    }

    DersKaydi kayit{
        .ogrenci_no = ogrenci_no,
        .ders_kodu = kod,
        .vize = vize,
        .final_notu = final_notu,
        .odev = odev
    };

    int kayit_id = ++_kayit_sayaci;
    _dersKaydiDepo.ekle(kayit_id, kayit);
}

float OBYSServis::sinif_ortalamasi(std::string_view ders_kodu) const
{
    std::string kod(ders_kodu);

    // Ders var mi kontrol et
    if (!_dersDepo.iceriyor_mu(kod)) {
        throw DersBulunamadiHatasi(
            std::format("{} kodlu ders bulunamadı!", kod));
    }

    const auto& kayitlar = _dersKaydiDepo.tumunu_al();

    // ranges::views ile filtreleme
    auto filtreli = kayitlar
        | std::views::values
        | std::views::filter([&kod](const DersKaydi& k) {
              return k.ders_kodu == kod;
          });

    // Ortalama hesapla
    float toplam = 0.0f;
    int sayac = 0;
    for (const auto& kayit : filtreli) {
        toplam += kayit.ortalama();
        ++sayac;
    }

    if (sayac == 0) {
        throw KayitHatasi(
            std::format("{} dersi için kayıt bulunamadı!", kod));
    }

    return toplam / static_cast<float>(sayac);
}

std::string OBYSServis::ogrenci_transkript(int ogrenci_no) const
{
    // Ogrenci var mi?
    auto ogr_opt = _ogrenciDepo.bul(ogrenci_no);
    if (!ogr_opt.has_value()) {
        throw OgrenciBulunamadiHatasi(
            std::format("{} numaralı öğrenci bulunamadı!", ogrenci_no));
    }

    const auto& ogr = ogr_opt.value();
    std::string sonuc = std::format(
        "=== Transkript ===\n"
        "Öğrenci: {} {} ({})\n"
        "Doğum Tarihi: {} | Cinsiyet: {}\n"
        "-----------------------------------\n",
        ogr.isim, ogr.soyisim, ogr.numara,
        ogr.dogum_tarihi, ogr.cinsiyet);

    const auto& kayitlar = _dersKaydiDepo.tumunu_al();

    // Bu ogrencinin kayitlarini filtrele
    auto ogr_kayitlari = kayitlar
        | std::views::values
        | std::views::filter([ogrenci_no](const DersKaydi& k) {
              return k.ogrenci_no == ogrenci_no;
          });

    float genel_toplam = 0.0f;
    int ders_sayisi = 0;

    for (const auto& kayit : ogr_kayitlari) {
        // Ders adini bul
        auto ders_opt = _dersDepo.bul(kayit.ders_kodu);
        std::string ders_adi = ders_opt.has_value()
            ? ders_opt.value().ders_adi
            : kayit.ders_kodu;

        float ort = kayit.ortalama();
        genel_toplam += ort;
        ++ders_sayisi;

        sonuc += std::format(
            "{:<8} {:<30} Ort: {:.1f}\n",
            kayit.ders_kodu, ders_adi, ort);
    }

    if (ders_sayisi > 0) {
        sonuc += std::format(
            "-----------------------------------\n"
            "Genel Ortalama: {:.2f}\n",
            genel_toplam / static_cast<float>(ders_sayisi));
    } else {
        sonuc += "Kayıtlı ders bulunamadı.\n";
    }

    sonuc += "===================================\n";
    return sonuc;
}

std::string OBYSServis::ders_listesi() const
{
    const auto& dersler = _dersDepo.tumunu_al();

    std::string sonuc = std::format(
        "=== Ders Listesi ({} ders) ===\n", dersler.size());

    // views::transform ile formatlama
    auto formatli = dersler
        | std::views::transform([](const auto& cift) {
              return std::format("  {} - {}",
                  cift.second.ders_kodu,
                  cift.second.ders_adi);
          });

    for (const auto& satir : formatli) {
        sonuc += satir + "\n";
    }

    sonuc += "==============================\n";
    return sonuc;
}

std::string OBYSServis::basarili_ogrenciler(std::string_view ders_kodu,
                                              float esik) const
{
    std::string kod(ders_kodu);

    // Ders var mi kontrol et
    if (!_dersDepo.iceriyor_mu(kod)) {
        throw DersBulunamadiHatasi(
            std::format("{} kodlu ders bulunamadı!", kod));
    }

    auto ders_opt = _dersDepo.bul(kod);
    std::string sonuc = std::format(
        "=== {} - Başarılı Öğrenciler (Eşik: {:.0f}) ===\n",
        ders_opt.value().ders_adi, esik);

    const auto& kayitlar = _dersKaydiDepo.tumunu_al();

    // filter + transform pipeline: once dersi filtrele, sonra basarililari sec
    auto basarililar = kayitlar
        | std::views::values
        | std::views::filter([&kod, esik](const DersKaydi& k) {
              return k.ders_kodu == kod && k.ortalama() >= esik;
          });

    int sayac = 0;
    for (const auto& kayit : basarililar) {
        auto ogr_opt = _ogrenciDepo.bul(kayit.ogrenci_no);
        if (ogr_opt.has_value()) {
            const auto& ogr = ogr_opt.value();
            sonuc += std::format("  {} {} {} - Ortalama: {:.1f}\n",
                ogr.numara, ogr.isim, ogr.soyisim, kayit.ortalama());
            ++sayac;
        }
    }

    if (sayac == 0) {
        sonuc += "  Başarılı öğrenci bulunamadı.\n";
    }

    sonuc += std::format("=== Toplam: {} öğrenci ===\n", sayac);
    return sonuc;
}

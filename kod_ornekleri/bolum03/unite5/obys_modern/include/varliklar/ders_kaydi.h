#pragma once
// Ders Kaydi varligi - variant tabanli not sistemi ile
// Bolum: 03 - Unite 5
// Derleme: C++20

#include <string>
#include <variant>
#include "varliklar/not_turleri.h"

struct DersKaydi {
    int ogrenci_no{};
    std::string ders_kodu;
    NotTuru vize;
    NotTuru final_notu;
    NotTuru odev;

    // Not degerini int olarak cikar, Muaf ise 0 dondur
    // string ise stoi ile donustur (basarisiz olursa 0)
    static int not_degeri(const NotTuru& n) {
        return std::visit([](const auto& val) -> int {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, int>) {
                return val;
            } else if constexpr (std::is_same_v<T, std::string>) {
                try { return std::stoi(val); }
                catch (...) { return 0; }
            } else {
                // Muaf durumu
                return 0;
            }
        }, n);
    }

    // Ortalama hesapla: vize %30 + final %40 + odev %30
    float ortalama() const {
        int v = not_degeri(vize);
        int f = not_degeri(final_notu);
        int o = not_degeri(odev);
        return static_cast<float>(v) * 0.30f
             + static_cast<float>(f) * 0.40f
             + static_cast<float>(o) * 0.30f;
    }
};

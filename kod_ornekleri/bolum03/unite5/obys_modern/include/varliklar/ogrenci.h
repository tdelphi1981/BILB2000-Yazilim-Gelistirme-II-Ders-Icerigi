#pragma once
// Ogrenci varligi - duz struct, kalitim ve getter/setter yok
// Bolum: 03 - Unite 5
// Derleme: C++20

#include <string>

struct Ogrenci {
    int numara{};
    std::string isim;
    std::string soyisim;
    std::string dogum_tarihi;
    std::string cinsiyet;
};

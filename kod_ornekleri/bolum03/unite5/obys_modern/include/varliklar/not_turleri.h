#pragma once
// Not Turleri - variant ile esnek not sistemi
// Bolum: 03 - Unite 5
// Derleme: C++20

#include <variant>
#include <string>

// Muaf durumu icin ozel struct
struct Muaf {
    std::string sebep{"Muaf"};
};

// NotTuru: bir not degeri ya sayi, ya harf, ya da Muaf olabilir
using NotTuru = std::variant<int, std::string, Muaf>;

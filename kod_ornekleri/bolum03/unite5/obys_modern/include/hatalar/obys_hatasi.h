#pragma once
// OBYS Hata Hiyerarsisi - ozel istisna siniflari
// Bolum: 03 - Unite 5
// Derleme: C++20

#include <stdexcept>
#include <string>

// --- Temel hata sinifi ---
class OBYSHatasi : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// --- Ogrenci hatalari ---
class OgrenciHatasi : public OBYSHatasi {
public:
    using OBYSHatasi::OBYSHatasi;
};

class OgrenciBulunamadiHatasi : public OgrenciHatasi {
public:
    using OgrenciHatasi::OgrenciHatasi;
};

class OgrenciZatenVarHatasi : public OgrenciHatasi {
public:
    using OgrenciHatasi::OgrenciHatasi;
};

// --- Ders hatalari ---
class DersHatasi : public OBYSHatasi {
public:
    using OBYSHatasi::OBYSHatasi;
};

class DersBulunamadiHatasi : public DersHatasi {
public:
    using DersHatasi::DersHatasi;
};

class DersZatenVarHatasi : public DersHatasi {
public:
    using DersHatasi::DersHatasi;
};

// --- Kayit hatalari ---
class KayitHatasi : public OBYSHatasi {
public:
    using OBYSHatasi::OBYSHatasi;
};

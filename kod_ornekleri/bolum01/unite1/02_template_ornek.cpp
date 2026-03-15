/**
 * Template Ornegi - Jenerik Programlama
 *
 * Bu program C++ template mekanizmasini tanitir.
 * Ayni fonksiyonun farkli turlerle nasil calistigini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - Fonksiyon sablonu (function template)
 * - Sinif sablonu (class template)
 * - Template somutlastirma (instantiation)
 * - Tur cikarsama (type deduction)
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 1 - STL'e Genel Bakis
 *
 * Derleme: g++ -std=c++20 -o 02_template_ornek 02_template_ornek.cpp
 * Calistirma: ./02_template_ornek
 */

#include <iostream>
#include <string>
#include <vector>

// --- FONKSIYON SABLONU ---
// Herhangi bir tur icin calisan maksimum fonksiyonu
template <typename T>
T maksimum(T a, T b) {
    return (a > b) ? a : b;
}

// --- SINIF SABLONU ---
// Basit bir jenerik cift (pair) yapisi
template <typename T1, typename T2>
struct Cift {
    T1 birinci;
    T2 ikinci;

    void yazdir() const {
        std::cout << "(" << birinci << ", " << ikinci << ")";
    }
};

// --- JENERIK DEPO (OBYS benzeri) ---
// Gecen donem yazdiginiz Depo<T>'nin basitlestirilmis hali
template <typename T>
class MiniDepo {
    std::vector<T> elemanlar_;

public:
    void ekle(const T& eleman) {
        elemanlar_.push_back(eleman);
    }

    size_t boyut() const {
        return elemanlar_.size();
    }

    void listele() const {
        for (size_t i = 0; i < elemanlar_.size(); ++i) {
            std::cout << "  [" << i << "] " << elemanlar_[i] << "\n";
        }
    }
};

int main() {
    std::cout << "=== Template Ornekleri ===\n\n";

    // --- Fonksiyon sablonu ornekleri ---
    std::cout << "--- Fonksiyon Sablonu ---\n";

    // Derleyici T=int olarak cikarir
    std::cout << "maksimum(3, 7) = " << maksimum(3, 7) << "\n";

    // Derleyici T=double olarak cikarir
    std::cout << "maksimum(3.14, 2.71) = " << maksimum(3.14, 2.71) << "\n";

    // Derleyici T=string olarak cikarir
    std::cout << "maksimum(\"Ali\", \"Veli\") = "
              << maksimum(std::string("Ali"), std::string("Veli")) << "\n\n";

    // --- Sinif sablonu ornekleri ---
    std::cout << "--- Sinif Sablonu ---\n";

    Cift<int, std::string> ogrenci{1001, "Ali Yilmaz"};
    std::cout << "Ogrenci: ";
    ogrenci.yazdir();
    std::cout << "\n";

    Cift<std::string, double> ders{"Matematik", 3.5};
    std::cout << "Ders:    ";
    ders.yazdir();
    std::cout << "\n\n";

    // --- MiniDepo ornegi ---
    std::cout << "--- MiniDepo<T> (Jenerik Depo) ---\n";

    // int deposu
    MiniDepo<int> not_deposu;
    not_deposu.ekle(85);
    not_deposu.ekle(92);
    not_deposu.ekle(78);
    std::cout << "Not deposu (" << not_deposu.boyut() << " eleman):\n";
    not_deposu.listele();

    std::cout << "\n";

    // string deposu - ayni sinif, farkli tur
    MiniDepo<std::string> isim_deposu;
    isim_deposu.ekle("Zeynep");
    isim_deposu.ekle("Mehmet");
    isim_deposu.ekle("Ayse");
    std::cout << "Isim deposu (" << isim_deposu.boyut() << " eleman):\n";
    isim_deposu.listele();

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ---------------
 * === Template Ornekleri ===
 *
 * --- Fonksiyon Sablonu ---
 * maksimum(3, 7) = 7
 * maksimum(3.14, 2.71) = 3.14
 * maksimum("Ali", "Veli") = Veli
 *
 * --- Sinif Sablonu ---
 * Ogrenci: (1001, Ali Yilmaz)
 * Ders:    (Matematik, 3.5)
 *
 * --- MiniDepo<T> (Jenerik Depo) ---
 * Not deposu (3 eleman):
 *   [0] 85
 *   [1] 92
 *   [2] 78
 *
 * Isim deposu (3 eleman):
 *   [0] Zeynep
 *   [1] Mehmet
 *   [2] Ayse
 */

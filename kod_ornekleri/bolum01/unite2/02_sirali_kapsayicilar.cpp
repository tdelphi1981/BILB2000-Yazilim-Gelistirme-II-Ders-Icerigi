/**
 * Sirali Kapsayicilar - deque, list, forward_list, array
 *
 * Bu program dort farkli sirali kapsayicinin temel
 * kullanimi ve ozelliklerini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - deque: cift uclu kuyruk, push_front/push_back
 * - list: cift yonlu bagli liste, splice, remove
 * - forward_list: tek yonlu bagli liste, insert_after
 * - array: sabit boyutlu dizi, STL arayuzu
 *
 * Bolum: 01 - STL Kapsayicilara Giris
 * Unite: 2 - Sirali Kapsayicilar
 *
 * Derleme: g++ -std=c++20 -o 02_sirali_kapsayicilar 02_sirali_kapsayicilar.cpp
 * Calistirma: ./02_sirali_kapsayicilar
 */

#include <iostream>
#include <deque>
#include <list>
#include <forward_list>
#include <array>
#include <algorithm>
#include <string>
#include <iterator>

// Herhangi bir kapsayiciyi yazdir
template<typename Container>
void yazdir(const std::string& baslik, const Container& c) {
    std::cout << baslik << ": ";
    for (const auto& e : c) {
        std::cout << e << ' ';
    }
    std::cout << '\n';
}

void deque_ornegi() {
    std::cout << "=== deque Ornegi ===" << '\n';

    std::deque<std::string> kuyruk;

    // Hem basa hem sona ekleme
    kuyruk.push_back("Ali");
    kuyruk.push_front("Zeynep");
    kuyruk.push_back("Mehmet");
    kuyruk.push_front("Ayse");

    yazdir("Kuyruk", kuyruk);  // Ayse Zeynep Ali Mehmet

    // Rastgele erisim
    std::cout << "kuyruk[1] = " << kuyruk[1] << '\n';

    // Bastan ve sondan silme
    kuyruk.pop_front();  // Ayse gider
    kuyruk.pop_back();   // Mehmet gider
    yazdir("Silme sonrasi", kuyruk);  // Zeynep Ali

    std::cout << '\n';
}

void list_ornegi() {
    std::cout << "=== list Ornegi ===" << '\n';

    std::list<int> sayilar = {10, 30, 50, 70};
    yazdir("Baslangic", sayilar);

    // Ortaya ekleme: 30'dan once 20 ekle
    auto it = std::find(sayilar.begin(), sayilar.end(), 30);
    sayilar.insert(it, 20);
    yazdir("20 eklendi", sayilar);  // 10 20 30 50 70

    // Belirli degeri sil
    sayilar.remove(50);
    yazdir("50 silindi", sayilar);  // 10 20 30 70

    // splice: iki listeyi birlestir
    std::list<int> diger = {100, 200};
    sayilar.splice(sayilar.end(), diger);
    yazdir("splice sonrasi", sayilar);  // 10 20 30 70 100 200
    std::cout << "diger.size() = " << diger.size() << " (bos)" << '\n';

    // Siralama (list'in kendi sort'u)
    sayilar.sort();
    yazdir("Siralanmis", sayilar);

    std::cout << '\n';
}

void forward_list_ornegi() {
    std::cout << "=== forward_list Ornegi ===" << '\n';

    std::forward_list<int> fl = {3, 1, 4, 1, 5, 9};
    yazdir("Baslangic", fl);

    // Basa ekleme (push_back yok!)
    fl.push_front(0);
    yazdir("push_front(0)", fl);

    // Belirli konumdan sonra ekle
    auto onceki = fl.before_begin();
    for (auto it = fl.begin(); it != fl.end(); ++it) {
        if (*it == 4) {
            fl.insert_after(it, 42);
            break;
        }
        onceki = it;
    }
    yazdir("4'ten sonra 42", fl);

    // Kosullu silme
    fl.remove_if([](int x) { return x < 3; });
    yazdir("x < 3 silindi", fl);

    // Siralama
    fl.sort();
    yazdir("Siralanmis", fl);

    // Boyut: size() fonksiyonu yok!
    auto boyut = std::distance(fl.begin(), fl.end());
    std::cout << "Eleman sayisi: " << boyut << '\n';

    std::cout << '\n';
}

void array_ornegi() {
    std::cout << "=== array Ornegi ===" << '\n';

    // Derleme zamaninda boyut belirlenir
    std::array<int, 7> haftalik_notlar = {85, 92, 78, 95, 88, 73, 91};
    yazdir("Notlar", haftalik_notlar);

    // STL algoritmalari dogrudan kullanilabilir
    std::sort(haftalik_notlar.begin(), haftalik_notlar.end());
    yazdir("Siralanmis", haftalik_notlar);

    // Guvenli erisim
    std::cout << "En dusuk not: " << haftalik_notlar.front() << '\n';
    std::cout << "En yuksek not: " << haftalik_notlar.back() << '\n';
    std::cout << "3. siradaki: " << haftalik_notlar.at(2) << '\n';

    // Boyut bilgisi
    std::cout << "Boyut: " << haftalik_notlar.size() << '\n';

    // fill ile tum elemanlari ayni degere ata
    std::array<int, 5> sifirlar;
    sifirlar.fill(0);
    yazdir("Sifirlanmis", sifirlar);

    std::cout << '\n';
}

int main() {
    deque_ornegi();
    list_ornegi();
    forward_list_ornegi();
    array_ornegi();

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== deque Ornegi ===
Kuyruk: Ayse Zeynep Ali Mehmet
kuyruk[1] = Zeynep
Silme sonrasi: Zeynep Ali

=== list Ornegi ===
Baslangic: 10 30 50 70
20 eklendi: 10 20 30 50 70
50 silindi: 10 20 30 70
splice sonrasi: 10 20 30 70 100 200
diger.size() = 0 (bos)
Siralanmis: 10 20 30 70 100 200

=== forward_list Ornegi ===
Baslangic: 3 1 4 1 5 9
push_front(0): 0 3 1 4 1 5 9
4'ten sonra 42: 0 3 1 4 42 1 5 9
x < 3 silindi: 3 4 42 5 9
Siralanmis: 3 4 5 9 42
Eleman sayisi: 5

=== array Ornegi ===
Notlar: 85 92 78 95 88 73 91
Siralanmis: 73 78 85 88 91 92 95
En dusuk not: 73
En yuksek not: 95
3. siradaki: 85
Boyut: 7
Sifirlanmis: 0 0 0 0 0
*/

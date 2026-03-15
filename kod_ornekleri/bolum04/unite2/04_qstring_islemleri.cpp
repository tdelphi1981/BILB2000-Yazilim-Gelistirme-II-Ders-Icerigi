/**
 * QString Temel Islemleri - OBYS Baglaminda
 *
 * Bu program, Qt'nin QString sinifinin temel ozelliklerini gosterir:
 * olusturma, birlestirme, arg() ile formatlama, split(), contains(),
 * toInt(), trimmed() ve std::string donusumu.
 *
 * Ogrenilecek kavramlar:
 * - QString olusturma ve birlestirme
 * - arg() ile formatli string olusturma
 * - split(), contains(), trimmed() ile metin isleme
 * - QStringList: join(), filter(), sort()
 * - std::string ile donusum
 *
 * Bolum: 04 - Qt Framework'une Giris
 * Unite: 2 - QObject ve Meta-Object Sistemi
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Core) \
 *       -o 04_qstring_islemleri 04_qstring_islemleri.cpp
 */

#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QDebug>

// -- OBYS: Ogrenci isim dogrulama --
bool isimGecerliMi(const QString &isim) {
    // Bosluk temizle
    QString temiz = isim.trimmed();

    // Bos kontrolu
    if (temiz.isEmpty()) {
        qDebug() << "[HATA] Isim bos olamaz!";
        return false;
    }

    // En az iki kelime (ad ve soyad) kontrolu
    QStringList parcalar = temiz.split(" ", Qt::SkipEmptyParts);
    if (parcalar.size() < 2) {
        qDebug() << "[HATA] Ad ve soyad girilmeli!";
        return false;
    }

    // Her kelimenin sadece harf icerdigini kontrol et
    for (const QString &parca : parcalar) {
        for (int i = 0; i < parca.size(); ++i) {
            if (!parca.at(i).isLetter()) {
                qDebug() << "[HATA] Isim sadece harf icermeli:"
                         << parca;
                return false;
            }
        }
    }

    return true;
}

// -- OBYS: Ogrenci numara formatlama --
QString numaraFormatla(int yil, int bolumKodu, int siraNo) {
    // Format: YIL-BOLUM-SIRA (ornek: 2024-0110-0042)
    return QString("%1-%2-%3")
        .arg(yil)
        .arg(bolumKodu, 4, 10, QChar('0'))   // 4 hane, 0 ile doldur
        .arg(siraNo, 4, 10, QChar('0'));      // 4 hane, 0 ile doldur
}

// -- OBYS: Ogrenci bilgi karti olusturma --
QString bilgiKartiOlustur(const QString &isim, int numara,
                           double ortalama) {
    QString kart = QString("=== Ogrenci Bilgi Karti ===\n"
                           "Isim    : %1\n"
                           "Numara  : %2\n"
                           "Ortalama: %3\n"
                           "Durum   : %4")
        .arg(isim)
        .arg(numara)
        .arg(ortalama, 0, 'f', 2)
        .arg(ortalama >= 2.0 ? "Basarili" : "Basarisiz");
    return kart;
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "=== QString Temel Islemleri ===\n";

    // --- 1. Olusturma ve birlestirme ---
    qDebug() << "--- 1. Olusturma ve Birlestirme ---";
    QString isim = "Ali Yilmaz";
    QString bos;
    QString bosLiteral = "";

    qDebug() << "isim:" << isim;
    qDebug() << "bos.isEmpty():" << bos.isEmpty();       // true
    qDebug() << "bos.isNull():" << bos.isNull();         // true
    qDebug() << "bosLiteral.isNull():" << bosLiteral.isNull(); // false

    // Birlestirme
    QString tam = isim + " - Ogrenci";
    qDebug() << "Birlestirme:" << tam;

    // --- 2. arg() ile formatlama ---
    qDebug() << "\n--- 2. arg() ile Formatlama ---";
    QString bilgi = QString("Ogrenci: %1, Numara: %2, Not: %3")
                        .arg("Ayse Kaya")
                        .arg(1002)
                        .arg(85.5, 0, 'f', 1);
    qDebug().noquote() << bilgi;

    // Sayidan QString
    QString sayi = QString::number(3.14159, 'f', 2);
    qDebug() << "number():" << sayi;

    // --- 3. Arama ve degistirme ---
    qDebug() << "\n--- 3. Arama ve Degistirme ---";
    QString cumle = "Qt ile GUI programlama dersi";

    qDebug() << "contains('GUI'):" << cumle.contains("GUI");
    qDebug() << "indexOf('GUI'):" << cumle.indexOf("GUI");
    qDebug() << "startsWith('Qt'):" << cumle.startsWith("Qt");
    qDebug() << "endsWith('dersi'):" << cumle.endsWith("dersi");

    // Degistirme (yerinde degistirir)
    QString degismis = cumle;
    degismis.replace("GUI", "grafiksel arayuz");
    qDebug().noquote() << "replace():" << degismis;

    // --- 4. split() ve QStringList ---
    qDebug() << "\n--- 4. split() ve QStringList ---";
    QString csv = "Algoritma,Veritabani,Yazilim Gelistirme,Ag Programlama";
    QStringList dersler = csv.split(",");

    qDebug() << "Ders sayisi:" << dersler.size();
    qDebug() << "Dersler:" << dersler;

    // join() ile birlestirme
    QString birlesik = dersler.join(" | ");
    qDebug().noquote() << "join():" << birlesik;

    // filter() ile filtreleme
    QStringList filtrelenmis = dersler.filter("Yaz");
    qDebug() << "filter('Yaz'):" << filtrelenmis;

    // sort()
    dersler.sort();
    qDebug() << "sort() sonrasi:" << dersler;

    // --- 5. Donusum islemleri ---
    qDebug() << "\n--- 5. Donusum Islemleri ---";
    QString sayiStr = "42";
    int tamsayi = sayiStr.toInt();
    qDebug() << "toInt():" << tamsayi;

    QString ondalikStr = "3.14";
    double ondalik = ondalikStr.toDouble();
    qDebug() << "toDouble():" << ondalik;

    // Buyuk/kucuk harf
    QString ornek = "Qt Framework";
    qDebug() << "toUpper():" << ornek.toUpper();
    qDebug() << "toLower():" << ornek.toLower();

    // trimmed() ve simplified()
    QString bosluklu = "  Yazilim   Gelistirme   II  ";
    qDebug().noquote() << "trimmed(): '" << bosluklu.trimmed() << "'";
    qDebug().noquote() << "simplified(): '" << bosluklu.simplified() << "'";

    // --- 6. std::string donusumu ---
    qDebug() << "\n--- 6. std::string Donusumu ---";
    QString qtStr = "Merhaba Qt";
    std::string stdStr = qtStr.toStdString();
    qDebug() << "toStdString():" << stdStr.c_str();

    std::string stlStr = "Merhaba STL";
    QString geriDonusmus = QString::fromStdString(stlStr);
    qDebug() << "fromStdString():" << geriDonusmus;

    // --- 7. OBYS: Isim dogrulama ---
    qDebug() << "\n--- 7. OBYS: Isim Dogrulama ---";
    qDebug() << "isimGecerliMi('Ali Yilmaz'):"
             << isimGecerliMi("Ali Yilmaz");
    qDebug() << "isimGecerliMi('Ali'):"
             << isimGecerliMi("Ali");
    qDebug() << "isimGecerliMi('Ali 123'):"
             << isimGecerliMi("Ali 123");
    qDebug() << "isimGecerliMi(''):"
             << isimGecerliMi("");

    // --- 8. OBYS: Numara formatlama ---
    qDebug() << "\n--- 8. OBYS: Numara Formatlama ---";
    QString numara = numaraFormatla(2024, 110, 42);
    qDebug().noquote() << "Numara:" << numara;

    // --- 9. OBYS: Bilgi karti ---
    qDebug() << "\n--- 9. OBYS: Bilgi Karti ---";
    QString kart = bilgiKartiOlustur("Ali Yilmaz", 20240042, 2.85);
    qDebug().noquote() << kart;

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== QString Temel Islemleri ===

--- 1. Olusturma ve Birlestirme ---
isim: "Ali Yilmaz"
bos.isEmpty(): true
bos.isNull(): true
bosLiteral.isNull(): false
Birlestirme: "Ali Yilmaz - Ogrenci"

--- 2. arg() ile Formatlama ---
Ogrenci: Ayse Kaya, Numara: 1002, Not: 85.5
number(): "3.14"

--- 3. Arama ve Degistirme ---
contains('GUI'): true
indexOf('GUI'): 7
startsWith('Qt'): true
endsWith('dersi'): true
replace(): Qt ile grafiksel arayuz programlama dersi

--- 4. split() ve QStringList ---
Ders sayisi: 4
Dersler: ("Algoritma", "Veritabani", "Yazilim Gelistirme", "Ag Programlama")
join(): Algoritma | Veritabani | Yazilim Gelistirme | Ag Programlama
filter('Yaz'): ("Yazilim Gelistirme")
sort() sonrasi: ("Ag Programlama", "Algoritma", "Veritabani", "Yazilim Gelistirme")

--- 5. Donusum Islemleri ---
toInt(): 42
toDouble(): 3.14
toUpper(): "QT FRAMEWORK"
toLower(): "qt framework"
trimmed(): 'Yazilim   Gelistirme   II'
simplified(): 'Yazilim Gelistirme II'

--- 6. std::string Donusumu ---
toStdString(): Merhaba Qt
fromStdString(): "Merhaba STL"

--- 7. OBYS: Isim Dogrulama ---
isimGecerliMi('Ali Yilmaz'): true
[HATA] Ad ve soyad girilmeli!
isimGecerliMi('Ali'): false
[HATA] Isim sadece harf icermeli: "123"
isimGecerliMi('Ali 123'): false
[HATA] Isim bos olamaz!
isimGecerliMi(''): false

--- 8. OBYS: Numara Formatlama ---
Numara: 2024-0110-0042

--- 9. OBYS: Bilgi Karti ---
=== Ogrenci Bilgi Karti ===
Isim    : Ali Yilmaz
Numara  : 20240042
Ortalama: 2.85
Durum   : Basarili
*/

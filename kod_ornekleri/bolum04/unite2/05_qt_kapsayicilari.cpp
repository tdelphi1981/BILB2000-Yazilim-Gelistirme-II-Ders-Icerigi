/**
 * Qt Kapsayicilari - QList, QMap, QSet
 *
 * Bu program, Qt'nin temel kapsayici siniflarini gosterir:
 * QList, QMap, QSet ve bunlarin STL karsiliklariyla
 * karsilastirmali kullanimi. OBYS baglami icinde
 * pratik ornekler sunar.
 *
 * Ogrenilecek kavramlar:
 * - QList temel kullanim (append, at, contains, removeOne)
 * - QMap kullanim (insert, value, keys, contains)
 * - QSet kullanim (insert, contains, intersect)
 * - Implicit sharing davranisi
 * - STL ile karsilastirmali ornekler
 * - OBYS: QMap ile ogrenci sozlugu
 *
 * Bolum: 04 - Qt Framework'une Giris
 * Unite: 2 - QObject ve Meta-Object Sistemi
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Core) \
 *       -o 05_qt_kapsayicilari 05_qt_kapsayicilari.cpp
 */

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QSet>
#include <QString>
#include <QDebug>
#include <numeric>  // std::accumulate

// -- OBYS: Ogrenci sozlugu (QMap tabanli) --
class OgrenciSozlugu {
public:
    // Ogrenci ekle (numara -> isim)
    void ekle(int numara, const QString &isim) {
        m_ogrenciler.insert(numara, isim);
        qDebug() << "  Eklendi:" << numara << "->" << isim;
    }

    // Numaraya gore ara
    QString ara(int numara) const {
        return m_ogrenciler.value(numara, "Bulunamadi");
    }

    // Isme gore ara (ters arama)
    int numaraBul(const QString &isim) const {
        // QMap::key() ters arama yapar
        return m_ogrenciler.key(isim, -1);
    }

    // Tum ogrencileri listele
    void listele() const {
        qDebug() << "  --- Ogrenci Listesi ---";
        // QMap::const_iterator ile gezinme (anahtara gore sirali)
        for (auto it = m_ogrenciler.constBegin();
             it != m_ogrenciler.constEnd(); ++it) {
            qDebug().noquote()
                << QString("  %1: %2").arg(it.key()).arg(it.value());
        }
        qDebug() << "  Toplam:" << m_ogrenciler.size() << "ogrenci";
    }

    // Ogrenci sayisi
    int boyut() const { return m_ogrenciler.size(); }

    // Tum numaralari don
    QList<int> numaralar() const { return m_ogrenciler.keys(); }

private:
    QMap<int, QString> m_ogrenciler;
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "=== Qt Kapsayicilari ===\n";

    // --- 1. QList Temel Kullanim ---
    qDebug() << "--- 1. QList Temel Kullanim ---";
    QList<int> notlar;
    notlar.append(85);
    notlar.append(92);
    notlar.append(78);
    notlar << 95 << 88;  // << operatoru ile ekleme

    qDebug() << "Notlar:" << notlar;
    qDebug() << "Boyut:" << notlar.size();
    qDebug() << "Ilk:" << notlar.first() << "Son:" << notlar.last();
    qDebug() << "at(2):" << notlar.at(2);
    qDebug() << "contains(92):" << notlar.contains(92);

    // Silme
    notlar.removeOne(78);
    qDebug() << "removeOne(78) sonrasi:" << notlar;

    // STL algoritmasi QList uzerinde
    double toplam = std::accumulate(notlar.begin(), notlar.end(), 0.0);
    double ortalama = toplam / notlar.size();
    qDebug() << "Ortalama (std::accumulate):" << ortalama;

    // --- 2. QList vs std::vector ---
    qDebug() << "\n--- 2. QList vs std::vector ---";

    // std::vector ile
    std::vector<int> stdNotlar = {85, 92, 78, 95, 88};
    qDebug() << "std::vector boyut:" << stdNotlar.size();

    // QList'e donusum
    QList<int> qtNotlar(stdNotlar.begin(), stdNotlar.end());
    qDebug() << "QList'e donusum:" << qtNotlar;

    // QList'ten std::vector'e
    std::vector<int> geriDonus(qtNotlar.begin(), qtNotlar.end());
    qDebug() << "std::vector'e geri:" << geriDonus.size() << "eleman";

    // --- 3. QMap Temel Kullanim ---
    qDebug() << "\n--- 3. QMap Temel Kullanim ---";
    QMap<QString, int> dersNotlari;
    dersNotlari.insert("Algoritma", 85);
    dersNotlari.insert("Veritabani", 92);
    dersNotlari["Yazilim Gelistirme"] = 78;  // [] ile ekleme

    qDebug() << "Ders notlari:" << dersNotlari;
    qDebug() << "Algoritma notu:" << dersNotlari.value("Algoritma");
    qDebug() << "Olmayan ders:"
             << dersNotlari.value("Fizik", -1); // varsayilan deger

    // Anahtarlar ve degerler
    qDebug() << "Dersler:" << dersNotlari.keys();
    qDebug() << "Notlar:" << dersNotlari.values();
    qDebug() << "contains('Veritabani'):"
             << dersNotlari.contains("Veritabani");

    // --- 4. QSet Temel Kullanim ---
    qDebug() << "\n--- 4. QSet Temel Kullanim ---";
    QSet<QString> bolumDersleri;
    bolumDersleri.insert("Algoritma");
    bolumDersleri.insert("Veritabani");
    bolumDersleri.insert("Ag Programlama");
    bolumDersleri.insert("Algoritma");  // zaten var, eklenmez

    qDebug() << "Ders sayisi:" << bolumDersleri.size(); // 3

    QSet<QString> secilenDersler;
    secilenDersler.insert("Algoritma");
    secilenDersler.insert("Yapay Zeka");

    // Kesisim
    QSet<QString> ortak = bolumDersleri;
    ortak.intersect(secilenDersler);
    qDebug() << "Ortak dersler:" << ortak;

    // Birlesim
    QSet<QString> tumDersler = bolumDersleri;
    tumDersler.unite(secilenDersler);
    qDebug() << "Tum dersler:" << tumDersler;

    // --- 5. Implicit Sharing ---
    qDebug() << "\n--- 5. Implicit Sharing ---";
    QList<int> a = {1, 2, 3, 4, 5};
    QList<int> b = a;  // Veri KOPYALANMAZ, paylasim

    qDebug() << "a:" << a;
    qDebug() << "b:" << b;
    qDebug() << "a ve b ayni veriyi paylasiyor (kopyalama olmadi)";

    b.append(6);  // Simdi gercek kopyalama gerceklesir (COW)
    qDebug() << "b.append(6) sonrasi:";
    qDebug() << "a:" << a;  // degismedi
    qDebug() << "b:" << b;  // 6 eklendi

    // --- 6. OBYS: Ogrenci Sozlugu ---
    qDebug() << "\n--- 6. OBYS: Ogrenci Sozlugu ---";
    OgrenciSozlugu sozluk;
    sozluk.ekle(1001, "Ali Yilmaz");
    sozluk.ekle(1002, "Ayse Kaya");
    sozluk.ekle(1003, "Mehmet Demir");
    sozluk.ekle(1004, "Fatma Celik");

    qDebug() << "\nNumaraya gore arama:";
    qDebug() << "  1002 ->" << sozluk.ara(1002);
    qDebug() << "  9999 ->" << sozluk.ara(9999);

    qDebug() << "\nIsme gore arama:";
    qDebug() << "  Mehmet Demir -> numara:" << sozluk.numaraBul("Mehmet Demir");

    qDebug() << "\nTum ogrenciler:";
    sozluk.listele();

    // Numara listesi
    QList<int> tumNumaralar = sozluk.numaralar();
    qDebug() << "\nTum numaralar:" << tumNumaralar;

    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== Qt Kapsayicilari ===

--- 1. QList Temel Kullanim ---
Notlar: QList(85, 92, 78, 95, 88)
Boyut: 5
Ilk: 85 Son: 88
at(2): 78
contains(92): true
removeOne(78) sonrasi: QList(85, 92, 95, 88)
Ortalama (std::accumulate): 90

--- 2. QList vs std::vector ---
std::vector boyut: 5
QList'e donusum: QList(85, 92, 78, 95, 88)
std::vector'e geri: 5 eleman

--- 3. QMap Temel Kullanim ---
Ders notlari: QMap(("Algoritma", 85)("Veritabani", 92)("Yazilim Gelistirme", 78))
Algoritma notu: 85
Olmayan ders: -1
Dersler: ("Algoritma", "Veritabani", "Yazilim Gelistirme")
Notlar: (85, 92, 78)
contains('Veritabani'): true

--- 4. QSet Temel Kullanim ---
Ders sayisi: 3
Ortak dersler: QSet("Algoritma")
Tum dersler: QSet("Ag Programlama", "Algoritma", "Veritabani", "Yapay Zeka")

--- 5. Implicit Sharing ---
a: QList(1, 2, 3, 4, 5)
b: QList(1, 2, 3, 4, 5)
a ve b ayni veriyi paylasiyor (kopyalama olmadi)
b.append(6) sonrasi:
a: QList(1, 2, 3, 4, 5)
b: QList(1, 2, 3, 4, 5, 6)

--- 6. OBYS: Ogrenci Sozlugu ---
  Eklendi: 1001 -> "Ali Yilmaz"
  Eklendi: 1002 -> "Ayse Kaya"
  Eklendi: 1003 -> "Mehmet Demir"
  Eklendi: 1004 -> "Fatma Celik"

Numaraya gore arama:
  1002 -> "Ayse Kaya"
  9999 -> "Bulunamadi"

Isme gore arama:
  Mehmet Demir -> numara: 1003

Tum ogrenciler:
  --- Ogrenci Listesi ---
  1001: Ali Yilmaz
  1002: Ayse Kaya
  1003: Mehmet Demir
  1004: Fatma Celik
  Toplam: 4 ogrenci

Tum numaralar: QList(1001, 1002, 1003, 1004)
*/

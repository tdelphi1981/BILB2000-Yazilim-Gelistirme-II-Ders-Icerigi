/**
 * Lab 04 - Çalışma 6 Başlangıç Kodu
 * QString ve Qt Kapsayıcıları
 *
 * Bu dosya Lab 04 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - QString temel işlemler (arg, split, join, contains, trimmed)
 * - QStringList ile liste işlemleri
 * - QMap ile anahtar-değer eşlemesi
 * - QList ve QSet temel kullanım
 * - std::accumulate ile Qt kapsayıcı üzerinde STL algoritması
 * - Implicit sharing (copy-on-write) kavramı
 *
 * Lab: 04 - Qt Framework'üne Giriş
 * Çalışma: 6 - QString ve Qt Kapsayıcıları (20 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştırma: ./app
 *
 * Not: Bu calismada GUI gerekmez, QCoreApplication yeterlidir.
 */

#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QDebug>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "=== Calisma 6: QString ve Qt Kapsayicilari ===\n";

    // QString temel islemler
    QString isim = "Ali Yilmaz";
    qDebug() << "Isim:" << isim;
    qDebug() << "Uzunluk:" << isim.size();
    qDebug() << "Buyuk harf:" << isim.toUpper();

    // arg() ile formatlama
    QString bilgi = QString("Ogrenci: %1, No: %2")
                        .arg(isim).arg(1001);
    qDebug() << bilgi;

    // split ve QStringList
    QString csv = "Algoritma,Veritabani,Yazilim";
    QStringList dersler = csv.split(",");
    qDebug() << "\nDersler:" << dersler;
    qDebug() << "Join:" << dersler.join(" | ");

    // QMap ile ogrenci sozlugu
    QMap<int, QString> ogrenciler;
    ogrenciler[1001] = "Ali Yilmaz";
    ogrenciler[1002] = "Ayse Kaya";
    qDebug() << "\n1001:" << ogrenciler.value(1001);
    qDebug() << "Anahtarlar:" << ogrenciler.keys();

    // === GOREV 6.1 - QString Islemleri (10 dk) ===
    qDebug() << "\n[GOREV 6.1]";
    // TODO: a) isimDogrula() fonksiyonu yazin:
    //          - Parametre: const QString& isim
    //          - Bos olmadigini kontrol et (isEmpty())
    //          - Boslukla ayirildiginda en az 2 kelime olmali (split, size)
    //          - trimmed() ile bas/son bosluklari temizle
    //
    //          bool isimDogrula(const QString& isim) {
    //              QString temiz = isim.trimmed();
    //              if (temiz.isEmpty()) return false;
    //              QStringList parcalar = temiz.split(" ",
    //                  Qt::SkipEmptyParts);
    //              return parcalar.size() >= 2;
    //          }
    //
    // TODO: b) arg() ile OBYS bilgi karti olusturun:
    //          QString kart = QString("Ogrenci: %1\nNo: %2\nNot: %3")
    //              .arg(isim).arg(1001).arg(3.75, 0, 'f', 2);
    //          qDebug() << kart;
    //
    // TODO: c) std::string ile donusum yapin:
    //          std::string stdIsim = isim.toStdString();
    //          QString geriDonus = QString::fromStdString(stdIsim);
    //          qDebug() << "Donusum:" << geriDonus;
    // ============================================

    // === GOREV 6.2 - Qt Kapsayicilari ile OBYS Sozlugu (10 dk) ===
    qDebug() << "\n[GOREV 6.2]";
    // TODO: a) QMap<int, QString> ile 5 ogrencilik sozluk olusturun.
    //          insert() ve operator[] ile ekleme yapin:
    //          QMap<int, QString> sozluk;
    //          sozluk.insert(1001, "Ali Yilmaz");
    //          sozluk[1002] = "Ayse Kaya";
    //          sozluk.insert(1003, "Mehmet Demir");
    //          sozluk[1004] = "Zeynep Can";
    //          sozluk.insert(1005, "Can Ozturk");
    //
    // TODO: b) Var olmayan bir numarayi value() ile arayin:
    //          qDebug() << "9999:" << sozluk.value(9999, "Bilinmiyor");
    //
    // TODO: c) QList<int> ile not listesi olusturun ve
    //          std::accumulate() ile ortalama hesaplayin:
    //          #include <numeric>
    //          QList<int> notlar = {85, 90, 75, 60, 95};
    //          double toplam = std::accumulate(notlar.begin(),
    //                                          notlar.end(), 0);
    //          double ortalama = toplam / notlar.size();
    //          qDebug() << "Ortalama:" << ortalama;
    //
    // TODO: d) QSet<QString> ile ders kumesi olusturup iki kumenin
    //          kesisimini (intersect()) bulun:
    //          QSet<QString> kume1 = {"Algoritma", "Veritabani", "Yazilim"};
    //          QSet<QString> kume2 = {"Veritabani", "Ag", "Yazilim"};
    //          QSet<QString> kesisim = kume1;
    //          kesisim.intersect(kume2);
    //          qDebug() << "Kesisim:" << kesisim;
    // ============================================

    return 0;
}

/*
BEKLENEN CIKTI (temel kod):
----------------------------
=== Calisma 6: QString ve Qt Kapsayicilari ===

Isim: "Ali Yilmaz"
Uzunluk: 10
Buyuk harf: "ALI YILMAZ"
"Ogrenci: Ali Yilmaz, No: 1001"

Dersler: QList("Algoritma", "Veritabani", "Yazilim")
Join: "Algoritma | Veritabani | Yazilim"

1001: "Ali Yilmaz"
Anahtarlar: QList(1001, 1002)

[GOREV 6.1]

[GOREV 6.2]
*/

/**
 * OBYS Binary Kayit/Yukleme
 *
 * Bu program OBYS Ogrenci sinifinin binary formatta
 * dosyaya kaydedilmesi ve yuklenmesini gosterir.
 * Magic number, versiyon kontrolu ve hata yonetimi icerir.
 *
 * Ogrenilecek kavramlar:
 * - Ozel sinif icin operator<< ve operator>> overload
 * - Magic number ile dosya dogrulama (0x4F425953 = "OBYS")
 * - Dosya versiyon kontrolu
 * - QFile ve QDataStream entegrasyonu
 * - Hata yonetimi stratejileri
 *
 * Bolum: 07 - Qt Ileri Bilesenler
 * Unite: 5 - QDataStream ile Binary Veri Kaydetme
 *
 * Derleme: g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Core) -o obys 08_obys_binary_kayit.cpp
 * Calistirma: ./obys
 */

#include <QCoreApplication>
#include <QDataStream>
#include <QFile>
#include <QList>
#include <QDebug>

// Dosya format sabitleri
constexpr quint32 OBYS_MAGIC = 0x4F425953;  // "OBYS" ASCII
constexpr quint16 OBYS_VERSION = 1;

// Ogrenci sinifi
struct Ogrenci {
    QString numara;
    QString ad;
    QString bolum;
    double gpa = 0.0;

    // QDataStream icin friend operator<< (yazma)
    friend QDataStream& operator<<(QDataStream& out,
                                    const Ogrenci& ogr) {
        out << ogr.numara << ogr.ad << ogr.bolum << ogr.gpa;
        return out;
    }

    // QDataStream icin friend operator>> (okuma)
    friend QDataStream& operator>>(QDataStream& in,
                                    Ogrenci& ogr) {
        in >> ogr.numara >> ogr.ad >> ogr.bolum >> ogr.gpa;
        return in;
    }
};

// Binary dosyaya kaydet
bool binaryKaydet(const QString& dosyaYolu,
                  const QList<Ogrenci>& liste) {
    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::WriteOnly)) {
        qWarning() << "[HATA] Dosya acilamadi:" << dosyaYolu;
        return false;
    }

    QDataStream out(&dosya);
    out.setVersion(QDataStream::Qt_6_0);

    // Dosya basligi: magic number + versiyon
    out << OBYS_MAGIC;
    out << OBYS_VERSION;

    // Ogrenci sayisi ve verileri
    out << static_cast<quint32>(liste.size());
    for (const auto& ogr : liste) {
        out << ogr;
    }

    qDebug() << "[OK] Kaydedildi:" << dosyaYolu;
    qDebug() << "Ogrenci sayisi:" << liste.size();
    qDebug() << "Dosya boyutu:" << dosya.size() << "byte";
    return true;
}

// Binary dosyadan yukle
QList<Ogrenci> binaryYukle(const QString& dosyaYolu) {
    QList<Ogrenci> liste;

    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::ReadOnly)) {
        qWarning() << "[HATA] Dosya acilamadi:" << dosyaYolu;
        return liste;
    }

    QDataStream in(&dosya);
    in.setVersion(QDataStream::Qt_6_0);

    // Magic number dogrulama
    quint32 magic;
    in >> magic;
    if (magic != OBYS_MAGIC) {
        qWarning() << "[HATA] Gecersiz dosya formati!"
                   << "Beklenen:" << Qt::hex << OBYS_MAGIC
                   << "Okunan:" << magic;
        return liste;
    }

    // Versiyon kontrolu
    quint16 versiyon;
    in >> versiyon;
    if (versiyon > OBYS_VERSION) {
        qWarning() << "[HATA] Desteklenmeyen versiyon:"
                   << versiyon;
        return liste;
    }

    // Ogrenci verilerini oku
    quint32 sayi;
    in >> sayi;

    for (quint32 i = 0; i < sayi; ++i) {
        Ogrenci ogr;
        in >> ogr;

        if (in.status() != QDataStream::Ok) {
            qWarning() << "[HATA] Veri okuma hatasi, satir:" << i;
            break;
        }
        liste.append(ogr);
    }

    qDebug() << "[OK] Yuklendi:" << dosyaYolu;
    qDebug() << "Ogrenci sayisi:" << liste.size();
    return liste;
}

// Ogrenci listesini ekrana yazdir
void listeyiGoster(const QList<Ogrenci>& liste) {
    qDebug() << "\n--- Ogrenci Listesi ---";
    for (const auto& ogr : liste) {
        qDebug().noquote()
            << QString("%1 | %2 | %3 | GPA: %4")
                   .arg(ogr.numara, -10)
                   .arg(ogr.ad, -20)
                   .arg(ogr.bolum, -25)
                   .arg(ogr.gpa, 0, 'f', 2);
    }
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    // Ornek ogrenci verileri
    QList<Ogrenci> ogrenciler = {
        {"2024001", "Ali Yilmaz",    "Bilgisayar Bilimleri",     3.45},
        {"2024002", "Ayse Kaya",     "Yazilim Muhendisligi",     3.82},
        {"2024003", "Mehmet Demir",  "Bilgisayar Muhendisligi",  2.95},
        {"2024004", "Fatma Celik",   "Bilgisayar Bilimleri",     3.67},
        {"2024005", "Ahmet Ozturk",  "Yapay Zeka Muhendisligi",  3.21}
    };

    const QString dosyaYolu = "ogrenciler.obys";

    // Kaydet
    qDebug() << "=== Binary Kaydetme ===";
    if (!binaryKaydet(dosyaYolu, ogrenciler)) {
        return 1;
    }

    // Yukle
    qDebug() << "\n=== Binary Yukleme ===";
    QList<Ogrenci> yuklenen = binaryYukle(dosyaYolu);
    listeyiGoster(yuklenen);

    // Dogrulama
    qDebug() << "\n=== Dogrulama ===";
    bool basarili = (ogrenciler.size() == yuklenen.size());
    for (int i = 0; i < ogrenciler.size() && basarili; ++i) {
        basarili = (ogrenciler[i].numara == yuklenen[i].numara &&
                    ogrenciler[i].ad == yuklenen[i].ad &&
                    ogrenciler[i].gpa == yuklenen[i].gpa);
    }
    qDebug() << "Veri butunlugu:"
             << (basarili ? "[OK] Basarili" : "[HATA] Farklilik var!");

    // Temizlik
    QFile::remove(dosyaYolu);
    qDebug() << "[OK] Test dosyasi silindi.";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ----------------
 * === Binary Kaydetme ===
 * [OK] Kaydedildi: "ogrenciler.obys"
 * Ogrenci sayisi: 5
 * Dosya boyutu: ... byte
 *
 * === Binary Yukleme ===
 * [OK] Yuklendi: "ogrenciler.obys"
 * Ogrenci sayisi: 5
 *
 * --- Ogrenci Listesi ---
 * 2024001    | Ali Yilmaz           | Bilgisayar Bilimleri      | GPA: 3.45
 * 2024002    | Ayse Kaya            | Yazilim Muhendisligi      | GPA: 3.82
 * 2024003    | Mehmet Demir         | Bilgisayar Muhendisligi   | GPA: 2.95
 * 2024004    | Fatma Celik          | Bilgisayar Bilimleri      | GPA: 3.67
 * 2024005    | Ahmet Ozturk         | Yapay Zeka Muhendisligi   | GPA: 3.21
 *
 * === Dogrulama ===
 * Veri butunlugu: [OK] Basarili
 * [OK] Test dosyasi silindi.
 */

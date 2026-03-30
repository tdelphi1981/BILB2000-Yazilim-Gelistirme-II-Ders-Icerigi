/**
 * QDataStream Temel Kullanim
 *
 * Bu program QDataStream ile temel veri tiplerini binary formatta
 * dosyaya yazma ve dosyadan okuma islemlerini gosterir.
 * int, QString ve QList serilestirme ornekleri icerir.
 *
 * Ogrenilecek kavramlar:
 * - QDataStream ile binary yazma/okuma
 * - operator<< ve operator>> kullanimi
 * - setVersion() ile versiyon kontrolu
 * - Temel Qt veri tiplerinin serilestirmesi
 *
 * Bolum: 07 - Qt Ileri Bilesenler
 * Unite: 5 - QDataStream ile Binary Veri Kaydetme
 *
 * Derleme: g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Core) -o temel 07_qdatastream_temel.cpp
 * Calistirma: ./temel
 */

#include <QCoreApplication>
#include <QDataStream>
#include <QFile>
#include <QList>
#include <QDebug>

// Binary dosyaya temel veri tiplerini yaz
bool dosyayaYaz(const QString& dosyaYolu) {
    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::WriteOnly)) {
        qWarning() << "Dosya acilamadi:" << dosyaYolu;
        return false;
    }

    QDataStream out(&dosya);
    out.setVersion(QDataStream::Qt_6_0);

    // Temel tipler
    int ogrenciSayisi = 150;
    double ortalamaGpa = 2.85;
    QString bolumAdi = "Bilgisayar Bilimleri";

    // Yazma islemi: operator<< kullan
    out << ogrenciSayisi;
    out << ortalamaGpa;
    out << bolumAdi;

    // QList serilestirme
    QList<QString> dersler = {
        "Yazilim Gelistirme II",
        "Veri Yapilari",
        "Algoritma Analizi",
        "Isletim Sistemleri"
    };
    out << dersler;

    // QMap serilestirme
    QMap<QString, int> notlar;
    notlar["Ali Yilmaz"] = 85;
    notlar["Ayse Kaya"] = 92;
    notlar["Mehmet Demir"] = 78;
    out << notlar;

    qDebug() << "[OK] Veriler yazildi:" << dosyaYolu;
    qDebug() << "Dosya boyutu:" << dosya.size() << "byte";
    return true;
}

// Binary dosyadan verileri oku
bool dosyadanOku(const QString& dosyaYolu) {
    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::ReadOnly)) {
        qWarning() << "Dosya acilamadi:" << dosyaYolu;
        return false;
    }

    QDataStream in(&dosya);
    in.setVersion(QDataStream::Qt_6_0);

    // Ayni sirayla oku
    int ogrenciSayisi;
    double ortalamaGpa;
    QString bolumAdi;

    in >> ogrenciSayisi;
    in >> ortalamaGpa;
    in >> bolumAdi;

    QList<QString> dersler;
    in >> dersler;

    QMap<QString, int> notlar;
    in >> notlar;

    // Hata kontrolu
    if (in.status() != QDataStream::Ok) {
        qWarning() << "[HATA] Veri okuma hatasi!";
        return false;
    }

    // Sonuclari goster
    qDebug() << "\n--- Okunan Veriler ---";
    qDebug() << "Ogrenci sayisi:" << ogrenciSayisi;
    qDebug() << "Ortalama GPA:" << ortalamaGpa;
    qDebug() << "Bolum:" << bolumAdi;
    qDebug() << "\nDersler:";
    for (const auto& ders : dersler) {
        qDebug() << "  -" << ders;
    }
    qDebug() << "\nNotlar:";
    for (auto it = notlar.cbegin(); it != notlar.cend(); ++it) {
        qDebug() << "  " << it.key() << ":" << it.value();
    }

    return true;
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    const QString dosyaYolu = "temel_veri.dat";

    // Yazma
    if (!dosyayaYaz(dosyaYolu)) {
        return 1;
    }

    // Okuma
    if (!dosyadanOku(dosyaYolu)) {
        return 1;
    }

    // Temizlik
    QFile::remove(dosyaYolu);
    qDebug() << "\n[OK] Test dosyasi silindi.";

    return 0;
}

/*
 * BEKLENEN CIKTI:
 * ----------------
 * [OK] Veriler yazildi: "temel_veri.dat"
 * Dosya boyutu: ... byte
 *
 * --- Okunan Veriler ---
 * Ogrenci sayisi: 150
 * Ortalama GPA: 2.85
 * Bolum: "Bilgisayar Bilimleri"
 *
 * Dersler:
 *   - "Yazilim Gelistirme II"
 *   - "Veri Yapilari"
 *   - "Algoritma Analizi"
 *   - "Isletim Sistemleri"
 *
 * Notlar:
 *   "Ali Yilmaz" : 85
 *   "Ayse Kaya" : 92
 *   "Mehmet Demir" : 78
 *
 * [OK] Test dosyasi silindi.
 */

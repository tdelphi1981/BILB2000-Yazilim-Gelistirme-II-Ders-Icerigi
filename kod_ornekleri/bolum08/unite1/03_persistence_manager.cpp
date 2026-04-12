/**
 * 03_persistence_manager.cpp -- PersistenceManager uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * QFile + QDataStream kullanarak OBYSServis icerigini binary
 * dosyaya yazip okuma uygulamasini icerir. Dosya basina magic
 * ve version alani yerlestirilir. Her adimda savunmaci kontroller
 * yapilir ve hata mesaji uye degiskende saklanir.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Core ile birlikte.
 *
 * NOT: Bu dosya bagimsiz tek basina derlenmek uzere yazilmamistir;
 * 04_obys_v1_binary_kayit projesinin bir parcasi olarak kullanilir.
 * OBYSServis siniftan ogrenci/ders/not listesi alan ve ekleyen
 * stream operatoru asirikyukleri bulundugu varsayilir.
 */

#include "03_persistence_manager.h"

#include "obys_servis.h"   // 04_obys_v1_binary_kayit/include/...
#include "obys_domain.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QList>

bool PersistenceManager::kaydet(const QString& dosyaYolu,
                                 OBYSServis& servis) {
    m_sonHata.clear();

    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        m_sonHata = QObject::tr("Dosya yazma için açılamadı: %1")
                        .arg(dosya.errorString());
        return false;
    }

    QDataStream akis(&dosya);
    akis.setVersion(QDataStream::Qt_6_5);

    // Header: magic + version
    akis << MAGIC << VERSION;

    // Icerik: ogrenciler, dersler, notlar
    const QList<Ogrenci>   ogrenciler = servis.tumOgrenciler();
    const QList<Ders>      dersler    = servis.tumDersler();
    const QList<DersKaydi> notlar     = servis.tumNotlar();

    akis << static_cast<quint32>(ogrenciler.size());
    for (const auto& o : ogrenciler) {
        akis << o;
    }

    akis << static_cast<quint32>(dersler.size());
    for (const auto& d : dersler) {
        akis << d;
    }

    akis << static_cast<quint32>(notlar.size());
    for (const auto& n : notlar) {
        akis << n;
    }

    if (akis.status() != QDataStream::Ok) {
        m_sonHata = QObject::tr("Yazma sırasında akış hatası oluştu.");
        dosya.close();
        return false;
    }

    dosya.close();
    return true;
}

bool PersistenceManager::yukle(const QString& dosyaYolu,
                                OBYSServis& servis) {
    m_sonHata.clear();

    QFileInfo bilgi(dosyaYolu);
    if (!bilgi.exists() || !bilgi.isFile()) {
        m_sonHata = QObject::tr("Dosya bulunamadı: %1").arg(dosyaYolu);
        return false;
    }

    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::ReadOnly)) {
        m_sonHata = QObject::tr("Dosya okuma için açılamadı: %1")
                        .arg(dosya.errorString());
        return false;
    }

    QDataStream akis(&dosya);
    akis.setVersion(QDataStream::Qt_6_5);

    // Header kontrolu
    quint32 magic = 0;
    quint32 version = 0;
    akis >> magic >> version;
    if (akis.status() != QDataStream::Ok) {
        m_sonHata = QObject::tr("Dosya başlığı okunamadı.");
        return false;
    }
    if (magic != MAGIC) {
        m_sonHata = QObject::tr("Geçersiz dosya imzası (MAGIC uyuşmuyor).");
        return false;
    }
    if (version > VERSION) {
        m_sonHata = QObject::tr(
            "Dosya sürümü desteklenmiyor: %1 (beklenen <= %2)")
                .arg(version).arg(VERSION);
        return false;
    }

    // Servisi temizle ve doldur
    servis.temizle();

    // Ogrenciler
    quint32 n = 0;
    akis >> n;
    for (quint32 i = 0; i < n; ++i) {
        Ogrenci o;
        akis >> o;
        if (akis.status() != QDataStream::Ok) {
            m_sonHata = QObject::tr("Öğrenci verisi okunurken hata.");
            return false;
        }
        servis.ogrenciEkle(o);
    }

    // Dersler
    akis >> n;
    for (quint32 i = 0; i < n; ++i) {
        Ders d;
        akis >> d;
        if (akis.status() != QDataStream::Ok) {
            m_sonHata = QObject::tr("Ders verisi okunurken hata.");
            return false;
        }
        servis.dersEkle(d);
    }

    // Notlar
    akis >> n;
    for (quint32 i = 0; i < n; ++i) {
        DersKaydi k;
        akis >> k;
        if (akis.status() != QDataStream::Ok) {
            m_sonHata = QObject::tr("Not verisi okunurken hata.");
            return false;
        }
        servis.dersKaydiEkle(k);
    }

    dosya.close();
    return true;
}

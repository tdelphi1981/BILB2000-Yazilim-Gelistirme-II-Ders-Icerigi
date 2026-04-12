/**
 * persistence_manager.cpp -- PersistenceManager (proje surumu)
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * 03 numarali ornekten bu projeye kopyalanmistir. OBYSServis ile
 * QDataStream uzerinden binary serilestirme yapar.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Core ile birlikte.
 */

#include "persistence_manager.h"

#include "obys_domain.h"
#include "obys_servis.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QList>

bool PersistenceManager::kaydet(const QString& dosyaYolu,
                                 OBYSServis& servis) {
    m_sonHata.clear();

    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        m_sonHata = QCoreApplication::translate(
            "PersistenceManager",
            "Dosya yazma için açılamadı: %1")
                .arg(dosya.errorString());
        return false;
    }

    QDataStream akis(&dosya);
    akis.setVersion(QDataStream::Qt_6_5);

    akis << MAGIC << VERSION;

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
        m_sonHata = QCoreApplication::translate(
            "PersistenceManager",
            "Yazma sırasında akış hatası oluştu.");
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
        m_sonHata = QCoreApplication::translate(
            "PersistenceManager",
            "Dosya bulunamadı: %1").arg(dosyaYolu);
        return false;
    }

    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::ReadOnly)) {
        m_sonHata = QCoreApplication::translate(
            "PersistenceManager",
            "Dosya okuma için açılamadı: %1")
                .arg(dosya.errorString());
        return false;
    }

    QDataStream akis(&dosya);
    akis.setVersion(QDataStream::Qt_6_5);

    quint32 magic = 0;
    quint32 version = 0;
    akis >> magic >> version;
    if (akis.status() != QDataStream::Ok) {
        m_sonHata = QCoreApplication::translate(
            "PersistenceManager", "Dosya başlığı okunamadı.");
        return false;
    }
    if (magic != MAGIC) {
        m_sonHata = QCoreApplication::translate(
            "PersistenceManager",
            "Geçersiz dosya imzası (MAGIC uyuşmuyor).");
        return false;
    }
    if (version > VERSION) {
        m_sonHata = QCoreApplication::translate(
            "PersistenceManager",
            "Dosya sürümü desteklenmiyor: %1 (beklenen <= %2)")
                .arg(version).arg(VERSION);
        return false;
    }

    servis.temizle();

    quint32 n = 0;
    akis >> n;
    for (quint32 i = 0; i < n; ++i) {
        Ogrenci o;
        akis >> o;
        if (akis.status() != QDataStream::Ok) {
            m_sonHata = QCoreApplication::translate(
                "PersistenceManager",
                "Öğrenci verisi okunurken hata.");
            return false;
        }
        servis.ogrenciEkle(o);
    }

    akis >> n;
    for (quint32 i = 0; i < n; ++i) {
        Ders d;
        akis >> d;
        if (akis.status() != QDataStream::Ok) {
            m_sonHata = QCoreApplication::translate(
                "PersistenceManager",
                "Ders verisi okunurken hata.");
            return false;
        }
        servis.dersEkle(d);
    }

    akis >> n;
    for (quint32 i = 0; i < n; ++i) {
        DersKaydi k;
        akis >> k;
        if (akis.status() != QDataStream::Ok) {
            m_sonHata = QCoreApplication::translate(
                "PersistenceManager",
                "Not verisi okunurken hata.");
            return false;
        }
        servis.dersKaydiEkle(k);
    }

    dosya.close();
    return true;
}

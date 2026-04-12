#include "persistence_manager.h"
#include "obys_domain.h"
#include "obys_servis.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>

bool PersistenceManager::kaydet(const QString& dosyaYolu,
                                 OBYSServis& servis) {
    m_sonHata.clear();
    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::WriteOnly
                    | QIODevice::Truncate)) {
        m_sonHata = QString("Dosya açılamadı: %1")
                        .arg(dosya.errorString());
        return false;
    }

    QDataStream akis(&dosya);
    akis.setVersion(QDataStream::Qt_6_5);
    akis << MAGIC << VERSION;

    // Ogrencileri yaz
    const auto ogrenciler = servis.tumOgrenciler();
    akis << static_cast<quint32>(ogrenciler.size());
    for (const auto& o : ogrenciler) { akis << o; }

    // Dersleri yaz
    const auto dersler = servis.tumDersler();
    akis << static_cast<quint32>(dersler.size());
    for (const auto& d : dersler) { akis << d; }

    // Notlari yaz
    const auto notlar = servis.tumNotlar();
    akis << static_cast<quint32>(notlar.size());
    for (const auto& n : notlar) { akis << n; }

    if (akis.status() != QDataStream::Ok) {
        m_sonHata = "Yazma sırasında akış hatası.";
        return false;
    }
    return true;
}

bool PersistenceManager::yukle(const QString& dosyaYolu,
                                OBYSServis& servis) {
    m_sonHata.clear();
    if (!QFileInfo::exists(dosyaYolu)) {
        m_sonHata = QString("Dosya bulunamadı: %1")
                        .arg(dosyaYolu);
        return false;
    }

    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::ReadOnly)) {
        m_sonHata = QString("Dosya okunamadı: %1")
                        .arg(dosya.errorString());
        return false;
    }

    QDataStream akis(&dosya);
    akis.setVersion(QDataStream::Qt_6_5);

    quint32 magic = 0, version = 0;
    akis >> magic >> version;

    if (magic != MAGIC) {
        m_sonHata = "Geçersiz dosya imzası.";
        return false;
    }
    if (version > VERSION) {
        m_sonHata = QString("Desteklenmeyen sürüm: %1")
                        .arg(version);
        return false;
    }

    servis.temizle();

    // Ogrencileri oku
    quint32 n = 0;
    akis >> n;
    for (quint32 i = 0; i < n; ++i) {
        Ogrenci o; akis >> o;
        if (akis.status() != QDataStream::Ok) {
            m_sonHata = "Öğrenci okunurken hata.";
            return false;
        }
        servis.ogrenciEkle(o);
    }

    // Dersleri oku
    akis >> n;
    for (quint32 i = 0; i < n; ++i) {
        Ders d; akis >> d;
        if (akis.status() != QDataStream::Ok) {
            m_sonHata = "Ders okunurken hata.";
            return false;
        }
        servis.dersEkle(d);
    }

    // Notlari oku
    akis >> n;
    for (quint32 i = 0; i < n; ++i) {
        DersKaydi k; akis >> k;
        if (akis.status() != QDataStream::Ok) {
            m_sonHata = "Not okunurken hata.";
            return false;
        }
        servis.dersKaydiEkle(k);
    }

    return true;
}

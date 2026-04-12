/**
 * obys_servis.h -- Basit OBYSServis stub'i
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * Bu dosya, PersistenceManager ve ana pencere tarafindan
 * kullanilan basit bir servis sinifidir. Gercek projede bu servis
 * veri tabani / repo katmani ile entegre olur; burada bellek-ici
 * listelerle calisir.
 *
 * Derleme: Qt6::Core yeterlidir.
 */

#ifndef OBYS_SERVIS_H
#define OBYS_SERVIS_H

#include "obys_domain.h"

#include <QList>

class OBYSServis {
public:
    OBYSServis() = default;

    // Listeleri toplu almak icin (PersistenceManager kullanir)
    QList<Ogrenci>   tumOgrenciler() const { return m_ogrenciler; }
    QList<Ders>      tumDersler()    const { return m_dersler;    }
    QList<DersKaydi> tumNotlar()     const { return m_notlar;     }

    // Toplu ekleme yardimcilari
    void ogrenciEkle(const Ogrenci& o)    { m_ogrenciler.append(o); }
    void dersEkle(const Ders& d)          { m_dersler.append(d);    }
    void dersKaydiEkle(const DersKaydi& k){ m_notlar.append(k);     }

    // DTO'dan not ekle (dialog akisi icin)
    void notEkle(const NotDTO& dto) {
        DersKaydi k;
        k.ogrenciId = dto.ogrenciId;
        k.dersId    = dto.dersId;
        k.notu      = dto.notu;
        m_notlar.append(k);
    }

    // Persistence icin temizleme
    void temizle() {
        m_ogrenciler.clear();
        m_dersler.clear();
        m_notlar.clear();
    }

    // Basit ornek verilerle doldurur (Yeni dosya icin)
    void ornekVerileriYukle() {
        temizle();
        m_ogrenciler = {
            {1, QStringLiteral("2023001"), QStringLiteral("Ayşe Yılmaz")},
            {2, QStringLiteral("2023002"), QStringLiteral("Mehmet Demir")},
            {3, QStringLiteral("2023003"), QStringLiteral("Zeynep Kaya")},
        };
        m_dersler = {
            {10, QStringLiteral("BILB2000"),
             QStringLiteral("Yazılım Geliştirme II"), 4},
            {11, QStringLiteral("BILB2001"),
             QStringLiteral("Veri Yapıları"),         4},
        };
    }

private:
    QList<Ogrenci>   m_ogrenciler;
    QList<Ders>      m_dersler;
    QList<DersKaydi> m_notlar;
};

#endif // OBYS_SERVIS_H

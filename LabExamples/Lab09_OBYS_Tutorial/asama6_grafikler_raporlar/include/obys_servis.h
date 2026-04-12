#ifndef OBYS_SERVIS_H
#define OBYS_SERVIS_H

#include "obys_domain.h"
#include <QList>

class OBYSServis {
public:
    OBYSServis() = default;

    // Veri erisimi
    QList<Ogrenci>   tumOgrenciler() const
        { return m_ogrenciler; }
    QList<Ders>      tumDersler()    const
        { return m_dersler; }
    QList<DersKaydi> tumNotlar()     const
        { return m_notlar; }

    // Ekleme
    void ogrenciEkle(const Ogrenci& o)
        { m_ogrenciler.append(o); }
    void dersEkle(const Ders& d)
        { m_dersler.append(d); }
    void dersKaydiEkle(const DersKaydi& k)
        { m_notlar.append(k); }

    // DTO'dan not ekle (dialog akisi icin)
    void notEkle(const NotDTO& dto) {
        DersKaydi k;
        k.ogrenciId = dto.ogrenciId;
        k.dersId    = dto.dersId;
        k.notu      = dto.notu;
        m_notlar.append(k);
    }

    // Temizle (yuklemeden once cagirilir)
    void temizle() {
        m_ogrenciler.clear();
        m_dersler.clear();
        m_notlar.clear();
    }

    // Ornek verilerle doldur
    void ornekVerileriYukle() {
        temizle();
        m_ogrenciler = {
            {1, "2023001",
             QStringLiteral("Ayşe Yılmaz")},
            {2, "2023002",
             QStringLiteral("Mehmet Demir")},
            {3, "2023003",
             QStringLiteral("Zeynep Kaya")},
        };
        m_dersler = {
            {10, "BILB2000",
             QStringLiteral(
                 "Yazılım Geliştirme II"), 4},
            {11, "BILB2001",
             QStringLiteral(
                 "Veri Yapıları"), 4},
        };
    }

private:
    QList<Ogrenci>   m_ogrenciler;
    QList<Ders>      m_dersler;
    QList<DersKaydi> m_notlar;
};

#endif // OBYS_SERVIS_H

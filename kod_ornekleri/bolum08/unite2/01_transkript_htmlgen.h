/**
 * 01_transkript_htmlgen.h -- TranskriptUretici sinifi basligi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 * Konu 1 - QTextDocument ile HTML Transkript Uretimi
 *
 * Tek bir ogrencinin transkriptini HTML bicimli QString olarak
 * ureten sinif. Uretilen metin QTextDocument::setHtml() ile
 * yuklenebilir. HTML icerigi; basliktan, ogrenci bilgilerinden,
 * derslerden olusan bir tablodan ve ozet bilgilerinden (GPA)
 * olusur. Tum dinamik metinler toHtmlEscaped() ile kacisli hale
 * getirilir.
 *
 * Derleme: Qt6::Core yeterlidir.
 */

#ifndef TRANSKRIPT_HTMLGEN_H
#define TRANSKRIPT_HTMLGEN_H

#include "obys_domain.h"

#include <QList>
#include <QString>

class TranskriptUretici {
public:
    TranskriptUretici() = default;

    // Belirli bir ogrencinin, verilen kayit listesindeki derslerine
    // ait transkriptini HTML olarak uretir. Dersler, dersId ile
    // verilen dersListesi uzerinden cozulur.
    QString html(const Ogrenci&            ogrenci,
                 const QList<Ders>&        dersListesi,
                 const QList<DersKaydi>&   kayitlar) const;

private:
    // Sayisal not / harf notu -> 4.0 sistemi katsayisi
    static double gpaKatsayisi(const NotTuru& n);

    // NotTuru'nun okunabilir metin temsilini dondurur
    static QString notMetni(const NotTuru& n);

    // HTML sablonu (sayfa sarmalayici)
    static QString sablonSar(const QString& govde);
};

#endif // TRANSKRIPT_HTMLGEN_H

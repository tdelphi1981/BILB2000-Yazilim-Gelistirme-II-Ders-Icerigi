/**
 * transkript_uretici.h -- OBYS v2 icinde transkript HTML ureticisi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 * Konu 1 - QTextDocument ile HTML Transkript Uretimi
 *
 * Konu 1'deki TranskriptUretici sinifinin proje icine yerlestirilmis
 * halidir. Tek bir ogrencinin derslerini HTML tablosu olarak
 * bicimlendirir; QTextDocument::setHtml() ile yuklenip
 * yazdirilabilir.
 */

#ifndef TRANSKRIPT_URETICI_H
#define TRANSKRIPT_URETICI_H

#include "obys_domain.h"

#include <QList>
#include <QString>

class TranskriptUretici {
public:
    TranskriptUretici() = default;

    QString html(const Ogrenci&          ogrenci,
                 const QList<Ders>&      dersListesi,
                 const QList<DersKaydi>& kayitlar) const;

private:
    static double  gpaKatsayisi(const NotTuru& n);
    static QString notMetni(const NotTuru& n);
    static QString sablonSar(const QString& govde);
};

#endif // TRANSKRIPT_URETICI_H

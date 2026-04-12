#ifndef TRANSKRIPT_URETICI_H
#define TRANSKRIPT_URETICI_H

#include "obys_domain.h"
#include <QList>
#include <QString>

class TranskriptUretici {
public:
    QString html(
        const Ogrenci& ogrenci,
        const QList<Ders>& dersler,
        const QList<DersKaydi>& kayitlar) const;

private:
    static double gpaKatsayisi(
        const NotTuru& n);
    static QString notMetni(
        const NotTuru& n);
    static QString sablonSar(
        const QString& govde);
};

#endif // TRANSKRIPT_URETICI_H

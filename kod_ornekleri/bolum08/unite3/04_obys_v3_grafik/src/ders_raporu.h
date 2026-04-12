/**
 * ders_raporu.h -- OBYS v2 icinde toplu ders raporu ureticisi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 * Konu 3 - Toplu Rapor: QTextDocument ile Cok Sayfali Belge
 *
 * Konu 3'teki DersRaporu sinifinin proje icine yerlestirilmis
 * halidir. Verilen ders icin cok sayfali bir QTextDocument uretir;
 * yazdirma ve PDF'e aktarma akislarinin girdisi olarak kullanilir.
 */

#ifndef DERS_RAPORU_H
#define DERS_RAPORU_H

#include "obys_domain.h"

#include <QList>

class QTextDocument;

class DersRaporu {
public:
    DersRaporu() = default;

    QTextDocument* olustur(const Ders&             ders,
                           const QList<Ogrenci>&   ogrenciler,
                           const QList<DersKaydi>& kayitlar) const;

private:
    static bool sayisalDegerAl(const NotTuru& n, double& out);
};

#endif // DERS_RAPORU_H

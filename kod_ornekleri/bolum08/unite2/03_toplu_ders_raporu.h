/**
 * 03_toplu_ders_raporu.h -- DersRaporu sinifi basligi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 * Konu 3 - Toplu Rapor: QTextDocument ile Cok Sayfali Belge
 *
 * Tek bir dersin tum ogrenci notlarini ozetleyen, istatistik ve
 * detay bolumlerinden olusan bir QTextDocument'i olusturan sinif.
 * Olusturulan belge cok sayfali olabilir; bolumler arasinda
 * QTextBlockFormat::PageBreak_AlwaysBefore ile sayfa sonu eklenir.
 *
 * Derleme: Qt6::Gui (QTextDocument icin) yeterlidir.
 */

#ifndef TOPLU_DERS_RAPORU_H
#define TOPLU_DERS_RAPORU_H

#include "obys_domain.h"

#include <QList>

class QTextDocument;

class DersRaporu {
public:
    DersRaporu() = default;

    // Verilen ders icin rapor belgesi uretir. Rapor:
    //   - Baslik sayfasi (ders bilgileri)
    //   - Istatistik sayfasi (ortalama, gecen/kalan sayisi)
    //   - Detay sayfasi (tum ogrencilerin not listesi)
    // Cagri yapan taraf, donen QTextDocument'in sahipligini
    // ustlenir.
    QTextDocument* olustur(const Ders&             ders,
                           const QList<Ogrenci>&   ogrenciler,
                           const QList<DersKaydi>& kayitlar) const;

private:
    // Sayisal degere cevrilebilen notlari dondurur (muaf -> nullopt)
    static bool sayisalDegerAl(const NotTuru& n, double& out);
};

#endif // TOPLU_DERS_RAPORU_H

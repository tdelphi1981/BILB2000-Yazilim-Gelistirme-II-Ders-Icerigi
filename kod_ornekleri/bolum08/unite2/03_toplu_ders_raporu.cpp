/**
 * 03_toplu_ders_raporu.cpp -- DersRaporu uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 * Konu 3 - Toplu Rapor: QTextDocument ile Cok Sayfali Belge
 *
 * QTextDocument'i dogrudan QTextCursor ile kurar. Farkli bolumler
 * arasinda sayfa sonu eklemek icin QTextBlockFormat::setPageBreak
 * kullanilir. Bu sayede QPrinter ile yazdirildiginda her bolum yeni
 * bir sayfada baslar.
 *
 * Derleme: Qt6::Gui yeterlidir.
 */

#include "03_toplu_ders_raporu.h"

#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextTable>
#include <QTextTableFormat>

bool DersRaporu::sayisalDegerAl(const NotTuru& n, double& out) {
    bool ok = false;
    std::visit([&ok, &out](const auto& deger) {
        using T = std::decay_t<decltype(deger)>;
        if constexpr (std::is_same_v<T, SayisalNot>) {
            out = deger.deger;
            ok  = true;
        } else if constexpr (std::is_same_v<T, HarfNotu>) {
            // Harf notlarini kaba bir haritayla sayisala cevir
            const QString h = deger.harf.trimmed().toUpper();
            if      (h == QStringLiteral("AA")) { out = 95.0; ok = true; }
            else if (h == QStringLiteral("BA")) { out = 87.0; ok = true; }
            else if (h == QStringLiteral("BB")) { out = 82.0; ok = true; }
            else if (h == QStringLiteral("CB")) { out = 77.0; ok = true; }
            else if (h == QStringLiteral("CC")) { out = 72.0; ok = true; }
            else if (h == QStringLiteral("DC")) { out = 67.0; ok = true; }
            else if (h == QStringLiteral("DD")) { out = 60.0; ok = true; }
            else                                 { out = 40.0; ok = true; }
        } else if constexpr (std::is_same_v<T, MuafNot>) {
            ok = false; // muaf: istatistige dahil degil
        }
    }, n);
    return ok;
}

QTextDocument* DersRaporu::olustur(
    const Ders&             ders,
    const QList<Ogrenci>&   ogrenciler,
    const QList<DersKaydi>& kayitlar) const
{
    auto* belge = new QTextDocument();
    belge->setDefaultFont(QFont(QStringLiteral("Sans Serif"), 10));

    QTextCursor imlec(belge);

    // ---------- Bolum 1: Baslik sayfasi ----------
    QTextCharFormat baslikFmt;
    baslikFmt.setFontWeight(QFont::Bold);
    baslikFmt.setFontPointSize(18);

    imlec.insertText(QStringLiteral("Ders Raporu\n"), baslikFmt);

    QTextCharFormat normalFmt;
    normalFmt.setFontPointSize(12);
    imlec.insertText(
        QStringLiteral("Ders Kodu: %1\nDers Adi: %2\nKredi: %3\n")
            .arg(ders.kod, ders.ad, QString::number(ders.kredi)),
        normalFmt);

    // ---------- Bolum 2: Istatistik sayfasi ----------
    QTextBlockFormat sayfaSonu;
    sayfaSonu.setPageBreakPolicy(
        QTextFormat::PageBreak_AlwaysBefore);
    imlec.insertBlock(sayfaSonu);

    imlec.insertText(QStringLiteral("Istatistikler\n"), baslikFmt);

    // Istatistikleri hesapla
    double toplam = 0.0;
    int    say    = 0;
    int    gecen  = 0;
    int    kalan  = 0;
    for (const auto& k : kayitlar) {
        if (k.dersId != ders.id) {
            continue;
        }
        double deger = 0.0;
        if (!sayisalDegerAl(k.notu, deger)) {
            continue;
        }
        toplam += deger;
        ++say;
        if (deger >= 60.0) { ++gecen; } else { ++kalan; }
    }
    const double ortalama = (say > 0) ? (toplam / say) : 0.0;

    imlec.insertText(
        QStringLiteral("Kayitli Ogrenci Sayisi: %1\n"
                       "Ortalama: %2\n"
                       "Gecen Ogrenci: %3\n"
                       "Kalan Ogrenci: %4\n")
            .arg(QString::number(say),
                 QString::number(ortalama, 'f', 2),
                 QString::number(gecen),
                 QString::number(kalan)),
        normalFmt);

    // ---------- Bolum 3: Detay sayfasi ----------
    imlec.insertBlock(sayfaSonu);
    imlec.insertText(QStringLiteral("Ogrenci Not Detaylari\n"),
                     baslikFmt);

    // Basligi takiben normal blok acalim
    QTextBlockFormat normalBlok;
    imlec.insertBlock(normalBlok);

    // Detay tablosu
    QTextTableFormat tabloFmt;
    tabloFmt.setBorder(1);
    tabloFmt.setCellPadding(4);
    tabloFmt.setCellSpacing(0);

    // Kaydi olan ogrencileri say
    int satirSayisi = 1; // baslik satiri
    for (const auto& k : kayitlar) {
        if (k.dersId == ders.id) { ++satirSayisi; }
    }

    QTextTable* tablo = imlec.insertTable(satirSayisi, 3, tabloFmt);
    tablo->cellAt(0, 0).firstCursorPosition().insertText(
        QStringLiteral("Numara"));
    tablo->cellAt(0, 1).firstCursorPosition().insertText(
        QStringLiteral("Ad Soyad"));
    tablo->cellAt(0, 2).firstCursorPosition().insertText(
        QStringLiteral("Not"));

    int satir = 1;
    for (const auto& k : kayitlar) {
        if (k.dersId != ders.id) {
            continue;
        }
        // Ogrenciyi bul
        QString numara  = QStringLiteral("?");
        QString adSoyad = QStringLiteral("?");
        for (const auto& o : ogrenciler) {
            if (o.id == k.ogrenciId) {
                numara  = o.numara;
                adSoyad = o.adSoyad;
                break;
            }
        }
        // Not metni
        QString notStr;
        std::visit([&notStr](const auto& d) {
            using T = std::decay_t<decltype(d)>;
            if constexpr (std::is_same_v<T, SayisalNot>) {
                notStr = QString::number(d.deger, 'f', 2);
            } else if constexpr (std::is_same_v<T, HarfNotu>) {
                notStr = d.harf;
            } else if constexpr (std::is_same_v<T, MuafNot>) {
                notStr = QStringLiteral("MUAF");
            }
        }, k.notu);

        tablo->cellAt(satir, 0).firstCursorPosition()
            .insertText(numara);
        tablo->cellAt(satir, 1).firstCursorPosition()
            .insertText(adSoyad);
        tablo->cellAt(satir, 2).firstCursorPosition()
            .insertText(notStr);
        ++satir;
    }

    return belge;
}

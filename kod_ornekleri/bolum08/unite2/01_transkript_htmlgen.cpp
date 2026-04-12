/**
 * 01_transkript_htmlgen.cpp -- TranskriptUretici uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 * Konu 1 - QTextDocument ile HTML Transkript Uretimi
 *
 * TranskriptUretici sinifinin uygulamasi. HTML sablonu icinde bir
 * tablo olusturur; her satir bir ders kaydini temsil eder. Ozetteki
 * GPA hesabi, yalnizca kredi \textgreater{} 0 olan ve sayisal/harf
 * notu bulunan derslerden yapilir.
 *
 * Derleme: Qt6::Core yeterlidir.
 */

#include "01_transkript_htmlgen.h"

#include <QStringBuilder>

double TranskriptUretici::gpaKatsayisi(const NotTuru& n) {
    double katsayi = 0.0;
    std::visit([&katsayi](const auto& deger) {
        using T = std::decay_t<decltype(deger)>;
        if constexpr (std::is_same_v<T, SayisalNot>) {
            // 100'luk -> 4'luk basit donusum
            const double s = deger.deger;
            if      (s >= 90.0) katsayi = 4.0;
            else if (s >= 85.0) katsayi = 3.5;
            else if (s >= 80.0) katsayi = 3.0;
            else if (s >= 75.0) katsayi = 2.5;
            else if (s >= 70.0) katsayi = 2.0;
            else if (s >= 60.0) katsayi = 1.5;
            else if (s >= 50.0) katsayi = 1.0;
            else                katsayi = 0.0;
        } else if constexpr (std::is_same_v<T, HarfNotu>) {
            const QString h = deger.harf.trimmed().toUpper();
            if      (h == QStringLiteral("AA")) katsayi = 4.0;
            else if (h == QStringLiteral("BA")) katsayi = 3.5;
            else if (h == QStringLiteral("BB")) katsayi = 3.0;
            else if (h == QStringLiteral("CB")) katsayi = 2.5;
            else if (h == QStringLiteral("CC")) katsayi = 2.0;
            else if (h == QStringLiteral("DC")) katsayi = 1.5;
            else if (h == QStringLiteral("DD")) katsayi = 1.0;
            else                                katsayi = 0.0;
        } else if constexpr (std::is_same_v<T, MuafNot>) {
            katsayi = 0.0; // GPA'ya katilmaz
        }
    }, n);
    return katsayi;
}

QString TranskriptUretici::notMetni(const NotTuru& n) {
    QString sonuc;
    std::visit([&sonuc](const auto& deger) {
        using T = std::decay_t<decltype(deger)>;
        if constexpr (std::is_same_v<T, SayisalNot>) {
            sonuc = QString::number(deger.deger, 'f', 2);
        } else if constexpr (std::is_same_v<T, HarfNotu>) {
            sonuc = deger.harf;
        } else if constexpr (std::is_same_v<T, MuafNot>) {
            sonuc = QStringLiteral("MUAF");
        }
    }, n);
    return sonuc;
}

QString TranskriptUretici::sablonSar(const QString& govde) {
    // Basit HTML sablonu. QTextDocument alt kume HTML'i destekler.
    return QStringLiteral(
        "<html><head><meta charset=\"utf-8\"/>"
        "<style>"
        "body  { font-family: sans-serif; }"
        "h1    { color: #1b4f72; }"
        "table { border-collapse: collapse; width: 100%; }"
        "th, td{ border: 1px solid #888; padding: 4px 8px; }"
        "th    { background-color: #d6eaf8; }"
        ".ozet { margin-top: 12px; font-weight: bold; }"
        "</style></head><body>")
        + govde
        + QStringLiteral("</body></html>");
}

QString TranskriptUretici::html(const Ogrenci&          ogrenci,
                                const QList<Ders>&      dersListesi,
                                const QList<DersKaydi>& kayitlar) const
{
    // 1) Baslik + ogrenci bilgileri
    QString govde;
    govde += QStringLiteral("<h1>Ogrenci Transkripti</h1>");
    govde += QStringLiteral("<p><b>Ogrenci No:</b> %1<br/>"
                            "<b>Ad Soyad:</b> %2</p>")
                 .arg(ogrenci.numara.toHtmlEscaped(),
                      ogrenci.adSoyad.toHtmlEscaped());

    // 2) Tablo baslari
    govde += QStringLiteral(
        "<table><tr>"
        "<th>Ders Kodu</th><th>Ders Adi</th>"
        "<th>Kredi</th><th>Not</th>"
        "</tr>");

    double toplamAgirlik    = 0.0;  // kredi * katsayi
    int    toplamKredi      = 0;

    // 3) Her kayit icin bir satir
    for (const auto& k : kayitlar) {
        if (k.ogrenciId != ogrenci.id) {
            continue;
        }
        // Dersi bul
        const Ders* dPtr = nullptr;
        for (const auto& d : dersListesi) {
            if (d.id == k.dersId) {
                dPtr = &d;
                break;
            }
        }
        if (dPtr == nullptr) {
            continue;
        }

        const QString notStr = notMetni(k.notu).toHtmlEscaped();
        govde += QStringLiteral("<tr><td>%1</td><td>%2</td>"
                                "<td>%3</td><td>%4</td></tr>")
                     .arg(dPtr->kod.toHtmlEscaped(),
                          dPtr->ad.toHtmlEscaped(),
                          QString::number(dPtr->kredi),
                          notStr);

        // Muaf notu GPA'ya katmiyoruz
        const bool muaf = std::holds_alternative<MuafNot>(k.notu);
        if (!muaf && dPtr->kredi > 0) {
            toplamAgirlik += gpaKatsayisi(k.notu) * dPtr->kredi;
            toplamKredi   += dPtr->kredi;
        }
    }

    govde += QStringLiteral("</table>");

    // 4) Ozet
    const double gpa = (toplamKredi > 0)
                           ? toplamAgirlik / toplamKredi
                           : 0.0;
    govde += QStringLiteral(
                 "<p class=\"ozet\">Toplam Kredi: %1 &nbsp; "
                 "GPA: %2</p>")
                 .arg(QString::number(toplamKredi),
                      QString::number(gpa, 'f', 2));

    return sablonSar(govde);
}

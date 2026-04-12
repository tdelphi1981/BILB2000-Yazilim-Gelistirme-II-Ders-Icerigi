#include "transkript_uretici.h"

double TranskriptUretici::gpaKatsayisi(
    const NotTuru& n) {
    double k = 0.0;
    std::visit([&k](const auto& d) {
        using T = std::decay_t<decltype(d)>;
        if constexpr (
            std::is_same_v<T, SayisalNot>) {
            double s = d.deger;
            if      (s >= 90) k = 4.0;
            else if (s >= 85) k = 3.5;
            else if (s >= 80) k = 3.0;
            else if (s >= 75) k = 2.5;
            else if (s >= 70) k = 2.0;
            else if (s >= 60) k = 1.5;
            else if (s >= 50) k = 1.0;
        } else if constexpr (
            std::is_same_v<T, HarfNotu>) {
            QString h = d.harf.toUpper();
            if      (h == "AA") k = 4.0;
            else if (h == "BA") k = 3.5;
            else if (h == "BB") k = 3.0;
            else if (h == "CB") k = 2.5;
            else if (h == "CC") k = 2.0;
            else if (h == "DC") k = 1.5;
            else if (h == "DD") k = 1.0;
        }
    }, n);
    return k;
}

QString TranskriptUretici::notMetni(
    const NotTuru& n) {
    QString s;
    std::visit([&s](const auto& d) {
        using T = std::decay_t<decltype(d)>;
        if constexpr (
            std::is_same_v<T, SayisalNot>)
            s = QString::number(d.deger, 'f', 2);
        else if constexpr (
            std::is_same_v<T, HarfNotu>)
            s = d.harf;
        else if constexpr (
            std::is_same_v<T, MuafNot>)
            s = "MUAF";
    }, n);
    return s;
}

QString TranskriptUretici::sablonSar(
    const QString& govde) {
    return QString(
        "<html><head>"
        "<meta charset=\"utf-8\"/>"
        "<style>"
        "body{font-family:sans-serif;}"
        "h1{color:#1b4f72;}"
        "table{border-collapse:collapse;"
        "width:100%;}"
        "th,td{border:1px solid #888;"
        "padding:4px 8px;}"
        "th{background:#d6eaf8;}"
        ".ozet{margin-top:12px;"
        "font-weight:bold;}"
        "</style></head><body>")
        + govde + "</body></html>";
}

QString TranskriptUretici::html(
    const Ogrenci& ogr,
    const QList<Ders>& dersler,
    const QList<DersKaydi>& kayitlar) const {
    QString g;
    g += "<h1>Öğrenci Transkripti</h1>";
    g += QString(
        "<p><b>No:</b> %1<br/>"
        "<b>Ad Soyad:</b> %2</p>")
        .arg(ogr.numara.toHtmlEscaped(),
             ogr.adSoyad.toHtmlEscaped());

    g += "<table><tr>"
         "<th>Kod</th><th>Ders</th>"
         "<th>Kredi</th><th>Not</th></tr>";

    double topAgirlik = 0;
    int    topKredi   = 0;

    for (const auto& k : kayitlar) {
        if (k.ogrenciId != ogr.id) continue;
        const Ders* dp = nullptr;
        for (const auto& d : dersler)
            if (d.id == k.dersId)
                { dp = &d; break; }
        if (!dp) continue;

        g += QString(
            "<tr><td>%1</td><td>%2</td>"
            "<td>%3</td><td>%4</td></tr>")
            .arg(dp->kod.toHtmlEscaped(),
                 dp->ad.toHtmlEscaped(),
                 QString::number(dp->kredi),
                 notMetni(k.notu)
                     .toHtmlEscaped());

        bool muaf =
            std::holds_alternative<MuafNot>(
                k.notu);
        if (!muaf && dp->kredi > 0) {
            topAgirlik +=
                gpaKatsayisi(k.notu)
                * dp->kredi;
            topKredi += dp->kredi;
        }
    }

    g += "</table>";
    double gpa = topKredi > 0
        ? topAgirlik / topKredi : 0.0;
    g += QString(
        "<p class=\"ozet\">"
        "Toplam Kredi: %1 &nbsp; GPA: %2</p>")
        .arg(QString::number(topKredi),
             QString::number(gpa, 'f', 2));

    return sablonSar(g);
}

/**
 * 02_trend_line_chart.cpp -- QLineSeries ile donem trendi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * Uc ogrencinin dort donem boyunca aldigi ortalama notlari
 * QLineSeries kullanarak cizgi grafikle gosterir. X ekseni
 * QDateTimeAxis (donem tarihleri), Y ekseni QValueAxis.
 *
 * Ogrenilecek kavramlar:
 * - QLineSeries::append ve point etiketleri
 * - setPointsVisible, setPointLabelsVisible kullanimi
 * - QDateTimeAxis ile zaman eksenli grafik
 * - Cok seriyi ayni grafikte gosterme (legend)
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs \
 *       Qt6Widgets Qt6Charts) 02_trend_line_chart.cpp -o ornek02
 */

#include <QApplication>
#include <QChart>
#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QMainWindow>
#include <QValueAxis>

#include <array>

namespace {

// Donem baslangic tarihleri (Guz/Bahar cift yil).
QList<QDateTime> donemTarihleri() {
    return {
        QDateTime(QDate(2023, 9,  15), QTime(0, 0)),
        QDateTime(QDate(2024, 2,  15), QTime(0, 0)),
        QDateTime(QDate(2024, 9,  15), QTime(0, 0)),
        QDateTime(QDate(2025, 2,  15), QTime(0, 0)),
    };
}

// Bir ogrenci icin seri olusturur ve 4 veri noktasi ekler.
QLineSeries* seriOlustur(const QString& isim,
                         const std::array<double, 4>& notlar,
                         const QList<QDateTime>& tarihler) {
    auto* seri = new QLineSeries();
    seri->setName(isim);
    for (int i = 0; i < 4; ++i) {
        // QDateTimeAxis ile kullanirken x degeri msecsSinceEpoch olmali
        seri->append(
            static_cast<double>(tarihler.at(i).toMSecsSinceEpoch()),
            notlar[i]);
    }
    seri->setPointsVisible(true);
    seri->setPointLabelsVisible(true);
    seri->setPointLabelsFormat(QStringLiteral("@yPoint"));
    return seri;
}

} // namespace

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    const auto tarihler = donemTarihleri();

    // Uc sahte ogrencinin donemlik ortalamalari
    auto* ayse = seriOlustur(
        QStringLiteral("Ayse Yilmaz"),
        {72.0, 78.5, 84.0, 88.5}, tarihler);
    auto* mehmet = seriOlustur(
        QStringLiteral("Mehmet Demir"),
        {65.0, 68.0, 63.5, 70.0}, tarihler);
    auto* zeynep = seriOlustur(
        QStringLiteral("Zeynep Kaya"),
        {90.0, 92.0, 85.0, 93.0}, tarihler);

    // Grafigi olustur ve seriler ekle
    auto* grafik = new QChart();
    grafik->addSeries(ayse);
    grafik->addSeries(mehmet);
    grafik->addSeries(zeynep);
    grafik->setTitle(QStringLiteral("Donemlik Ortalama Trendleri"));
    grafik->legend()->setVisible(true);
    grafik->legend()->setAlignment(Qt::AlignBottom);

    // Zaman ekseni
    auto* xEksen = new QDateTimeAxis();
    xEksen->setFormat(QStringLiteral("MMM yyyy"));
    xEksen->setTitleText(QStringLiteral("Donem"));
    xEksen->setTickCount(4);
    xEksen->setRange(tarihler.first(), tarihler.last());
    grafik->addAxis(xEksen, Qt::AlignBottom);
    ayse->attachAxis(xEksen);
    mehmet->attachAxis(xEksen);
    zeynep->attachAxis(xEksen);

    // Deger ekseni
    auto* yEksen = new QValueAxis();
    yEksen->setRange(0, 100);
    yEksen->setTickCount(11);
    yEksen->setTitleText(QStringLiteral("Ortalama"));
    grafik->addAxis(yEksen, Qt::AlignLeft);
    ayse->attachAxis(yEksen);
    mehmet->attachAxis(yEksen);
    zeynep->attachAxis(yEksen);

    auto* gorunum = new QChartView(grafik);
    gorunum->setRenderHint(QPainter::Antialiasing);

    QMainWindow pencere;
    pencere.setCentralWidget(gorunum);
    pencere.setWindowTitle(
        QStringLiteral("Ornek 02 - Trend Line Chart"));
    pencere.resize(760, 500);
    pencere.show();

    return uygulama.exec();
}

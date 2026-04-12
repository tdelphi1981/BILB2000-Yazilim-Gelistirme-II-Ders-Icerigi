/**
 * 01_qchart_notdagilimi.cpp -- QChart ile harf notu dagilimi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * Bu tek dosyalik ornek, 20 ogrenciye ait sahte sayisal notlardan
 * AA/BA/BB/CB/CC/DC/DD/FF harf notu dagilimini hesaplar ve
 * QBarSeries ile bir cubuk grafik (bar chart) olarak gosterir.
 *
 * Ogrenilecek kavramlar:
 * - QApplication olmadan QWidget tabanli Qt uygulamasi calismaz
 * - QChart, QChartView, QBarSeries, QBarSet temelleri
 * - Modern Qt 6 Charts API: addAxis + attachAxis (eski setAxisX degil)
 * - QBarCategoryAxis ile kategorik X ekseni, QValueAxis ile sayisal Y
 *
 * Derleme (CMake):
 *   find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)
 *   target_link_libraries(app PRIVATE Qt6::Widgets Qt6::Charts)
 *
 * Derleme (dogrudan):
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs \
 *       Qt6Widgets Qt6Charts) 01_qchart_notdagilimi.cpp -o ornek01
 */

#include <QApplication>
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QMainWindow>
#include <QStringList>
#include <QValueAxis>
#include <QVector>

#include <array>

// Sahte 20 ogrenci notu uretir (tekrarlanabilir olmasi icin sabit).
static QVector<double> ornekNotlariUret() {
    return {
        92, 88, 75, 63, 49, 85, 71, 58, 96, 44,
        67, 79, 82, 55, 38, 91, 73, 60, 48, 86
    };
}

// Sayisal notu KTU harf notu sistemine donusturur.
static int harfIndeksi(double puan) {
    if (puan >= 90) return 0; // AA
    if (puan >= 85) return 1; // BA
    if (puan >= 80) return 2; // BB
    if (puan >= 75) return 3; // CB
    if (puan >= 65) return 4; // CC
    if (puan >= 58) return 5; // DC
    if (puan >= 50) return 6; // DD
    return 7;                 // FF
}

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // Harf notu kategorileri (X ekseni etiketleri)
    const QStringList harfler{
        QStringLiteral("AA"), QStringLiteral("BA"),
        QStringLiteral("BB"), QStringLiteral("CB"),
        QStringLiteral("CC"), QStringLiteral("DC"),
        QStringLiteral("DD"), QStringLiteral("FF")
    };

    // Dagilim histogramini hesapla
    std::array<int, 8> sayilar{};
    for (double puan : ornekNotlariUret()) {
        ++sayilar[static_cast<std::size_t>(harfIndeksi(puan))];
    }

    // QBarSet -- tek bir veri kumesi (seri icinde bir "kategori grubu")
    auto* set = new QBarSet(QStringLiteral("Ogrenci Sayisi"));
    for (int deger : sayilar) {
        *set << deger;
    }

    // QBarSeries -- bar (cubuk) serisi
    auto* seri = new QBarSeries();
    seri->append(set);
    seri->setLabelsVisible(true); // Cubuk uzerinde deger etiketi

    // QChart -- serileri ve eksenleri barindiran grafigin kendisi
    auto* grafik = new QChart();
    grafik->addSeries(seri);
    grafik->setTitle(QStringLiteral("BILB2000 Harf Notu Dagilimi"));
    grafik->setAnimationOptions(QChart::SeriesAnimations);
    grafik->legend()->setVisible(true);
    grafik->legend()->setAlignment(Qt::AlignBottom);

    // Modern API: addAxis + attachAxis
    auto* xEksen = new QBarCategoryAxis();
    xEksen->append(harfler);
    grafik->addAxis(xEksen, Qt::AlignBottom);
    seri->attachAxis(xEksen);

    auto* yEksen = new QValueAxis();
    yEksen->setRange(0, 6);
    yEksen->setTickCount(7);
    yEksen->setTitleText(QStringLiteral("Ogrenci Sayisi"));
    grafik->addAxis(yEksen, Qt::AlignLeft);
    seri->attachAxis(yEksen);

    // QChartView -- grafigi cizen ozel QGraphicsView
    auto* gorunum = new QChartView(grafik);
    gorunum->setRenderHint(QPainter::Antialiasing);

    QMainWindow pencere;
    pencere.setCentralWidget(gorunum);
    pencere.setWindowTitle(QStringLiteral("Ornek 01 - Harf Notu Dagilimi"));
    pencere.resize(720, 480);
    pencere.show();

    return uygulama.exec();
}

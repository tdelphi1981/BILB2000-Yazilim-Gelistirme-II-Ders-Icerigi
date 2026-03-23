/**
 * ss02_standarditem_tableview.cpp
 *
 * QStandardItemModel + QTableView ornegi.
 * Ogrenci bilgilerini tablo halinde gosteren basit bir pencere.
 *
 * Derleme:
 *   g++ -std=c++20 $(pkg-config --cflags --libs Qt6Widgets) -fPIC \
 *       -o ss02 ss02_standarditem_tableview.cpp
 */

#include <QApplication>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Model olustur: 5 satir, 5 sutun
    auto *model = new QStandardItemModel(5, 5);

    // Basliklar
    model->setHorizontalHeaderLabels(
        {"No", "Ad Soyad", "Bölüm", "Sınıf", "GPA"});

    // Ogrenci verileri
    struct OgrenciVeri {
        QString no, ad, bolum, sinif, gpa;
    };

    QVector<OgrenciVeri> ogrenciler = {
        {"1", "Ali Yılmaz",     "Bilgisayar Bilimleri",  "3", "3.45"},
        {"2", "Ayşe Kaya",      "Yazılım Mühendisliği",  "2", "3.78"},
        {"3", "Mehmet Demir",   "Elektrik-Elektronik",   "4", "2.90"},
        {"4", "Fatma Şahin",    "Bilgisayar Bilimleri",  "1", "1.85"},
        {"5", "Mustafa Çelik",  "Makine Mühendisliği",   "3", "3.52"},
    };

    for (int i = 0; i < ogrenciler.size(); ++i) {
        const auto &ogr = ogrenciler[i];
        model->setItem(i, 0, new QStandardItem(ogr.no));
        model->setItem(i, 1, new QStandardItem(ogr.ad));
        model->setItem(i, 2, new QStandardItem(ogr.bolum));
        model->setItem(i, 3, new QStandardItem(ogr.sinif));
        model->setItem(i, 4, new QStandardItem(ogr.gpa));
    }

    // Tablo gorunumu
    QTableView tableView;
    tableView.setWindowTitle("QStandardItemModel Örneği");
    tableView.setModel(model);
    tableView.setAlternatingRowColors(true);
    tableView.horizontalHeader()->setStretchLastSection(true);
    tableView.setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView.verticalHeader()->setVisible(false);

    // Sutun genislikleri
    tableView.setColumnWidth(0, 50);
    tableView.setColumnWidth(1, 160);
    tableView.setColumnWidth(2, 200);
    tableView.setColumnWidth(3, 60);

    tableView.resize(800, 400);
    tableView.show();

    return app.exec();
}

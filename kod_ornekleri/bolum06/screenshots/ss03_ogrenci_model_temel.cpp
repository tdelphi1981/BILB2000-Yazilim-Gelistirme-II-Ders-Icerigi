/**
 * ss03_ogrenci_model_temel.cpp
 *
 * QAbstractTableModel turevli basit OgrenciModel ornegi.
 * rowCount, columnCount, data, headerData implementasyonu gosterir.
 *
 * Derleme:
 *   moc ss03_ogrenci_model_temel.cpp -o ss03_ogrenci_model_temel.moc
 *   g++ -std=c++20 $(pkg-config --cflags --libs Qt6Widgets) -fPIC \
 *       -o ss03 ss03_ogrenci_model_temel.cpp
 */

#include <QApplication>
#include <QTableView>
#include <QAbstractTableModel>
#include <QHeaderView>
#include <QString>
#include <QVector>

struct Ogrenci {
    QString numara;
    QString ad;
    QString bolum;
    int sinif;
    double gpa;
};

class OgrenciModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit OgrenciModel(QVector<Ogrenci> veri, QObject *parent = nullptr)
        : QAbstractTableModel(parent), ogrenciler_(std::move(veri))
    {}

    int rowCount(const QModelIndex &parent = {}) const override {
        if (parent.isValid()) return 0;
        return static_cast<int>(ogrenciler_.size());
    }

    int columnCount(const QModelIndex &parent = {}) const override {
        if (parent.isValid()) return 0;
        return 5; // Numara, Ad Soyad, Bolum, Sinif, GPA
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return {};

        const auto &ogr = ogrenciler_.at(index.row());
        switch (index.column()) {
            case 0: return ogr.numara;
            case 1: return ogr.ad;
            case 2: return ogr.bolum;
            case 3: return ogr.sinif;
            case 4: return QString::number(ogr.gpa, 'f', 2);
            default: return {};
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole)
            return {};

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0: return "Numara";
                case 1: return "Ad Soyad";
                case 2: return "Bölüm";
                case 3: return "Sınıf";
                case 4: return "GPA";
                default: return {};
            }
        }
        // Dikey baslik: satir numarasi
        return section + 1;
    }

private:
    QVector<Ogrenci> ogrenciler_;
};

#include "ss03_ogrenci_model_temel.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QVector<Ogrenci> veriler = {
        {"2024001", "Ali Yılmaz",       "Bilgisayar Bilimleri",  3, 3.45},
        {"2024002", "Ayşe Kaya",        "Yazılım Mühendisliği",  2, 3.78},
        {"2024003", "Mehmet Demir",     "Elektrik-Elektronik",   4, 2.90},
        {"2024004", "Fatma Şahin",      "Bilgisayar Bilimleri",  1, 1.85},
        {"2024005", "Mustafa Çelik",    "Makine Mühendisliği",   3, 3.52},
        {"2024006", "Zeynep Öztürk",    "İnşaat Mühendisliği",   2, 2.15},
    };

    auto *model = new OgrenciModel(veriler);

    QTableView tableView;
    tableView.setWindowTitle("OgrenciModel - Temel Tablo");
    tableView.setModel(model);
    tableView.setAlternatingRowColors(true);
    tableView.horizontalHeader()->setStretchLastSection(true);
    tableView.setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableView.setColumnWidth(0, 100);
    tableView.setColumnWidth(1, 160);
    tableView.setColumnWidth(2, 200);
    tableView.setColumnWidth(3, 60);

    tableView.resize(800, 450);
    tableView.show();

    return app.exec();
}

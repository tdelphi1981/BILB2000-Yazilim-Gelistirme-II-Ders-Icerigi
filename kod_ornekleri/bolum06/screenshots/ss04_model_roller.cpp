/**
 * ss04_model_roller.cpp
 *
 * Model rolleri ornegi: DisplayRole, TextAlignmentRole, BackgroundRole,
 * FontRole ve ToolTipRole kullanarak kosullu bicimlendirme gosterir.
 * ss03'un genisletilmis halidir.
 *
 * Derleme:
 *   moc ss04_model_roller.cpp -o ss04_model_roller.moc
 *   g++ -std=c++20 $(pkg-config --cflags --libs Qt6Widgets) -fPIC \
 *       -o ss04 ss04_model_roller.cpp
 */

#include <QApplication>
#include <QTableView>
#include <QAbstractTableModel>
#include <QHeaderView>
#include <QString>
#include <QVector>
#include <QColor>
#include <QFont>
#include <QBrush>

struct Ogrenci {
    QString numara;
    QString ad;
    QString bolum;
    int sinif;
    double gpa;
};

class OgrenciModelRoller : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit OgrenciModelRoller(QVector<Ogrenci> veri, QObject *parent = nullptr)
        : QAbstractTableModel(parent), ogrenciler_(std::move(veri))
    {}

    int rowCount(const QModelIndex &parent = {}) const override {
        if (parent.isValid()) return 0;
        return static_cast<int>(ogrenciler_.size());
    }

    int columnCount(const QModelIndex &parent = {}) const override {
        if (parent.isValid()) return 0;
        return 5;
    }

    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid())
            return {};

        const auto &ogr = ogrenciler_.at(index.row());
        int col = index.column();

        switch (role) {
        case Qt::DisplayRole:
            switch (col) {
                case 0: return ogr.numara;
                case 1: return ogr.ad;
                case 2: return ogr.bolum;
                case 3: return ogr.sinif;
                case 4: return QString::number(ogr.gpa, 'f', 2);
                default: return {};
            }

        case Qt::TextAlignmentRole:
            // Numara ve Sinif ortala, GPA saga yasla
            if (col == 0 || col == 3)
                return QVariant(Qt::AlignCenter);
            if (col == 4)
                return QVariant(Qt::AlignRight | Qt::AlignVCenter);
            return {};

        case Qt::BackgroundRole:
            // GPA sutununda kosullu arka plan rengi
            if (col == 4) {
                if (ogr.gpa < 2.0)
                    return QBrush(QColor("#FFE0E0")); // Acik kirmizi
                if (ogr.gpa >= 3.5)
                    return QBrush(QColor("#E0FFE0")); // Acik yesil
            }
            return {};

        case Qt::FontRole:
            // GPA >= 3.5 ise kalin yazi
            if (col == 4 && ogr.gpa >= 3.5) {
                QFont font;
                font.setBold(true);
                return font;
            }
            return {};

        case Qt::ToolTipRole:
            // GPA hucresinde tooltip
            if (col == 4)
                return QString("Not ortalaması: %1").arg(ogr.gpa, 0, 'f', 2);
            return {};

        default:
            return {};
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
        return section + 1;
    }

private:
    QVector<Ogrenci> ogrenciler_;
};

#include "ss04_model_roller.moc"

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

    auto *model = new OgrenciModelRoller(veriler);

    QTableView tableView;
    tableView.setWindowTitle("Model Rolleri - Koşullu Biçimlendirme");
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

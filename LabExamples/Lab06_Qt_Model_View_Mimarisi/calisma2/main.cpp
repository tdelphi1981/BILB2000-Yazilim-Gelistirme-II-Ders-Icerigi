/**
 * Lab 06 - Calisma 2 Baslangic Kodu
 * QAbstractTableModel ile Ozel Model
 *
 * Bu dosya Lab 06 foyu ile birlikte kullanilir.
 * "GOREV" isaretli bolgeleri tamamlayin.
 *
 * Ogrenilecek kavramlar:
 * - QAbstractTableModel alt siniflamasi
 * - rowCount(), columnCount(), data() gecersiz kilma
 * - headerData() ile sutun basliklari
 * - beginInsertRows() / endInsertRows() ile satir ekleme
 * - Qt::DisplayRole ve Qt::TextAlignmentRole
 * - Qt::BackgroundRole ile hucre renklendirme
 * - beginRemoveRows() / endRemoveRows() ile satir silme
 * - Q_OBJECT makrosu ve main.moc kullanimi
 *
 * Lab: 06 - Qt Model/View Mimarisi
 * Calisma: 2 - QAbstractTableModel ile Ozel Model (30 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Calistirma: ./app
 */

#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QAbstractTableModel>
#include <QVBoxLayout>
#include <QLabel>
#include <QStatusBar>
#include <vector>
#include <QString>

struct Ogrenci {
    int numara;
    QString ad;
    QString bolum;
    double gpa;
};

class OgrenciModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit OgrenciModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent) {}

    void ogrenciEkle(const Ogrenci& o) {
        beginInsertRows(QModelIndex(),
            m_liste.size(), m_liste.size());
        m_liste.push_back(o);
        endInsertRows();
    }

    int rowCount(const QModelIndex& = {})
        const override {
        return static_cast<int>(m_liste.size());
    }

    int columnCount(const QModelIndex& = {})
        const override {
        return 4;
    }

    QVariant data(const QModelIndex& idx,
        int role = Qt::DisplayRole)
        const override {
        if (!idx.isValid()) return {};
        const auto& o = m_liste[idx.row()];

        if (role == Qt::DisplayRole) {
            switch (idx.column()) {
                case 0: return o.numara;
                case 1: return o.ad;
                case 2: return o.bolum;
                case 3: return QString::number(
                             o.gpa, 'f', 2);
            }
        }

        if (role == Qt::TextAlignmentRole) {
            if (idx.column() == 0 || idx.column() == 3)
                return int(Qt::AlignCenter);
        }

        // === GOREV 2.1 - Arka Plan Rengi ile GPA Vurgulama (10 dk) ===
        // TODO: a) Asagidaki blogu tamamlayin:
        //          GPA 2.0'in altindaysa hucre arka planini acik kirmizi
        //          (QColor(255, 200, 200)) yapin.
        // TODO: b) GPA 3.5'in ustundeyse acik yesil
        //          (QColor(200, 255, 200)) yapin.
        // TODO: c) Gerekli #include <QColor> basligini eklemeyi unutmayin.
        //
        // if (role == Qt::BackgroundRole) {
        //     double gpa = m_liste[idx.row()].gpa;
        //     if (gpa < 2.0)
        //         return QColor(255, 200, 200);
        //     if (gpa > 3.5)
        //         return QColor(200, 255, 200);
        // }
        // ============================================

        return {};
    }

    QVariant headerData(int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole)
        const override {
        if (role != Qt::DisplayRole) return {};
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0: return "No";
                case 1: return "Ad Soyad";
                case 2: return QString::fromUtf8("Bölüm");
                case 3: return "GPA";
            }
        }
        return section + 1;
    }

    // === GOREV 2.2 - Satir Silme ve Secim Bilgisi (20 dk) ===
    // TODO: a) Asagidaki ogrenciSil() metodunu sinifa ekleyin:
    //          void ogrenciSil(int satir) {
    //              beginRemoveRows(QModelIndex(), satir, satir);
    //              m_liste.erase(m_liste.begin() + satir);
    //              endRemoveRows();
    //          }
    //
    // TODO: b) Ana pencereye bir arac cubugu ekleyin ve "Sil"
    //          aksiyonu olusturun. Tiklandiginda secili satiri silin.
    //
    // TODO: c) Satir secildiginde durum cubugunda secilen ogrencinin
    //          adini ve GPA'sini gosterin. selectionModel() uzerinden
    //          currentRowChanged sinyalini kullanin.
    // ============================================

private:
    std::vector<Ogrenci> m_liste;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow anaPencere;
    anaPencere.setWindowTitle(QString::fromUtf8(
        "OBYS - Öğrenci Tablosu"));
    anaPencere.resize(650, 400);

    auto* model = new OgrenciModel(&anaPencere);
    model->ogrenciEkle({101, "Ali Yilmaz",
        "Bilgisayar", 3.45});
    model->ogrenciEkle({102, "Ayse Kaya",
        "Yazilim", 3.80});
    model->ogrenciEkle({103, "Veli Ozturk",
        "Elektrik", 2.95});
    model->ogrenciEkle({104, "Fatma Demir",
        "Bilgisayar", 1.85});

    auto* tablo = new QTableView;
    tablo->setModel(model);
    tablo->horizontalHeader()
        ->setStretchLastSection(true);
    tablo->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    anaPencere.setCentralWidget(tablo);
    anaPencere.statusBar()->showMessage(
        QString::fromUtf8("%1 öğrenci kayitli")
        .arg(model->rowCount()));

    anaPencere.show();
    return app.exec();
}

#include "main.moc"

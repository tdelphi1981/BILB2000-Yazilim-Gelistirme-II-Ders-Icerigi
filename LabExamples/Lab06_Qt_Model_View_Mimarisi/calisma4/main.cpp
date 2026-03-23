/**
 * Lab 06 - Calisma 4 Baslangic Kodu
 * QSortFilterProxyModel ile Siralama ve Filtreleme
 *
 * Bu dosya Lab 06 foyu ile birlikte kullanilir.
 * "GOREV" isaretli bolgeleri tamamlayin.
 *
 * Ogrenilecek kavramlar:
 * - QSortFilterProxyModel ile proxy model kullanimi
 * - setSourceModel() ile kaynak model baglama
 * - setFilterFixedString() ile metin filtreleme
 * - setFilterKeyColumn() ile sutun bazli filtreleme
 * - setSortingEnabled() ile tablo siralama
 * - filterAcceptsRow() gecersiz kilma ile ozel filtreleme
 * - invalidateFilter() ile filtre yenileme
 * - layoutChanged sinyali ile sayac guncelleme
 *
 * Lab: 06 - Qt Model/View Mimarisi
 * Calisma: 4 - QSortFilterProxyModel (30 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Calistirma: ./app
 */

#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QStatusBar>
#include <vector>
#include <QString>

// --- Ogrenci veri yapisi ---
struct Ogrenci {
    int numara;
    QString ad;
    QString bolum;
    double gpa;
};

// --- Ozel tablo modeli ---
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

private:
    std::vector<Ogrenci> m_liste;
};

// === GOREV 4.1 - GPA Aralik Filtresi (10 dk) ===
// TODO: a) Filtre arayuzune iki QDoubleSpinBox ekleyin:
//          "Min GPA" ve "Max GPA" (0.0-4.0 araligi).
//          #include <QDoubleSpinBox> eklemeyi unutmayin.
//
// TODO: b) QSortFilterProxyModel'i alt sinifylayarak
//          filterAcceptsRow() fonksiyonunu gecersiz kilin:
//
// class OgrenciProxy : public QSortFilterProxyModel {
//     Q_OBJECT
// public:
//     using QSortFilterProxyModel::QSortFilterProxyModel;
//
//     void setGpaAraligi(double min, double max) {
//         m_minGpa = min;
//         m_maxGpa = max;
//         invalidateFilter();
//     }
//
// protected:
//     bool filterAcceptsRow(int satir,
//         const QModelIndex& parent)
//         const override {
//         // GPA kontrolu (sutun 3)
//         auto idx = sourceModel()->index(
//             satir, 3, parent);
//         double gpa =
//             idx.data().toString().toDouble();
//         if (gpa < m_minGpa || gpa > m_maxGpa)
//             return false;
//         // Metin filtresini de uygula
//         return QSortFilterProxyModel
//             ::filterAcceptsRow(satir, parent);
//     }
//
// private:
//     double m_minGpa = 0.0;
//     double m_maxGpa = 4.0;
// };
//
// TODO: c) SpinBox degerleri degistiginde
//          proxy->setGpaAraligi(min, max) cagirin.
// ============================================

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow anaPencere;
    anaPencere.setWindowTitle(QString::fromUtf8(
        "OBYS - Öğrenci Filtresi"));
    anaPencere.resize(700, 450);

    // Model olustur ve veri ekle
    auto* model = new OgrenciModel(&anaPencere);
    model->ogrenciEkle({101, "Ali Yilmaz",
        "Bilgisayar", 3.45});
    model->ogrenciEkle({102, "Ayse Kaya",
        "Yazilim", 3.80});
    model->ogrenciEkle({103, "Veli Ozturk",
        "Elektrik", 2.95});
    model->ogrenciEkle({104, "Fatma Demir",
        "Bilgisayar", 1.85});
    model->ogrenciEkle({105, "Mehmet Can",
        "Yazilim", 3.20});
    model->ogrenciEkle({106, "Zeynep Arslan",
        "Elektrik", 3.65});

    // Proxy model olustur
    auto* proxy = new QSortFilterProxyModel(
        &anaPencere);
    proxy->setSourceModel(model);
    proxy->setFilterCaseSensitivity(
        Qt::CaseInsensitive);

    // Tablo proxy modele baglanir
    auto* tablo = new QTableView;
    tablo->setModel(proxy);
    tablo->setSortingEnabled(true);
    tablo->horizontalHeader()
        ->setStretchLastSection(true);
    tablo->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    // Filtre arayuzu
    auto* merkez = new QWidget;
    auto* anaDuzen = new QVBoxLayout(merkez);

    auto* filtreDuzen = new QHBoxLayout;
    auto* filtreAlani = new QLineEdit;
    filtreAlani->setPlaceholderText(
        "Filtre metni...");
    auto* sutunSecici = new QComboBox;
    sutunSecici->addItems(
        {QString::fromUtf8("Tümü"), "No", "Ad",
         QString::fromUtf8("Bölüm"), "GPA"});
    filtreDuzen->addWidget(
        new QLabel("Filtre:"));
    filtreDuzen->addWidget(filtreAlani);
    filtreDuzen->addWidget(sutunSecici);
    anaDuzen->addLayout(filtreDuzen);

    anaDuzen->addWidget(tablo);
    anaPencere.setCentralWidget(merkez);

    // Filtre metni degistiginde
    QObject::connect(filtreAlani,
        &QLineEdit::textChanged,
        [proxy](const QString& metin) {
            proxy->setFilterFixedString(metin);
        });

    // Sutun secici degistiginde
    QObject::connect(sutunSecici,
        &QComboBox::currentIndexChanged,
        [proxy](int idx) {
            // 0=Tumu(-1), 1=No(0), 2=Ad(1), ...
            proxy->setFilterKeyColumn(idx - 1);
        });

    anaPencere.statusBar()->showMessage(
        QString::fromUtf8("%1 öğrenci kayitli")
        .arg(model->rowCount()));

    // === GOREV 4.2 - Filtrelenmis Sayac ve Siralama (10 dk) ===
    // TODO: a) Proxy modelin rowCount() degerini durum cubugunda
    //          gosterin: "Gosterilen: X / Y ogrenci". Bunun icin
    //          proxy'nin layoutChanged sinyalini kullanin:
    //
    //          auto guncelle = [proxy, model, &anaPencere]() {
    //              anaPencere.statusBar()->showMessage(
    //                  QString::fromUtf8(
    //                      "Gösterilen: %1 / %2 öğrenci")
    //                  .arg(proxy->rowCount())
    //                  .arg(model->rowCount()));
    //          };
    //          QObject::connect(proxy,
    //              &QSortFilterProxyModel::layoutChanged,
    //              guncelle);
    //
    // TODO: b) "Temizle" butonu ekleyin: filtre metnini, sutun
    //          secicisini ve GPA araligini sifirlasin.
    //
    // TODO: c) Tablo basligina tiklayarak siralamanin calistigini
    //          dogrulayin. GPA sutununa tiklandiginda sayisal siralama
    //          yapildigini kontrol edin.
    // ============================================

    // === GOREV 4.3 - Bolum Filtre Butonlari (10 dk) ===
    // TODO: a) Filtre arayuzunun altina yatay bir QHBoxLayout ekleyin.
    //
    // TODO: b) Dort adet QPushButton olusturun: "Tumu",
    //          "Bilgisayar", "Yazilim", "Elektrik".
    //
    // TODO: c) Her butona tiklandiginda setFilterKeyColumn(2) ile
    //          bolum sutununu secin ve setFilterFixedString() ile
    //          ilgili bolumu filtreleyin. "Tumu" butonu filtreyi
    //          temizlesin.
    //
    //          Ornek:
    //          auto* bolumDuzen = new QHBoxLayout;
    //          auto* tumuButon = new QPushButton("Tumu");
    //          auto* bilgButon = new QPushButton("Bilgisayar");
    //          auto* yazButon = new QPushButton("Yazilim");
    //          auto* elkButon = new QPushButton("Elektrik");
    //          bolumDuzen->addWidget(tumuButon);
    //          bolumDuzen->addWidget(bilgButon);
    //          bolumDuzen->addWidget(yazButon);
    //          bolumDuzen->addWidget(elkButon);
    //          anaDuzen->addLayout(bolumDuzen);
    //
    //          QObject::connect(bilgButon, &QPushButton::clicked,
    //              [proxy, filtreAlani]() {
    //                  proxy->setFilterKeyColumn(2);
    //                  filtreAlani->setText("Bilgisayar");
    //              });
    //          // Diger butonlar icin de benzer baglanti kurun
    //          QObject::connect(tumuButon, &QPushButton::clicked,
    //              [proxy, filtreAlani, sutunSecici]() {
    //                  filtreAlani->clear();
    //                  sutunSecici->setCurrentIndex(0);
    //              });
    // ============================================

    anaPencere.show();
    return app.exec();
}

#include "main.moc"

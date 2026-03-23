/**
 * Lab 06 - Calisma 3 Baslangic Kodu
 * Delegeler (QStyledItemDelegate)
 *
 * Bu dosya Lab 06 foyu ile birlikte kullanilir.
 * "GOREV" isaretli bolgeleri tamamlayin.
 *
 * Ogrenilecek kavramlar:
 * - QStyledItemDelegate ile ozel delege olusturma
 * - createEditor(), setEditorData(), setModelData()
 * - QDoubleSpinBox ile GPA duzenleme
 * - QComboBox ile bolum secimi delegesi
 * - paint() ile ozel hucre cizimi
 * - flags() ve setData() ile duzenleme destegi
 * - dataChanged sinyali ile durum guncelleme
 *
 * Lab: 06 - Qt Model/View Mimarisi
 * Calisma: 3 - Delegeler (30 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Calistirma: ./app
 */

#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QStatusBar>
#include <QMessageBox>
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

    // Duzenleme destegi: flags()
    Qt::ItemFlags flags(const QModelIndex& idx)
        const override {
        if (!idx.isValid())
            return Qt::NoItemFlags;
        return Qt::ItemIsSelectable
             | Qt::ItemIsEnabled
             | Qt::ItemIsEditable;
    }

    // Duzenleme destegi: setData()
    bool setData(const QModelIndex& idx,
        const QVariant& deger, int role) override {
        if (!idx.isValid()
            || role != Qt::EditRole) return false;

        auto& o = m_liste[idx.row()];
        switch (idx.column()) {
            case 0: o.numara = deger.toInt(); break;
            case 1: o.ad = deger.toString(); break;
            case 2: o.bolum = deger.toString(); break;
            case 3: o.gpa = deger.toDouble(); break;
            default: return false;
        }
        emit dataChanged(idx, idx, {role});
        return true;
    }

private:
    std::vector<Ogrenci> m_liste;
};

// --- GPA Delegesi: QDoubleSpinBox ile duzenleme ---
class GpaDelegate
    : public QStyledItemDelegate {
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QWidget* createEditor(QWidget* parent,
        const QStyleOptionViewItem&,
        const QModelIndex&) const override {
        auto* editor = new QDoubleSpinBox(parent);
        editor->setRange(0.0, 4.0);
        editor->setSingleStep(0.01);
        editor->setDecimals(2);
        return editor;
    }

    void setEditorData(QWidget* editor,
        const QModelIndex& idx) const override {
        double deger = idx.data().toString().toDouble();
        auto* spin =
            static_cast<QDoubleSpinBox*>(editor);
        spin->setValue(deger);
    }

    void setModelData(QWidget* editor,
        QAbstractItemModel* model,
        const QModelIndex& idx) const override {
        auto* spin =
            static_cast<QDoubleSpinBox*>(editor);
        model->setData(idx, spin->value());
    }
};

// === GOREV 3.1 - Bolum Delegesi (10 dk) ===
// TODO: a) BolumDelegate sinifini QStyledItemDelegate'den turetin.
//
// TODO: b) createEditor() icinde bir QComboBox olusturun
//          ve su secenekleri ekleyin: "Bilgisayar", "Yazilim",
//          "Elektrik", "Makine".
//          #include <QComboBox> eklemeyi unutmayin.
//
// TODO: c) setEditorData() ve setModelData() fonksiyonlarini
//          yazin. QComboBox icin setCurrentText() ve
//          currentText() kullanin.
//
// TODO: d) Delegeyi main() icinde sutun 2'ye (Bolum) atayin:
//          auto* bolumDelege = new BolumDelegate(tablo);
//          tablo->setItemDelegateForColumn(2, bolumDelege);
//
// Ornek iskelet:
// class BolumDelegate : public QStyledItemDelegate {
//     Q_OBJECT
// public:
//     using QStyledItemDelegate::QStyledItemDelegate;
//
//     QWidget* createEditor(QWidget* parent,
//         const QStyleOptionViewItem&,
//         const QModelIndex&) const override {
//         auto* combo = new QComboBox(parent);
//         combo->addItems({"Bilgisayar", "Yazilim",
//                          "Elektrik", "Makine"});
//         return combo;
//     }
//
//     void setEditorData(QWidget* editor,
//         const QModelIndex& idx) const override {
//         auto* combo = static_cast<QComboBox*>(editor);
//         combo->setCurrentText(idx.data().toString());
//     }
//
//     void setModelData(QWidget* editor,
//         QAbstractItemModel* model,
//         const QModelIndex& idx) const override {
//         auto* combo = static_cast<QComboBox*>(editor);
//         model->setData(idx, combo->currentText());
//     }
// };
// ============================================

// === GOREV 3.2 - GPA Dogrulama ve Renklendirme (10 dk) ===
// TODO: a) GpaDelegate::setModelData() icinde GPA degeri
//          0.0-4.0 araliginda degilse modeli guncellemeyin ve
//          QMessageBox::warning() ile uyari gosterin.
//
// TODO: b) paint() fonksiyonunu gecersiz kilarak GPA degerine
//          gore hucre arka planini boyayin:
//          kirmizi (< 2.0), sari (2.0-3.0), yesil (> 3.0)
//
//          #include <QPainter> eklemeyi unutmayin.
//
//          void paint(QPainter* painter,
//              const QStyleOptionViewItem& option,
//              const QModelIndex& idx) const override {
//              double gpa = idx.data().toString().toDouble();
//              QColor renk;
//              if (gpa < 2.0) renk = QColor(255, 180, 180);
//              else if (gpa < 3.0) renk = QColor(255, 255, 180);
//              else renk = QColor(180, 255, 180);
//
//              painter->fillRect(option.rect, renk);
//              QStyledItemDelegate::paint(
//                  painter, option, idx);
//          }
// ============================================

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow anaPencere;
    anaPencere.setWindowTitle(QString::fromUtf8(
        "OBYS - Öğrenci Tablosu (Delegeler)"));
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

    // GPA delegesini sutun 3'e ata
    auto* gpaDelege = new GpaDelegate(tablo);
    tablo->setItemDelegateForColumn(3, gpaDelege);

    anaPencere.setCentralWidget(tablo);
    anaPencere.statusBar()->showMessage(
        QString::fromUtf8("%1 öğrenci kayitli")
        .arg(model->rowCount()));

    // === GOREV 3.3 - dataChanged Sinyali ile Durum Guncelleme (10 dk) ===
    // TODO: a) Modelin dataChanged sinyalini bir lambda'ya baglayin.
    //
    // TODO: b) GPA sutunu degistiginde durum cubugunda yeni degeri gosterin:
    //          "GPA guncellendi: [ad] -> [yeni_gpa]"
    //
    // TODO: c) Ortalama GPA'yi hesaplayip durum cubugunda gosterin:
    //          "Sinif ortalamasi: X.XX"
    //
    //          QObject::connect(model,
    //              &OgrenciModel::dataChanged,
    //              [model, &anaPencere](
    //                  const QModelIndex& topLeft) {
    //                  if (topLeft.column() == 3) {
    //                      QString ad = model->data(
    //                          model->index(topLeft.row(), 1))
    //                          .toString();
    //                      QString gpa = topLeft.data().toString();
    //                      anaPencere.statusBar()->showMessage(
    //                          QString("GPA guncellendi: %1 -> %2")
    //                          .arg(ad).arg(gpa));
    //                  }
    //              });
    // ============================================

    anaPencere.show();
    return app.exec();
}

#include "main.moc"

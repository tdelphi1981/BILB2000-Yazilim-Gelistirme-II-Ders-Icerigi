/**
 * QUndoStack ile Geri Al/Yinele (Undo/Redo)
 *
 * Bu program QUndoStack ve QUndoCommand ile öğrenci ekleme/silme
 * işlemlerini geri alınabilir şekilde uygulamayı gösterir.
 *
 * Öğrenilecek kavramlar:
 * - QUndoCommand ile komut deseni
 * - QUndoStack ile komut yığını yönetimi
 * - createUndoAction/createRedoAction
 * - Değişiklik takibi (cleanChanged)
 *
 * Bölüm: 07 - Qt İleri Bileşenler
 * Ünite: 4 - Olay İşleme ve Zamanlayıcılar
 *
 * Derleme: CMake ile (Qt6 gerekli)
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QUndoStack>
#include <QUndoCommand>

// Ogrenci ekleme komutu
class EkleKomutu : public QUndoCommand {
public:
    EkleKomutu(QStandardItemModel* model,
               const QStringList& veriler)
        : m_model(model), m_veriler(veriler) {
        setText(QString("Öğrenci ekle: %1").arg(veriler.at(1)));
    }

    void redo() override {
        int satir = m_model->rowCount();
        m_model->insertRow(satir);
        for (int i = 0; i < m_veriler.size(); ++i)
            m_model->setItem(satir, i,
                new QStandardItem(m_veriler.at(i)));
    }

    void undo() override {
        m_model->removeRow(m_model->rowCount() - 1);
    }

private:
    QStandardItemModel* m_model;
    QStringList m_veriler;
};

// Ogrenci silme komutu
class SilKomutu : public QUndoCommand {
public:
    SilKomutu(QStandardItemModel* model, int satir)
        : m_model(model), m_satir(satir) {
        // Silinecek verileri sakla
        for (int i = 0; i < model->columnCount(); ++i) {
            auto* item = model->item(satir, i);
            m_veriler << (item ? item->text() : "");
        }
        setText(QString("Öğrenci sil: %1").arg(m_veriler.at(1)));
    }

    void redo() override {
        m_model->removeRow(m_satir);
    }

    void undo() override {
        m_model->insertRow(m_satir);
        for (int i = 0; i < m_veriler.size(); ++i)
            m_model->setItem(m_satir, i,
                new QStandardItem(m_veriler.at(i)));
    }

private:
    QStandardItemModel* m_model;
    int m_satir;
    QStringList m_veriler;
};

class UndoRedoOBYS : public QMainWindow {
    Q_OBJECT
public:
    explicit UndoRedoOBYS(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - Undo/Redo Örneği");
        resize(800, 500);

        m_undoStack = new QUndoStack(this);

        tabloOlustur();
        menuOlustur();
        aracCubuguOlustur();

        // Degisiklik takibi
        connect(m_undoStack, &QUndoStack::cleanChanged, this,
            [this](bool temiz) {
                QString baslik = "OBYS - Undo/Redo Örneği";
                if (!temiz) baslik += " *";
                setWindowTitle(baslik);
            });

        statusBar()->showMessage("Hazır");
    }

private:
    QStandardItemModel* m_model = nullptr;
    QUndoStack* m_undoStack = nullptr;
    QTableView* m_tablo = nullptr;

    void tabloOlustur() {
        auto* merkez = new QWidget(this);
        auto* duzen = new QVBoxLayout(merkez);

        m_model = new QStandardItemModel(0, 4, this);
        m_model->setHorizontalHeaderLabels(
            {"Numara", "Ad Soyad", "Bölüm", "GPA"});

        m_tablo = new QTableView(merkez);
        m_tablo->setModel(m_model);
        m_tablo->setAlternatingRowColors(true);
        m_tablo->horizontalHeader()->setStretchLastSection(true);
        m_tablo->setSelectionBehavior(QAbstractItemView::SelectRows);
        duzen->addWidget(m_tablo);

        // Ekleme formu
        auto* formDuzen = new QHBoxLayout;
        auto* noEdit = new QLineEdit;
        noEdit->setPlaceholderText("Numara");
        auto* adEdit = new QLineEdit;
        adEdit->setPlaceholderText("Ad Soyad");
        auto* ekleButon = new QPushButton("Ekle");
        auto* silButon = new QPushButton("Seçili Sil");

        formDuzen->addWidget(noEdit);
        formDuzen->addWidget(adEdit);
        formDuzen->addWidget(ekleButon);
        formDuzen->addWidget(silButon);
        duzen->addLayout(formDuzen);

        setCentralWidget(merkez);

        // Baglantılar
        static int sayac = 1;
        connect(ekleButon, &QPushButton::clicked, this,
            [this, noEdit, adEdit]() {
                QString no = noEdit->text().trimmed();
                QString ad = adEdit->text().trimmed();
                if (no.isEmpty()) no = QString("2024%1").arg(sayac++, 3, 10, QChar('0'));
                if (ad.isEmpty()) ad = "Yeni Öğrenci";

                m_undoStack->push(new EkleKomutu(m_model,
                    {no, ad, "Bilg. Bil.", "3.00"}));
                noEdit->clear();
                adEdit->clear();
                statusBar()->showMessage("Öğrenci eklendi", 2000);
            });

        connect(silButon, &QPushButton::clicked, this, [this]() {
            auto idx = m_tablo->currentIndex();
            if (!idx.isValid()) return;
            m_undoStack->push(new SilKomutu(m_model, idx.row()));
            statusBar()->showMessage("Öğrenci silindi", 2000);
        });
    }

    void menuOlustur() {
        auto* duzenleMenu = menuBar()->addMenu("Düzenle");

        auto* geriAlAct = m_undoStack->createUndoAction(this, "Geri Al");
        geriAlAct->setShortcut(QKeySequence::Undo);

        auto* yineleAct = m_undoStack->createRedoAction(this, "Yinele");
        yineleAct->setShortcut(QKeySequence::Redo);

        duzenleMenu->addAction(geriAlAct);
        duzenleMenu->addAction(yineleAct);
    }

    void aracCubuguOlustur() {
        auto* aracCubugu = addToolBar("Düzenle");
        aracCubugu->addAction(m_undoStack->createUndoAction(this));
        aracCubugu->addAction(m_undoStack->createRedoAction(this));
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    UndoRedoOBYS pencere;
    pencere.show();
    return app.exec();
}

#include "06_undo_redo_tam.moc"

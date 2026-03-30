/**
 * Lab 07 - Çalışma 4: QTimer ve QUndoStack
 *
 * TODO: Bu başlangıç kodunu tamamlayın.
 * 1. Otomatik kaydetme timer'ı ekleyin
 * 2. Durum çubuğu saati ekleyin
 * 3. Ekleme/silme için QUndoCommand yazın
 * 4. Düzenle menüsüne Geri Al/Yinele ekleyin
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
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QUndoStack>
#include <QUndoCommand>

// TODO: EkleKomutu sınıfı yazın
// - QUndoCommand'dan türetin
// - redo(): model'e satır ekle
// - undo(): eklenen satırı sil

// TODO: SilKomutu sınıfı yazın
// - redo(): satırı sil (verileri kaydet!)
// - undo(): silinen satırı geri ekle

class UndoOBYS : public QMainWindow {
    Q_OBJECT
public:
    explicit UndoOBYS(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - Lab 07 Çalışma 4");
        resize(800, 500);

        // TODO: m_undoStack oluşturun

        tabloOlustur();
        menuOlustur();

        // TODO: otomatikKaydetmeKur();
        // TODO: saatKur();

        statusBar()->showMessage("Hazır");
    }

private:
    QStandardItemModel* m_model = nullptr;
    QTableView* m_tablo = nullptr;
    // TODO: QUndoStack* m_undoStack = nullptr;

    void tabloOlustur() {
        auto* merkez = new QWidget(this);
        auto* duzen = new QVBoxLayout(merkez);

        m_model = new QStandardItemModel(0, 3, this);
        m_model->setHorizontalHeaderLabels({"Numara", "Ad", "GPA"});

        m_tablo = new QTableView(merkez);
        m_tablo->setModel(m_model);
        m_tablo->setAlternatingRowColors(true);
        m_tablo->horizontalHeader()->setStretchLastSection(true);
        m_tablo->setSelectionBehavior(QAbstractItemView::SelectRows);
        duzen->addWidget(m_tablo);

        // Ekleme formu
        auto* form = new QHBoxLayout;
        auto* noEdit = new QLineEdit;
        noEdit->setPlaceholderText("Numara");
        auto* adEdit = new QLineEdit;
        adEdit->setPlaceholderText("Ad Soyad");
        auto* ekleBtn = new QPushButton("Ekle");
        auto* silBtn = new QPushButton("Seçili Sil");

        form->addWidget(noEdit);
        form->addWidget(adEdit);
        form->addWidget(ekleBtn);
        form->addWidget(silBtn);
        duzen->addLayout(form);

        setCentralWidget(merkez);

        // TODO: Ekle butonuna undo komutlu bağlantı
        // TODO: Sil butonuna undo komutlu bağlantı
    }

    void menuOlustur() {
        auto* duzenleMenu = menuBar()->addMenu("Düzenle");

        // TODO: createUndoAction ve createRedoAction ekleyin
        // TODO: Kısayol tuşları atayın (Ctrl+Z, Ctrl+Y)
    }

    // TODO: otomatikKaydetmeKur() - 60 sn periyotlu timer
    // TODO: saatKur() - durum çubuğu saati
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    UndoOBYS pencere;
    pencere.show();
    return app.exec();
}

#include "main.moc"

/**
 * Qt Stil Sistemi ve Tema Değiştirme
 *
 * Bu program QSS ile aydınlık/karanlık tema oluşturma
 * ve çalışma zamanında tema değiştirme özelliğini gösterir.
 *
 * Öğrenilecek kavramlar:
 * - QSS (Qt Style Sheets) söz dizimi
 * - Karanlık ve aydınlık tema tanımları
 * - Çalışma zamanında tema değiştirme
 * - QActionGroup ile radyo menü öğeleri
 *
 * Bölüm: 07 - Qt İleri Bileşenler
 * Ünite: 2 - Qt Stil Sistemi
 *
 * Derleme: CMake ile (Qt6 gerekli)
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QLabel>

// Aydinlik tema QSS
static const char* AYDINLIK_QSS = R"(
QMainWindow {
    background-color: #f5f5f5;
}
QMenuBar {
    background-color: #ffffff;
    border-bottom: 1px solid #e0e0e0;
}
QMenuBar::item:selected {
    background-color: #e8f5e9;
}
QToolBar {
    background-color: #fafafa;
    border-bottom: 1px solid #e0e0e0;
    spacing: 4px;
}
QTableView {
    background-color: #ffffff;
    gridline-color: #e0e0e0;
    selection-background-color: #bbdefb;
    alternate-background-color: #fafafa;
}
QHeaderView::section {
    background-color: #e8e8e8;
    padding: 4px;
    border: 1px solid #d0d0d0;
    font-weight: bold;
}
QStatusBar {
    background-color: #f0f0f0;
    border-top: 1px solid #e0e0e0;
}
)";

// Karanlik tema QSS
static const char* KARANLIK_QSS = R"(
QMainWindow {
    background-color: #2d2d2d;
    color: #e0e0e0;
}
QMenuBar {
    background-color: #333333;
    color: #e0e0e0;
    border-bottom: 1px solid #444444;
}
QMenuBar::item:selected {
    background-color: #505050;
}
QMenu {
    background-color: #383838;
    color: #e0e0e0;
    border: 1px solid #505050;
}
QMenu::item:selected {
    background-color: #505050;
}
QToolBar {
    background-color: #333333;
    border-bottom: 1px solid #444444;
    spacing: 4px;
}
QTableView {
    background-color: #383838;
    color: #e0e0e0;
    gridline-color: #505050;
    selection-background-color: #1565c0;
    alternate-background-color: #3d3d3d;
}
QHeaderView::section {
    background-color: #404040;
    color: #e0e0e0;
    padding: 4px;
    border: 1px solid #505050;
    font-weight: bold;
}
QStatusBar {
    background-color: #333333;
    color: #e0e0e0;
    border-top: 1px solid #444444;
}
QLabel {
    color: #e0e0e0;
}
)";

class TemaliBYS : public QMainWindow {
    Q_OBJECT
public:
    explicit TemaliBYS(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - Tema Örneği");
        resize(800, 500);

        tabloOlustur();
        menuOlustur();
        statusBar()->showMessage("Tema: Aydınlık");
    }

private:
    void tabloOlustur() {
        auto* model = new QStandardItemModel(5, 4, this);
        model->setHorizontalHeaderLabels(
            {"Numara", "Ad Soyad", "Bölüm", "GPA"});

        // Ornek veriler
        QStringList numaralar = {
            "2024001", "2024002", "2024003", "2024004", "2024005"};
        QStringList adlar = {
            "Ali Yılmaz", "Ayşe Kaya", "Mehmet Demir",
            "Fatma Şahin", "Emre Çelik"};
        QStringList bolumler = {
            "Bilg. Bil.", "Yaz. Müh.", "Bilg. Bil.",
            "Yaz. Müh.", "Bilg. Bil."};
        QStringList gpalar = {"3.45", "3.82", "2.95", "3.67", "3.12"};

        for (int i = 0; i < 5; ++i) {
            model->setItem(i, 0, new QStandardItem(numaralar[i]));
            model->setItem(i, 1, new QStandardItem(adlar[i]));
            model->setItem(i, 2, new QStandardItem(bolumler[i]));
            model->setItem(i, 3, new QStandardItem(gpalar[i]));
        }

        auto* tablo = new QTableView(this);
        tablo->setModel(model);
        tablo->setAlternatingRowColors(true);
        tablo->horizontalHeader()->setStretchLastSection(true);
        setCentralWidget(tablo);
    }

    void menuOlustur() {
        auto* gorunumMenu = menuBar()->addMenu("Görünüm");
        auto* temaMenu = gorunumMenu->addMenu("Tema");
        auto* temaGrubu = new QActionGroup(this);

        auto* aydinlikAct = new QAction("Aydınlık", this);
        aydinlikAct->setCheckable(true);
        aydinlikAct->setChecked(true);

        auto* karanlikAct = new QAction("Karanlık", this);
        karanlikAct->setCheckable(true);

        temaGrubu->addAction(aydinlikAct);
        temaGrubu->addAction(karanlikAct);
        temaMenu->addAction(aydinlikAct);
        temaMenu->addAction(karanlikAct);

        connect(aydinlikAct, &QAction::triggered, this, [this]() {
            qApp->setStyleSheet(AYDINLIK_QSS);
            statusBar()->showMessage("Tema: Aydınlık");
        });

        connect(karanlikAct, &QAction::triggered, this, [this]() {
            qApp->setStyleSheet(KARANLIK_QSS);
            statusBar()->showMessage("Tema: Karanlık");
        });
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet(AYDINLIK_QSS);

    TemaliBYS pencere;
    pencere.show();
    return app.exec();
}

#include "02_tema_degistirme.moc"

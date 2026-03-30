/**
 * Lab 07 - Çalışma 2: QSS ile Tema Oluşturma
 *
 * TODO: Bu başlangıç kodunu tamamlayın.
 * 1. QSS dosyalarını oluşturun (stiller/ dizini)
 * 2. Tema yükleme fonksiyonu yazın
 * 3. Tema menüsü ekleyin
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QStatusBar>
#include <QFile>

class TemaliBYS : public QMainWindow {
    Q_OBJECT
public:
    explicit TemaliBYS(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - Lab 07 Çalışma 2");
        resize(800, 500);

        tabloOlustur();
        // TODO: temaMenusuOlustur();

        statusBar()->showMessage("Tema: Aydınlık");
    }

private:
    void tabloOlustur() {
        auto* model = new QStandardItemModel(3, 3, this);
        model->setHorizontalHeaderLabels({"Numara", "Ad", "GPA"});
        model->setItem(0, 0, new QStandardItem("2024001"));
        model->setItem(0, 1, new QStandardItem("Ali Yılmaz"));
        model->setItem(0, 2, new QStandardItem("3.45"));
        model->setItem(1, 0, new QStandardItem("2024002"));
        model->setItem(1, 1, new QStandardItem("Ayşe Kaya"));
        model->setItem(1, 2, new QStandardItem("3.82"));
        model->setItem(2, 0, new QStandardItem("2024003"));
        model->setItem(2, 1, new QStandardItem("Mehmet Demir"));
        model->setItem(2, 2, new QStandardItem("2.95"));

        auto* tablo = new QTableView(this);
        tablo->setModel(model);
        tablo->setAlternatingRowColors(true);
        tablo->horizontalHeader()->setStretchLastSection(true);
        setCentralWidget(tablo);
    }

    // TODO: temaUygula() fonksiyonu yazın
    // - QFile ile .qss dosyasını okuyun
    // - qApp->setStyleSheet() ile uygulayın

    // TODO: temaMenusuOlustur() fonksiyonu yazın
    // - Görünüm > Tema alt menüsü
    // - QActionGroup ile Aydınlık/Karanlık seçenekleri
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    TemaliBYS pencere;
    pencere.show();
    return app.exec();
}

#include "main.moc"

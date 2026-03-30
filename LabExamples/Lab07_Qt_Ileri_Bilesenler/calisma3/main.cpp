/**
 * Lab 07 - Çalışma 3: QSettings ve JSON Kayıt/Yükleme
 *
 * TODO: Bu başlangıç kodunu tamamlayın.
 * 1. QSettings ile pencere durumunu kaydet/yükle
 * 2. JSON dosyaya kaydetme fonksiyonu yazın
 * 3. JSON dosyadan yükleme fonksiyonu yazın
 * 4. Son Açılanlar menüsü ekleyin
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QSettings>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QCloseEvent>

class JsonOBYS : public QMainWindow {
    Q_OBJECT
public:
    explicit JsonOBYS(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - Lab 07 Çalışma 3");
        resize(800, 500);

        tabloOlustur();
        menuOlustur();

        // TODO: ayarlariYukle();

        statusBar()->showMessage("Hazır");
    }

protected:
    // TODO: closeEvent override edin
    // - Pencere geometrisini QSettings ile kaydedin
    // - Tema tercihini kaydedin

private:
    QStandardItemModel* m_model = nullptr;
    QString m_mevcutDosya;

    void tabloOlustur() {
        m_model = new QStandardItemModel(0, 3, this);
        m_model->setHorizontalHeaderLabels({"Numara", "Ad", "GPA"});

        // Örnek veriler
        ogrenciEkle("2024001", "Ali Yılmaz", "3.45");
        ogrenciEkle("2024002", "Ayşe Kaya", "3.82");

        auto* tablo = new QTableView(this);
        tablo->setModel(m_model);
        tablo->setAlternatingRowColors(true);
        tablo->horizontalHeader()->setStretchLastSection(true);
        setCentralWidget(tablo);
    }

    void ogrenciEkle(const QString& no, const QString& ad,
                     const QString& gpa) {
        int r = m_model->rowCount();
        m_model->insertRow(r);
        m_model->setItem(r, 0, new QStandardItem(no));
        m_model->setItem(r, 1, new QStandardItem(ad));
        m_model->setItem(r, 2, new QStandardItem(gpa));
    }

    void menuOlustur() {
        auto* dosyaMenu = menuBar()->addMenu("Dosya");

        // TODO: Kaydet action'ı ekleyin
        // TODO: Aç action'ı ekleyin
        // TODO: Son Açılanlar alt menüsü ekleyin
        // TODO: Çıkış action'ı ekleyin

        auto* cikisAct = dosyaMenu->addAction("Çıkış");
        connect(cikisAct, &QAction::triggered,
                this, &QWidget::close);
    }

    // TODO: dosyaKaydet() - JSON olarak kaydet
    // TODO: dosyaAc() - JSON dosyadan yükle
    // TODO: sonDosyaEkle() - Son açılanlar listesini güncelle
    // TODO: ayarlariYukle() - QSettings ile geri yükle
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("KTU");
    app.setApplicationName("Lab07");

    JsonOBYS pencere;
    pencere.show();
    return app.exec();
}

#include "main.moc"

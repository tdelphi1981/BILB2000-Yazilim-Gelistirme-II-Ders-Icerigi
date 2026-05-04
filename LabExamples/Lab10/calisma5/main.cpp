/**
 * Lab 10 - Çalışma 5: Qt Responsive UI
 *
 * QtConcurrent::mapped + QFutureWatcher + QProgressDialog entegrasyonu.
 * TODO yorumlarını doldurun.
 *
 * Derleme:
 *   mkdir build && cd build
 *   cmake ..
 *   make
 *   ./lab10_calisma5
 */

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QProgressDialog>
#include <QFutureWatcher>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <QList>

// Ağır işlem simülasyonu
int agirIslem(int x) {
    QThread::msleep(100);
    return x * x;
}

class AnaPencere : public QMainWindow {
    Q_OBJECT
public:
    AnaPencere() {
        auto* merkez = new QWidget;
        auto* yerlesim = new QVBoxLayout(merkez);

        basEtiketi_ = new QLabel("Hazır", this);
        yerlesim->addWidget(basEtiketi_);

        baslat_ = new QPushButton("Paralel Hesaplama Başlat", this);
        yerlesim->addWidget(baslat_);

        setCentralWidget(merkez);
        setWindowTitle("Lab 10 - Çalışma 5");
        resize(400, 200);

        connect(baslat_, &QPushButton::clicked,
                this, &AnaPencere::hesaplamaBaslat);
    }

private slots:
    void hesaplamaBaslat() {
        // 50 elemanlı test verisi
        QList<int> veriler;
        for (int i = 0; i < 50; ++i) veriler.append(i + 1);

        // TODO (Gorev 5.2): QtConcurrent::mapped + progress dialog
        //
        // 1. QProgressDialog olustur:
        //    progressDialog_ = new QProgressDialog(
        //        "Hesaplaniyor...", "Iptal", 0, 100, this);
        //    progressDialog_->setWindowModality(Qt::WindowModal);
        //
        // 2. QFutureWatcher<int>* olustur:
        //    watcher_ = new QFutureWatcher<int>(this);
        //
        // 3. Sinyalleri bagla:
        //    - progressRangeChanged  -> progressDialog_::setRange
        //    - progressValueChanged  -> progressDialog_::setValue
        //    - finished              -> this::hesaplamaBitti
        //    - progressDialog_::canceled -> watcher_::cancel
        //
        // 4. QtConcurrent::mapped ile baslat:
        //    QFuture<int> f = QtConcurrent::mapped(veriler, agirIslem);
        //    watcher_->setFuture(f);

        basEtiketi_->setText("Başlatılıyor...");
    }

    void hesaplamaBitti() {
        // TODO: QFutureWatcher.results() ile sonucları al
        basEtiketi_->setText("Tamamlandı");
    }

private:
    QLabel* basEtiketi_;
    QPushButton* baslat_;
    QProgressDialog* progressDialog_ = nullptr;
    QFutureWatcher<int>* watcher_ = nullptr;
};

#include "main.moc"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    AnaPencere pencere;
    pencere.show();
    return app.exec();
}

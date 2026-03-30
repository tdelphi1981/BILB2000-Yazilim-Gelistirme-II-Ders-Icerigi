/**
 * QSettings ile Uygulama Ayarları
 *
 * Bu program QSettings ile pencere boyutu, konum ve
 * kullanıcı tercihlerini kalıcı olarak kaydetme/yükleme
 * işlemlerini gösterir.
 *
 * Öğrenilecek kavramlar:
 * - QSettings ile ayar yazma/okuma
 * - Gruplar ile ayar düzenleme
 * - Pencere geometrisini kaydetme/yükleme
 * - closeEvent override etme
 *
 * Bölüm: 07 - Qt İleri Bileşenler
 * Ünite: 3 - Ayarlar ve Veri Kalıcılığı
 *
 * Derleme: CMake ile (Qt6 gerekli)
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QSettings>
#include <QLabel>
#include <QCloseEvent>
#include <QMessageBox>

class AyarliPencere : public QMainWindow {
    Q_OBJECT
public:
    explicit AyarliPencere(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - Ayarlar Örneği");

        auto* bilgi = new QLabel(this);
        bilgi->setAlignment(Qt::AlignCenter);
        bilgi->setWordWrap(true);
        setCentralWidget(bilgi);

        // Ayarlari yukle
        ayarlariYukle();

        // Bilgi goster
        QSettings ayarlar;
        int acilisSayisi = ayarlar.value(
            "istatistik/acilisSayisi", 0).toInt() + 1;
        ayarlar.setValue("istatistik/acilisSayisi", acilisSayisi);

        bilgi->setText(QString(
            "Bu uygulama %1. kez açıldı.\n\n"
            "Pencere boyutunu ve konumunu değiştirip kapatın.\n"
            "Yeniden açtığınızda aynı durumda başlayacak.\n\n"
            "Ayar dosyası: %2")
            .arg(acilisSayisi)
            .arg(ayarlar.fileName()));

        statusBar()->showMessage(
            QString("Açılış #%1").arg(acilisSayisi));
    }

protected:
    void closeEvent(QCloseEvent* event) override {
        QSettings ayarlar;
        ayarlar.beginGroup("pencere");
        ayarlar.setValue("geometri", saveGeometry());
        ayarlar.setValue("durum", saveState());
        ayarlar.endGroup();

        statusBar()->showMessage("Ayarlar kaydedildi");
        QMainWindow::closeEvent(event);
    }

private:
    void ayarlariYukle() {
        QSettings ayarlar;
        ayarlar.beginGroup("pencere");
        QByteArray geo = ayarlar.value("geometri").toByteArray();
        if (!geo.isEmpty()) {
            restoreGeometry(geo);
        } else {
            resize(800, 500);
        }
        QByteArray durum = ayarlar.value("durum").toByteArray();
        if (!durum.isEmpty())
            restoreState(durum);
        ayarlar.endGroup();
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("KTU");
    app.setApplicationName("OBYS_Ayarlar_Ornek");

    AyarliPencere pencere;
    pencere.show();
    return app.exec();
}

#include "03_qsettings_kullanimi.moc"

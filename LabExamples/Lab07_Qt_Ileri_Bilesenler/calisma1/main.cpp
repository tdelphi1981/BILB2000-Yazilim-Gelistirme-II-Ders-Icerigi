/**
 * Lab 07 - Çalışma 1: Qt Kaynak Sistemi ve İkonlar
 *
 * TODO: Bu başlangıç kodunu tamamlayın.
 * 1. resources.qrc dosyasını oluşturun
 * 2. İkonlu action'lar ekleyin
 * 3. Araç çubuğu oluşturun
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QStatusBar>
#include <QLabel>

class AnaPencere : public QMainWindow {
    Q_OBJECT
public:
    explicit AnaPencere(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - Lab 07 Çalışma 1");
        resize(800, 500);

        auto* merkez = new QLabel("Kaynak Sistemi Örneği", this);
        merkez->setAlignment(Qt::AlignCenter);
        setCentralWidget(merkez);

        // TODO: actionlariOlustur();
        // TODO: menuleriOlustur();
        // TODO: aracCubuguOlustur();

        statusBar()->showMessage("Hazır");
    }

private:
    // TODO: Action'ları oluşturun
    // - yeniAction: ":/icons/icons/yeni.png", Ctrl+N
    // - kaydetAction: ":/icons/icons/kaydet.png", Ctrl+S
    // - silAction: ":/icons/icons/sil.png", Delete
    // - cikisAction: Ctrl+Q, close() bağlantısı

    // TODO: Menüleri oluşturun
    // - Dosya: Yeni, Kaydet, Çıkış
    // - Düzenle: Sil

    // TODO: Araç çubuğunu oluşturun
    // - İkon boyutu: 24x24
    // - Stil: ToolButtonTextUnderIcon
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    AnaPencere pencere;
    pencere.show();
    return app.exec();
}

#include "main.moc"

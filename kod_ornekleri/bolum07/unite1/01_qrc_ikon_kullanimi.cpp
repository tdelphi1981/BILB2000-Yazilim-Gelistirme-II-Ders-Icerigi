/**
 * Qt Kaynak Sistemi ve İkon Kullanımı
 *
 * Bu program .qrc kaynak sistemi ile ikon yükleme ve
 * araç çubuğu/menü entegrasyonunu gösterir.
 *
 * Öğrenilecek kavramlar:
 * - .qrc kaynak dosyası oluşturma
 * - QIcon ile ikon yükleme
 * - QPixmap ile görsel gösterme
 * - Araç çubuğu ve menüye ikon ekleme
 *
 * Bölüm: 07 - Qt İleri Bileşenler
 * Ünite: 1 - Qt Kaynak Sistemi
 *
 * Derleme: CMake ile (Qt6 gerekli)
 *   mkdir build && cd build && cmake .. && cmake --build .
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QStatusBar>
#include <QMessageBox>

class AnaPencere : public QMainWindow {
    Q_OBJECT
public:
    explicit AnaPencere(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - İkon Örneği");
        resize(800, 500);

        actionlariOlustur();
        menuleriOlustur();
        aracCubuguOlustur();

        // Merkezi widget
        auto* merkezLabel = new QLabel("OBYS Ana Ekranı", this);
        merkezLabel->setAlignment(Qt::AlignCenter);
        merkezLabel->setStyleSheet("font-size: 20px; color: #008080;");
        setCentralWidget(merkezLabel);

        statusBar()->showMessage("Hazır", 3000);
    }

private:
    void actionlariOlustur() {
        // Dosya menüsü action'lari
        // Not: Gercek projede ":/icons/yeni.png" seklinde
        // kaynak sisteminden yuklenir.
        // Burada standart ikonlar kullaniyoruz.
        yeniAction = new QAction(
            style()->standardIcon(QStyle::SP_FileIcon),
            "Yeni", this);
        yeniAction->setShortcut(QKeySequence::New);
        yeniAction->setStatusTip("Yeni öğrenci kaydı oluştur");

        acAction = new QAction(
            style()->standardIcon(QStyle::SP_DialogOpenButton),
            "Aç...", this);
        acAction->setShortcut(QKeySequence::Open);
        acAction->setStatusTip("JSON dosyasından yükle");

        kaydetAction = new QAction(
            style()->standardIcon(QStyle::SP_DialogSaveButton),
            "Kaydet", this);
        kaydetAction->setShortcut(QKeySequence::Save);
        kaydetAction->setStatusTip("Mevcut dosyaya kaydet");

        cikisAction = new QAction(
            style()->standardIcon(QStyle::SP_DialogCloseButton),
            "Çıkış", this);
        cikisAction->setShortcut(QKeySequence::Quit);

        connect(cikisAction, &QAction::triggered,
                this, &QWidget::close);

        connect(yeniAction, &QAction::triggered, this, [this]() {
            statusBar()->showMessage("Yeni kayıt oluşturuldu", 2000);
        });
    }

    void menuleriOlustur() {
        auto* dosyaMenu = menuBar()->addMenu("Dosya");
        dosyaMenu->addAction(yeniAction);
        dosyaMenu->addAction(acAction);
        dosyaMenu->addAction(kaydetAction);
        dosyaMenu->addSeparator();
        dosyaMenu->addAction(cikisAction);
    }

    void aracCubuguOlustur() {
        auto* aracCubugu = addToolBar("Ana Araç Çubuğu");
        aracCubugu->setIconSize(QSize(24, 24));
        aracCubugu->setToolButtonStyle(
            Qt::ToolButtonTextUnderIcon);

        aracCubugu->addAction(yeniAction);
        aracCubugu->addAction(acAction);
        aracCubugu->addAction(kaydetAction);
    }

    QAction* yeniAction = nullptr;
    QAction* acAction = nullptr;
    QAction* kaydetAction = nullptr;
    QAction* cikisAction = nullptr;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    AnaPencere pencere;
    pencere.show();
    return app.exec();
}

#include "01_qrc_ikon_kullanimi.moc"

/*
BEKLENEN CIKTI:
---------------
Ikonlu arac cubugu ve menu ogeleri ile
Qt ana penceresi acilir.
*/

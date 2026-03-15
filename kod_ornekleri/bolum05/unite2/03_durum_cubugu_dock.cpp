/**
 * Durum Çubuğu ve Dock Widget - QStatusBar ve QDockWidget
 *
 * Bu program, QMainWindow'un durum çubuğu ve kenetlenebilir panel
 * (dock widget) özelliklerini gösterir. Durum çubuğunda kalıcı ve
 * geçici mesajlar, dock widget'ta ise gezinme paneli (QListWidget)
 * bulunmaktadır.
 *
 * Öğrenilecek kavramlar:
 * - QStatusBar ile kalıcı ve geçici mesajlar
 * - addPermanentWidget() ile kalıcı widget
 * - showMessage() ile geçici mesaj
 * - QDockWidget ile kenetlenebilir panel
 * - setAllowedAreas() ile izin verilen alanlar
 * - setFeatures() ile dock widget özellikleri
 *
 * Bölüm: 05 - Qt Pencere ve Düzen Yönetimi
 * Ünite: 2 - Ana Pencere Tasarımı
 *
 * Derleme (CMake önerilir, CMakeLists.txt dosyasına bakınız):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 *
 * Doğrudan derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 03_durum_cubugu_dock 03_durum_cubugu_dock.cpp
 */

#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QDockWidget>
#include <QListWidget>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) ANA PENCERE
    // =============================================
    QMainWindow anaPencere;
    anaPencere.setWindowTitle("OBYS - Durum Çubuğu ve Dock Widget");
    anaPencere.resize(800, 600);

    // Merkezi widget
    QTextEdit* metin = new QTextEdit();
    metin->setPlaceholderText("Ana çalışma alanı...\n"
                              "Soldaki panelden bir öğe seçin.");
    anaPencere.setCentralWidget(metin);

    // =============================================
    // 2) DURUM ÇUBUĞU
    // =============================================
    QStatusBar* durumCubugu = anaPencere.statusBar();

    // Geçici mesaj (5 saniye)
    durumCubugu->showMessage("Uygulama başlatıldı", 5000);

    // Kalıcı widget: kayıt sayısı
    QLabel* kayitSayisi = new QLabel("Kayıt: 0");
    kayitSayisi->setStyleSheet("padding: 0 10px; color: #2c3e50;");
    durumCubugu->addPermanentWidget(kayitSayisi);

    // Kalıcı widget: bağlantı durumu
    QLabel* baglantiDurum = new QLabel("[OK] Bağlı");
    baglantiDurum->setStyleSheet(
        "padding: 2px 8px; background-color: #27ae60; "
        "color: white; border-radius: 3px;"
    );
    durumCubugu->addPermanentWidget(baglantiDurum);

    // =============================================
    // 3) DOCK WIDGET - GEZİNME PANELİ
    // =============================================
    QDockWidget* gezinmeDock = new QDockWidget("Gezinme Paneli", &anaPencere);

    // İzin verilen kenetlenme alanları
    gezinmeDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
    );

    // Dock widget özellikleri
    gezinmeDock->setFeatures(
        QDockWidget::DockWidgetMovable |      // Taşınabilir
        QDockWidget::DockWidgetClosable |      // Kapatılabilir
        QDockWidget::DockWidgetFloatable       // Yüzer pencere olabilir
    );

    // Liste widget'ı dock içeriği olarak
    QListWidget* gezinmeListe = new QListWidget();
    gezinmeListe->addItem("Öğrenci Listesi");
    gezinmeListe->addItem("Ders Programı");
    gezinmeListe->addItem("Sınav Takvimi");
    gezinmeListe->addItem("Not Girişi");
    gezinmeListe->addItem("Raporlar");
    gezinmeListe->addItem("Ayarlar");
    gezinmeListe->setStyleSheet(
        "QListWidget { font-size: 13px; }"
        "QListWidget::item { padding: 8px; }"
        "QListWidget::item:selected { "
        "  background-color: #3498db; color: white; }"
    );

    gezinmeDock->setWidget(gezinmeListe);
    anaPencere.addDockWidget(Qt::LeftDockWidgetArea, gezinmeDock);

    // =============================================
    // 4) İKİNCİ DOCK - LOG PANELİ
    // =============================================
    QDockWidget* logDock = new QDockWidget("İşlem Günlüğü", &anaPencere);
    logDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);

    QTextEdit* logMetin = new QTextEdit();
    logMetin->setReadOnly(true);
    logMetin->setMaximumHeight(150);
    logMetin->setStyleSheet(
        "font-family: 'Consolas', monospace; font-size: 11px; "
        "background-color: #2c3e50; color: #ecf0f1;"
    );
    logMetin->append("[OK] Uygulama başlatıldı");

    logDock->setWidget(logMetin);
    anaPencere.addDockWidget(Qt::BottomDockWidgetArea, logDock);

    // =============================================
    // 5) SİNYAL/SLOT BAĞLANTILARI
    // =============================================

    // Gezinme listesinden öğe seçildiğinde
    int kayitSayaci = 0;
    QObject::connect(gezinmeListe, &QListWidget::currentTextChanged,
                     &anaPencere, [metin, logMetin, durumCubugu,
                                   kayitSayisi, &kayitSayaci](const QString& secilen) {
        metin->setText(QString("Seçilen modül: %1\n\n"
                               "Bu alan '%1' modülünün içeriğini gösterecek.")
                       .arg(secilen));

        logMetin->append(QString("[OK] Modül açıldı: %1").arg(secilen));
        durumCubugu->showMessage(
            QString("'%1' modülü yüklendi").arg(secilen), 3000
        );

        kayitSayaci++;
        kayitSayisi->setText(QString("Kayıt: %1").arg(kayitSayaci));

        qDebug() << "Seçilen modül:" << secilen;
    });

    anaPencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "OBYS - Durum Çubuğu ve Dock Widget"
 * Boyut: 800x600 piksel
 *
 * Yapı:
 *   +---------------------------------------------+
 *   | Gezinme    |   Ana Çalışma Alanı             |
 *   | Paneli     |   (QTextEdit)                   |
 *   | [Liste]    |                                 |
 *   |            |                                 |
 *   +---------------------------------------------+
 *   | İşlem Günlüğü (log paneli)                  |
 *   +---------------------------------------------+
 *   | Durum Çubuğu     | Kayıt: 0 | [OK] Bağlı   |
 *   +---------------------------------------------+
 *
 * Dock Paneli (Sol):
 *   - Öğrenci Listesi
 *   - Ders Programı
 *   - Sınav Takvimi
 *   - Not Girişi
 *   - Raporlar
 *   - Ayarlar
 *
 * Davranış:
 *   - Listeden öğe seçildiğinde: metin alanı güncellenir
 *   - Log paneline işlem kaydı eklenir
 *   - Durum çubuğunda geçici mesaj gösterilir
 *   - Kayıt sayacı artar
 *   - Dock panelleri sürüklenip taşınabilir, kapatılabilir
 *   - Gezinme paneli sola/sağa kenetlenebilir
 *   - Log paneli yukarı/aşağı kenetlenebilir
 */

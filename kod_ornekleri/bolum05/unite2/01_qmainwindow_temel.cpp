/**
 * QMainWindow Temelleri - Ana Pencere Yapısı
 *
 * Bu program, QMainWindow sınıfının temel kullanımını gösterir.
 * QMainWindow, menü çubuğu, araç çubuğu, durum çubuğu ve merkezi
 * widget alanlarını otomatik olarak yöneten bir ana pencere sınıfıdır.
 * Bu örnekte QTextEdit merkezi widget olarak kullanılmaktadır.
 *
 * Öğrenilecek kavramlar:
 * - QMainWindow sınıfı ve yapısı
 * - setCentralWidget() ile merkezi widget belirleme
 * - Pencere başlığı, boyutu ve konumu ayarlama
 * - resize() ve setMinimumSize() kullanımı
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
 *       -o 01_qmainwindow_temel 01_qmainwindow_temel.cpp
 */

#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QStatusBar>
#include <QLabel>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) ANA PENCERE OLUŞTUR
    // =============================================
    QMainWindow anaPencere;
    anaPencere.setWindowTitle("OBYS - Metin Düzenleyici");
    anaPencere.resize(700, 500);
    anaPencere.setMinimumSize(400, 300);

    // =============================================
    // 2) MERKEZİ WİDGET
    // =============================================
    // QMainWindow'un en önemli özelliği: merkezi widget alanı.
    // setCentralWidget() ile belirlenen widget, penceredeki
    // menü, araç çubuğu ve durum çubuğu dışındaki tüm alanı kaplar.
    QTextEdit* metin = new QTextEdit();
    metin->setPlaceholderText(
        "Buraya metin yazabilirsiniz...\n\n"
        "QMainWindow, merkezi widget alanını otomatik yönetir.\n"
        "Pencere boyutu değiştiğinde metin alanı da buna uyum sağlar."
    );
    metin->setStyleSheet(
        "QTextEdit {"
        "  font-family: 'Consolas', 'Monaco', monospace;"
        "  font-size: 14px;"
        "  padding: 10px;"
        "  background-color: #fefefe;"
        "}"
    );

    // Merkezi widget olarak ayarla
    // [NOT] QMainWindow, widget'ın sahipliğini (ownership) alır
    anaPencere.setCentralWidget(metin);

    // =============================================
    // 3) DURUM ÇUBUĞU
    // =============================================
    // statusBar() ilk çağrıda otomatik oluşturur
    anaPencere.statusBar()->showMessage("Hazır", 3000);

    // Kalıcı durum mesajı
    QLabel* durumEtiket = new QLabel("Satır: 1  Sütun: 1");
    anaPencere.statusBar()->addPermanentWidget(durumEtiket);

    // Metin değiştiğinde durum çubuğunu güncelle
    QObject::connect(metin, &QTextEdit::cursorPositionChanged,
                     durumEtiket, [metin, durumEtiket]() {
        auto imleç = metin->textCursor();
        int satir = imleç.blockNumber() + 1;
        int sutun = imleç.columnNumber() + 1;
        durumEtiket->setText(
            QString("Satır: %1  Sütun: %2").arg(satir).arg(sutun)
        );
    });

    anaPencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "OBYS - Metin Düzenleyici"
 * Boyut: 700x500 piksel
 *
 * İçerik:
 *   - Merkezi QTextEdit alanı (tüm pencereyi kaplar)
 *   - Placeholder metin: "Buraya metin yazabilirsiniz..."
 *   - Durum çubuğu: "Hazır" (3 sn sonra kaybolur)
 *   - Kalıcı durum: "Satır: 1  Sütun: 1"
 *
 * Davranış:
 *   - Metin yazıldığında imleç konumu durum çubuğunda güncellenir
 *   - Pencere boyutu değiştiğinde metin alanı otomatik uyum sağlar
 *   - Minimum boyut 400x300 piksel
 */

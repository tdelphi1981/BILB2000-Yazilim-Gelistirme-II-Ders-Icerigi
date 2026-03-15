/**
 * Menü ve Araç Çubuğu - QMenuBar, QAction, QToolBar
 *
 * Bu program, QMainWindow üzerinde menü çubuğu, araç çubuğu ve
 * kısayol tuşları oluşturmayı gösterir. Dosya, Düzen ve Yardım
 * menüleri ile yaygın eylemler (QAction) tanımlanır. Her eylem
 * kısayol tuşu ve durum çubuğu mesajı ile ilişkilendirilmiştir.
 *
 * Öğrenilecek kavramlar:
 * - QMenuBar ve QMenu ile menü oluşturma
 * - QAction ile eylem tanımlama
 * - QKeySequence ile standart kısayollar
 * - QToolBar ile araç çubuğu
 * - Eylemleri sinyal/slot ile bağlama
 * - Lambda slot kullanımı
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
 *       -o 02_menu_arac_cubugu 02_menu_arac_cubugu.cpp
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QTextEdit>
#include <QStatusBar>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) ANA PENCERE
    // =============================================
    QMainWindow anaPencere;
    anaPencere.setWindowTitle("OBYS - Menü ve Araç Çubuğu");
    anaPencere.resize(800, 600);

    // Merkezi widget
    QTextEdit* metin = new QTextEdit();
    metin->setPlaceholderText("İşlem sonuçları burada görünecek...");
    anaPencere.setCentralWidget(metin);

    // =============================================
    // 2) EYLEMLER (QAction)
    // =============================================

    // -- Dosya Menüsü Eylemleri --
    QAction* yeniEylem = new QAction("&Yeni", &anaPencere);
    yeniEylem->setShortcut(QKeySequence::New);
    yeniEylem->setStatusTip("Yeni belge oluştur");

    QAction* acEylem = new QAction("&Aç...", &anaPencere);
    acEylem->setShortcut(QKeySequence::Open);
    acEylem->setStatusTip("Mevcut belgeyi aç");

    QAction* kaydetEylem = new QAction("&Kaydet", &anaPencere);
    kaydetEylem->setShortcut(QKeySequence::Save);
    kaydetEylem->setStatusTip("Belgeyi kaydet");

    QAction* cikisEylem = new QAction("Çı&kış", &anaPencere);
    cikisEylem->setShortcut(QKeySequence::Quit);
    cikisEylem->setStatusTip("Uygulamadan çık");

    // -- Düzen Menüsü Eylemleri --
    QAction* kesEylem = new QAction("&Kes", &anaPencere);
    kesEylem->setShortcut(QKeySequence::Cut);
    kesEylem->setStatusTip("Seçili metni kes");

    QAction* kopyalaEylem = new QAction("K&opyala", &anaPencere);
    kopyalaEylem->setShortcut(QKeySequence::Copy);
    kopyalaEylem->setStatusTip("Seçili metni kopyala");

    QAction* yapistirEylem = new QAction("Ya&pıştır", &anaPencere);
    yapistirEylem->setShortcut(QKeySequence::Paste);
    yapistirEylem->setStatusTip("Panodan yapıştır");

    // -- Yardım Menüsü Eylemleri --
    QAction* hakkindaEylem = new QAction("&Hakkında", &anaPencere);
    hakkindaEylem->setStatusTip("Uygulama hakkında bilgi");

    // =============================================
    // 3) MENÜ ÇUBUĞU
    // =============================================
    QMenuBar* menuCubugu = anaPencere.menuBar();

    // Dosya menüsü
    QMenu* dosyaMenu = menuCubugu->addMenu("&Dosya");
    dosyaMenu->addAction(yeniEylem);
    dosyaMenu->addAction(acEylem);
    dosyaMenu->addAction(kaydetEylem);
    dosyaMenu->addSeparator();          // Ayırıcı çizgi
    dosyaMenu->addAction(cikisEylem);

    // Düzen menüsü
    QMenu* duzenMenu = menuCubugu->addMenu("Dü&zen");
    duzenMenu->addAction(kesEylem);
    duzenMenu->addAction(kopyalaEylem);
    duzenMenu->addAction(yapistirEylem);

    // Yardım menüsü
    QMenu* yardimMenu = menuCubugu->addMenu("&Yardım");
    yardimMenu->addAction(hakkindaEylem);

    // =============================================
    // 4) ARAÇ ÇUBUĞU
    // =============================================
    QToolBar* aracCubugu = anaPencere.addToolBar("Ana Araç Çubuğu");
    aracCubugu->addAction(yeniEylem);
    aracCubugu->addAction(acEylem);
    aracCubugu->addAction(kaydetEylem);
    aracCubugu->addSeparator();
    aracCubugu->addAction(kesEylem);
    aracCubugu->addAction(kopyalaEylem);
    aracCubugu->addAction(yapistirEylem);

    // =============================================
    // 5) SİNYAL/SLOT BAĞLANTILARI
    // =============================================

    // Dosya eylemleri
    QObject::connect(yeniEylem, &QAction::triggered, &anaPencere,
                     [metin, &anaPencere]() {
        metin->clear();
        anaPencere.statusBar()->showMessage("Yeni belge oluşturuldu", 3000);
        qDebug() << "[OK] Yeni belge oluşturuldu";
    });

    QObject::connect(acEylem, &QAction::triggered, &anaPencere,
                     [metin, &anaPencere]() {
        metin->setText("Dosyadan okunan örnek metin...\n"
                       "(Gerçek dosya açma işlemi Bölüm 5, Ünite 3'te)");
        anaPencere.statusBar()->showMessage("Dosya açıldı (simülasyon)", 3000);
        qDebug() << "[OK] Dosya aç simülasyonu";
    });

    QObject::connect(kaydetEylem, &QAction::triggered, &anaPencere,
                     [&anaPencere]() {
        anaPencere.statusBar()->showMessage("Belge kaydedildi (simülasyon)", 3000);
        qDebug() << "[OK] Kaydet simülasyonu";
    });

    QObject::connect(cikisEylem, &QAction::triggered,
                     &uygulama, &QApplication::quit);

    // Düzen eylemleri - QTextEdit'in kendi slotlarına bağla
    QObject::connect(kesEylem, &QAction::triggered, metin, &QTextEdit::cut);
    QObject::connect(kopyalaEylem, &QAction::triggered, metin, &QTextEdit::copy);
    QObject::connect(yapistirEylem, &QAction::triggered, metin, &QTextEdit::paste);

    // Hakkında
    QObject::connect(hakkindaEylem, &QAction::triggered, &anaPencere,
                     [&anaPencere]() {
        QMessageBox::about(&anaPencere, "Hakkında",
                           "OBYS Metin Düzenleyici v1.0\n\n"
                           "Qt Pencere ve Düzen Yönetimi\n"
                           "Bölüm 5 - Ünite 2 Örneği");
    });

    // =============================================
    // 6) DURUM ÇUBUĞU
    // =============================================
    anaPencere.statusBar()->showMessage("Hazır", 5000);

    anaPencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "OBYS - Menü ve Araç Çubuğu"
 * Boyut: 800x600 piksel
 *
 * Menü Çubuğu:
 *   Dosya        Düzen        Yardım
 *   |- Yeni      |- Kes       |- Hakkında
 *   |- Aç...     |- Kopyala
 *   |- Kaydet    |- Yapıştır
 *   |--------
 *   |- Çıkış
 *
 * Araç Çubuğu:
 *   [Yeni] [Aç] [Kaydet] | [Kes] [Kopyala] [Yapıştır]
 *
 * Merkezi Widget:
 *   QTextEdit metin alanı
 *
 * Durum Çubuğu:
 *   "Hazır" (5 sn sonra kaybolur)
 *
 * Davranış:
 *   - Ctrl+N: Yeni belge (metin temizlenir)
 *   - Ctrl+O: Dosya aç simülasyonu
 *   - Ctrl+S: Kaydet simülasyonu
 *   - Ctrl+Q: Çıkış
 *   - Ctrl+X/C/V: Kes/Kopyala/Yapıştır
 *   - Her işlem durum çubuğunda bildirilir
 *
 * Konsol çıktısı (eylemler tetiklendiğinde):
 *   [OK] Yeni belge oluşturuldu
 *   [OK] Dosya aç simülasyonu
 *   [OK] Kaydet simülasyonu
 */

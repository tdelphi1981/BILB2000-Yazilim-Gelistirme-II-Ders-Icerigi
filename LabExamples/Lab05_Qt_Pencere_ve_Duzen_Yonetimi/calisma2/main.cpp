/**
 * Lab 05 - Calisma 2 Baslangic Kodu
 * QMainWindow ve Menuler
 *
 * Bu dosya Lab 05 foyu ile birlikte kullanilir.
 * "GOREV" isaretli bolgeleri tamamlayin.
 *
 * Ogrenilecek kavramlar:
 * - QMainWindow yapisi (menu bar, toolbar, status bar)
 * - QMenu ve QAction ile menu olusturma
 * - Klavye kisayollari (QKeySequence)
 * - QToolBar ile arac cubugu
 * - QStatusBar ile durum mesajlari
 * - Sinyal/slot baglantilari
 *
 * Lab: 05 - Qt Pencere ve Duzen Yonetimi
 * Calisma: 2 - QMainWindow ve Menuler (30 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Calistirma: ./app
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow anaPencere;
    anaPencere.setWindowTitle("OBYS - Ana Pencere");
    anaPencere.resize(700, 500);

    // Merkezi widget: metin duzenleyici
    auto* editor = new QTextEdit;
    editor->setPlaceholderText(
        "OBYS kayitlari burada goruntulenir...");
    anaPencere.setCentralWidget(editor);

    // --- Menu Cubugu ---
    auto* dosyaMenu = anaPencere.menuBar()->addMenu("&Dosya");

    auto* yeniAksiyon = new QAction("&Yeni", &anaPencere);
    yeniAksiyon->setShortcut(QKeySequence("Ctrl+N"));
    dosyaMenu->addAction(yeniAksiyon);

    auto* kaydetAksiyon = new QAction("&Kaydet", &anaPencere);
    kaydetAksiyon->setShortcut(QKeySequence("Ctrl+S"));
    dosyaMenu->addAction(kaydetAksiyon);

    dosyaMenu->addSeparator();

    auto* cikisAksiyon = new QAction("C&ikis", &anaPencere);
    cikisAksiyon->setShortcut(QKeySequence("Ctrl+Q"));
    dosyaMenu->addAction(cikisAksiyon);

    // --- Arac Cubugu ---
    auto* aracCubugu = anaPencere.addToolBar("Ana Araclar");
    aracCubugu->addAction(yeniAksiyon);
    aracCubugu->addAction(kaydetAksiyon);

    // --- Durum Cubugu ---
    anaPencere.statusBar()->showMessage("Hazir", 3000);

    // --- Sinyal/Slot Baglantilari ---
    QObject::connect(yeniAksiyon, &QAction::triggered,
        [editor, &anaPencere]() {
            editor->clear();
            anaPencere.statusBar()->showMessage(
                "Yeni belge olusturuldu", 2000);
        });

    QObject::connect(kaydetAksiyon, &QAction::triggered,
        [&anaPencere]() {
            anaPencere.statusBar()->showMessage(
                "Kaydedildi!", 2000);
            qDebug() << "Kaydet tiklandi";
        });

    QObject::connect(cikisAksiyon, &QAction::triggered,
                     &app, &QApplication::quit);

    // === GOREV 2.1 - Temel Menu Olusturma (10 dk) ===
    // TODO: a) "Gorunum" adinda yeni bir menu ekleyin
    //          auto* gorunumMenu = anaPencere.menuBar()->addMenu("&Gorunum");
    //
    // TODO: b) Bu menuye "Tam Ekran" aksiyonu ekleyin (kisayol: F11)
    //          auto* tamEkranAksiyon = new QAction("Tam Ekran", &anaPencere);
    //          tamEkranAksiyon->setShortcut(QKeySequence("F11"));
    //          gorunumMenu->addAction(tamEkranAksiyon);
    //
    // TODO: c) Tam Ekran aksiyonuna tiklandiginda showFullScreen() veya
    //          showNormal() arasinda gecis yapin:
    //          bool tamEkran = false;
    //          QObject::connect(tamEkranAksiyon, &QAction::triggered,
    //              [&anaPencere, &tamEkran]() {
    //                  tamEkran = !tamEkran;
    //                  if (tamEkran) anaPencere.showFullScreen();
    //                  else anaPencere.showNormal();
    //              });
    //
    // TODO: d) Durum cubuguna pencere modunu yazdirin
    //          anaPencere.statusBar()->showMessage(
    //              tamEkran ? "Tam Ekran" : "Normal", 2000);
    // ============================================

    // === GOREV 2.2 - Duzenle Menusu (20 dk) ===
    // TODO: a) Dosya ve Gorunum menuleri arasina "&Duzenle" menusu ekleyin
    //          auto* duzenleMenu = anaPencere.menuBar()->addMenu("&Duzenle");
    //          (Not: Menuyu Dosya'dan sonra, Gorunum'den once ekleyin)
    //
    // TODO: b) Asagidaki aksiyonlari olusturun:
    //          auto* kesAksiyon = new QAction("Kes", &anaPencere);
    //          kesAksiyon->setShortcut(QKeySequence("Ctrl+X"));
    //          duzenleMenu->addAction(kesAksiyon);
    //
    //          auto* kopyalaAksiyon = new QAction("Kopyala", &anaPencere);
    //          kopyalaAksiyon->setShortcut(QKeySequence("Ctrl+C"));
    //          duzenleMenu->addAction(kopyalaAksiyon);
    //
    //          auto* yapistirAksiyon = new QAction("Yapistir", &anaPencere);
    //          yapistirAksiyon->setShortcut(QKeySequence("Ctrl+V"));
    //          duzenleMenu->addAction(yapistirAksiyon);
    //
    // TODO: c) Her aksiyonu QTextEdit'in ilgili slot'larina baglayin:
    //          QObject::connect(kesAksiyon, &QAction::triggered,
    //                           editor, &QTextEdit::cut);
    //          QObject::connect(kopyalaAksiyon, &QAction::triggered,
    //                           editor, &QTextEdit::copy);
    //          QObject::connect(yapistirAksiyon, &QAction::triggered,
    //                           editor, &QTextEdit::paste);
    //
    // TODO: d) Kes ve Kopyala aksiyonlarini arac cubuguna da ekleyin
    //          aracCubugu->addSeparator();
    //          aracCubugu->addAction(kesAksiyon);
    //          aracCubugu->addAction(kopyalaAksiyon);
    //
    // TODO: e) Her islemden sonra durum cubugunda bilgi mesaji gosterin
    //          QObject::connect(kesAksiyon, &QAction::triggered,
    //              [&anaPencere]() {
    //                  anaPencere.statusBar()->showMessage("Metin kesildi", 2000);
    //              });
    // ============================================

    anaPencere.show();
    return app.exec();
}

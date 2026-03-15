/**
 * Lab 05 - Calisma 4 Baslangic Kodu
 * OBYS Ana Pencere
 *
 * Bu dosya Lab 05 foyu ile birlikte kullanilir.
 * "GOREV" isaretli bolgeleri tamamlayin.
 *
 * Ogrenilecek kavramlar:
 * - QMainWindow ile tam uygulamali pencere iskeleti
 * - QTableWidget ile tablo gorunumu
 * - QDockWidget ile yan panel
 * - Menuler, arac cubugu ve durum cubugu entegrasyonu
 * - QDialog ile veri girisi ve tabloya ekleme
 * - Arama ve vurgulama islevi
 *
 * Lab: 05 - Qt Pencere ve Duzen Yonetimi
 * Calisma: 4 - OBYS Ana Pencere (30 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Calistirma: ./app
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow anaPencere;
    anaPencere.setWindowTitle("OBYS - Ana Pencere");
    anaPencere.resize(800, 600);

    // Merkezi widget ve ana duzen
    auto* merkez = new QWidget;
    auto* anaDuzen = new QVBoxLayout(merkez);

    // Baslik
    auto* baslik = new QLabel("Ogrenci Bilgi Yonetim Sistemi");
    baslik->setStyleSheet(
        "font-size: 18px; font-weight: bold;"
        " color: #008080; padding: 5px;");
    baslik->setAlignment(Qt::AlignCenter);
    anaDuzen->addWidget(baslik);

    // Arama formu
    auto* formDuzen = new QHBoxLayout;
    formDuzen->addWidget(new QLabel("Ara:"));
    auto* aramaAlani = new QLineEdit;
    aramaAlani->setPlaceholderText("Ad veya numara...");
    formDuzen->addWidget(aramaAlani);
    auto* araButon = new QPushButton("Ara");
    formDuzen->addWidget(araButon);
    anaDuzen->addLayout(formDuzen);

    // Ogrenci tablosu
    auto* tablo = new QTableWidget(3, 4);
    tablo->setHorizontalHeaderLabels(
        {"No", "Ad Soyad", "Bolum", "Ortalama"});
    tablo->horizontalHeader()->setStretchLastSection(true);

    // Ornek veri
    tablo->setItem(0, 0, new QTableWidgetItem("101"));
    tablo->setItem(0, 1, new QTableWidgetItem("Ali Yilmaz"));
    tablo->setItem(0, 2, new QTableWidgetItem("Bilgisayar"));
    tablo->setItem(0, 3, new QTableWidgetItem("3.45"));
    tablo->setItem(1, 0, new QTableWidgetItem("102"));
    tablo->setItem(1, 1, new QTableWidgetItem("Ayse Kaya"));
    tablo->setItem(1, 2, new QTableWidgetItem("Yazilim"));
    tablo->setItem(1, 3, new QTableWidgetItem("3.80"));
    tablo->setItem(2, 0, new QTableWidgetItem("103"));
    tablo->setItem(2, 1, new QTableWidgetItem("Veli Ozturk"));
    tablo->setItem(2, 2, new QTableWidgetItem("Elektrik"));
    tablo->setItem(2, 3, new QTableWidgetItem("2.95"));
    anaDuzen->addWidget(tablo);

    anaPencere.setCentralWidget(merkez);

    // Menu cubugu
    auto* dosyaMenu =
        anaPencere.menuBar()->addMenu("&Dosya");
    auto* yeniAksiyon =
        new QAction("&Yeni Ogrenci", &anaPencere);
    yeniAksiyon->setShortcut(QKeySequence("Ctrl+N"));
    dosyaMenu->addAction(yeniAksiyon);
    dosyaMenu->addSeparator();
    auto* cikisAksiyon =
        new QAction("C&ikis", &anaPencere);
    dosyaMenu->addAction(cikisAksiyon);

    // Ogrenci menusu
    auto* ogrenciMenu =
        anaPencere.menuBar()->addMenu("&Ogrenci");
    ogrenciMenu->addAction(yeniAksiyon);

    // Yardim menusu
    auto* yardimMenu =
        anaPencere.menuBar()->addMenu("&Yardim");
    auto* hakkindaAksiyon =
        new QAction("&Hakkinda", &anaPencere);
    yardimMenu->addAction(hakkindaAksiyon);

    QObject::connect(hakkindaAksiyon, &QAction::triggered,
        [&anaPencere]() {
            QMessageBox::information(&anaPencere, "Hakkinda",
                "OBYS - Ogrenci Bilgi Yonetim Sistemi\n"
                "Versiyon 1.0\n"
                "Lab 05 - Qt Pencere ve Duzen Yonetimi");
        });

    // Arac cubugu
    auto* aracCubugu =
        anaPencere.addToolBar("Islemler");
    aracCubugu->addAction(yeniAksiyon);

    // Durum cubugu
    anaPencere.statusBar()->showMessage(
        "3 ogrenci kayitli", 5000);

    // Baglanti: Cikis
    QObject::connect(cikisAksiyon, &QAction::triggered,
                     &app, &QApplication::quit);

    // === GOREV 4.1 - Dock Widget Ekleme (15 dk) ===
    // TODO: a) Sag tarafa bir QDockWidget ekleyin: "Ogrenci Detaylari"
    //          auto* dock = new QDockWidget(
    //              "Ogrenci Detaylari", &anaPencere);
    //          dock->setAllowedAreas(
    //              Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //
    // TODO: b) Dock widget'in icine QFormLayout ile bir detay formu
    //          yerlestirin: Ad, Bolum, Ortalama etiketleri
    //          auto* detayWidget = new QWidget;
    //          auto* detayDuzen = new QFormLayout(detayWidget);
    //          detayDuzen->addRow("Ad:", new QLabel("--"));
    //          detayDuzen->addRow("Bolum:", new QLabel("--"));
    //          detayDuzen->addRow("Ortalama:", new QLabel("--"));
    //          dock->setWidget(detayWidget);
    //
    // TODO: c) Dock widget'i ana pencereye ekleyin:
    //          anaPencere.addDockWidget(
    //              Qt::RightDockWidgetArea, dock);
    //
    // TODO: d) Gorunum menusu olusturup dock widget'in goster/gizle
    //          aksiyonunu ekleyin:
    //          auto* gorunumMenu =
    //              anaPencere.menuBar()->addMenu("&Gorunum");
    //          gorunumMenu->addAction(dock->toggleViewAction());
    // ============================================

    // === GOREV 4.2 - Menu Aksiyonlarini Baglama (15 dk) ===
    // TODO: a) "Yeni Ogrenci" aksiyonuna tiklandiginda bir QDialog acin
    //          (Calisma 3'teki gibi). Diyalogda Ad Soyad ve Bolum
    //          alanlari olsun:
    //          QObject::connect(yeniAksiyon, &QAction::triggered,
    //              [&anaPencere, tablo]() {
    //                  QDialog diyalog(&anaPencere);
    //                  diyalog.setWindowTitle("Yeni Ogrenci");
    //                  auto* form = new QFormLayout(&diyalog);
    //                  auto* adAlani = new QLineEdit;
    //                  auto* bolumAlani = new QLineEdit;
    //                  form->addRow("Ad Soyad:", adAlani);
    //                  form->addRow("Bolum:", bolumAlani);
    //                  auto* butonlar = new QDialogButtonBox(
    //                      QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    //                  form->addRow(butonlar);
    //                  QObject::connect(butonlar,
    //                      &QDialogButtonBox::accepted,
    //                      &diyalog, &QDialog::accept);
    //                  QObject::connect(butonlar,
    //                      &QDialogButtonBox::rejected,
    //                      &diyalog, &QDialog::reject);
    //
    // TODO: b) Diyalogdan donen bilgiyi tabloya yeni satir olarak ekleyin:
    //                  if (diyalog.exec() == QDialog::Accepted) {
    //                      int satir = tablo->rowCount();
    //                      tablo->insertRow(satir);
    //                      tablo->setItem(satir, 0,
    //                          new QTableWidgetItem(
    //                              QString::number(100 + satir + 1)));
    //                      tablo->setItem(satir, 1,
    //                          new QTableWidgetItem(adAlani->text()));
    //                      tablo->setItem(satir, 2,
    //                          new QTableWidgetItem(bolumAlani->text()));
    //                      tablo->setItem(satir, 3,
    //                          new QTableWidgetItem("0.00"));
    //                  }
    //              });
    //
    // TODO: c) Her ekleme sonrasinda durum cubugunu guncelleyin:
    //          anaPencere.statusBar()->showMessage(
    //              QString("Toplam: %1 ogrenci kayitli")
    //                  .arg(tablo->rowCount()), 3000);
    //
    // TODO: d) Arama butonuna tiklandiginda tablodaki eslesen satirlari
    //          vurgulayin (setBackground() ile):
    //          QObject::connect(araButon, &QPushButton::clicked,
    //              [tablo, aramaAlani]() {
    //                  QString aranan = aramaAlani->text().toLower();
    //                  for (int i = 0; i < tablo->rowCount(); ++i) {
    //                      bool eslesti = false;
    //                      for (int j = 0; j < tablo->columnCount(); ++j) {
    //                          auto* hucre = tablo->item(i, j);
    //                          if (hucre && hucre->text().toLower().contains(aranan)) {
    //                              eslesti = true;
    //                              break;
    //                          }
    //                      }
    //                      for (int j = 0; j < tablo->columnCount(); ++j) {
    //                          if (auto* hucre = tablo->item(i, j)) {
    //                              hucre->setBackground(
    //                                  eslesti ? QColor("#FFFFCC") : QColor("#FFFFFF"));
    //                          }
    //                      }
    //                  }
    //              });
    // ============================================

    anaPencere.show();
    return app.exec();
}

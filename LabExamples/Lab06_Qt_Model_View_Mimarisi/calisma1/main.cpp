/**
 * Lab 06 - Calisma 1 Baslangic Kodu
 * QStringListModel ve QListView
 *
 * Bu dosya Lab 06 foyu ile birlikte kullanilir.
 * "GOREV" isaretli bolgeleri tamamlayin.
 *
 * Ogrenilecek kavramlar:
 * - Model/View mimarisinin temelleri
 * - QStringListModel ile basit liste modeli
 * - QListView ile liste gorunumu
 * - insertRow() ve removeRow() ile model manipulasyonu
 * - setEditTriggers() ile duzenleme kontrolu
 * - dataChanged sinyali ile degisiklik izleme
 * - selectionModel() ile secim bilgisi
 *
 * Lab: 06 - Qt Model/View Mimarisi
 * Calisma: 1 - QStringListModel ve QListView (30 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Calistirma: ./app
 */

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QStringListModel>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget pencere;
    pencere.setWindowTitle("OBYS - Bölüm Listesi");
    pencere.resize(400, 350);

    auto* anaDuzen = new QVBoxLayout(&pencere);

    // Baslik
    auto* baslik = new QLabel("Bölüm Yönetimi");
    baslik->setStyleSheet(
        "font-size: 16px; font-weight: bold;"
        " color: #008080;");
    baslik->setAlignment(Qt::AlignCenter);
    anaDuzen->addWidget(baslik);

    // Model olustur
    auto* model = new QStringListModel;
    model->setStringList({
        "Bilgisayar Bilimleri",
        "Yazilim Muhendisligi",
        "Elektrik-Elektronik",
        "Makine Muhendisligi"
    });

    // Gorunum olustur ve modeli bagla
    auto* liste = new QListView;
    liste->setModel(model);
    liste->setEditTriggers(
        QAbstractItemView::NoEditTriggers);
    anaDuzen->addWidget(liste);

    // Ekleme formu
    auto* formDuzen = new QHBoxLayout;
    auto* yeniAlani = new QLineEdit;
    yeniAlani->setPlaceholderText("Yeni bölüm adi...");
    auto* ekleButon = new QPushButton("Ekle");
    auto* silButon = new QPushButton("Sil");
    formDuzen->addWidget(yeniAlani);
    formDuzen->addWidget(ekleButon);
    formDuzen->addWidget(silButon);
    anaDuzen->addLayout(formDuzen);

    // Durum etiketi
    auto* durumLabel = new QLabel("4 bölüm kayitli");
    durumLabel->setAlignment(Qt::AlignCenter);
    anaDuzen->addWidget(durumLabel);

    // Ekle butonu
    QObject::connect(ekleButon, &QPushButton::clicked,
        [model, yeniAlani, durumLabel]() {
            QString yeni = yeniAlani->text().trimmed();
            if (yeni.isEmpty()) return;

            int satir = model->rowCount();
            model->insertRow(satir);
            model->setData(model->index(satir), yeni);
            yeniAlani->clear();
            durumLabel->setText(
                QString("%1 bölüm kayitli")
                .arg(model->rowCount()));
        });

    // Sil butonu
    QObject::connect(silButon, &QPushButton::clicked,
        [model, liste, durumLabel]() {
            auto secili = liste->currentIndex();
            if (!secili.isValid()) return;
            model->removeRow(secili.row());
            durumLabel->setText(
                QString("%1 bölüm kayitli")
                .arg(model->rowCount()));
        });

    // === GOREV 1.1 - Cift Tiklama ile Duzenleme (10 dk) ===
    // TODO: a) setEditTriggers() parametresini
    //          QAbstractItemView::DoubleClicked olarak degistirin.
    //
    // TODO: b) Bir bolum adini cift tiklayarak duzenleyin ve degisikligin
    //          modele yansidigini dogrulayin.
    //
    // TODO: c) Duzenleme sonrasi durum etiketinde
    //          "Duzenlendi: [yeni_ad]" gosterin. Bunun icin
    //          modelin dataChanged sinyalini kullanin:
    //          QObject::connect(model,
    //              &QStringListModel::dataChanged,
    //              [model, durumLabel](const QModelIndex& idx) {
    //                  durumLabel->setText(
    //                      QString("Duzenlendi: %1")
    //                      .arg(model->data(idx).toString()));
    //              });
    // ============================================

    // === GOREV 1.2 - Secim Bilgisi ve Arama (20 dk) ===
    // TODO: a) Bir oge secildiginde durum etiketinde secilen bolum adini
    //          ve indeksini gosterin. QListView'in selectionModel()
    //          uzerinden currentChanged sinyalini kullanin:
    //          QObject::connect(
    //              liste->selectionModel(),
    //              &QItemSelectionModel::currentChanged,
    //              [durumLabel](const QModelIndex& current) {
    //                  durumLabel->setText(
    //                      QString("Secili [%1]: %2")
    //                      .arg(current.row())
    //                      .arg(current.data().toString()));
    //              });
    //
    // TODO: b) Bir QLineEdit ve "Ara" butonu ekleyin. Butona
    //          tiklandiginda modeldeki eslesen ilk ogeyi secin:
    //          auto* aramaAlani = new QLineEdit;
    //          aramaAlani->setPlaceholderText("Ara...");
    //          auto* araButon = new QPushButton("Ara");
    //          // formDuzen veya yeni bir QHBoxLayout ile ekleyin
    //
    //          QObject::connect(araButon, &QPushButton::clicked,
    //              [model, liste, aramaAlani, &pencere]() {
    //                  QString aranan = aramaAlani->text().trimmed();
    //                  if (aranan.isEmpty()) return;
    //                  auto eslesme = model->match(
    //                      model->index(0), Qt::DisplayRole,
    //                      aranan, 1, Qt::MatchContains);
    //                  if (!eslesme.isEmpty())
    //                      liste->setCurrentIndex(eslesme.first());
    //                  else
    //                      QMessageBox::information(&pencere,
    //                          "Arama", "Sonuc bulunamadi");
    //              });
    //
    // TODO: c) Eslesme bulunamazsa QMessageBox::information() ile
    //          "Sonuc bulunamadi" mesaji gosterin (yukarida zaten var).
    // ============================================

    pencere.show();
    return app.exec();
}

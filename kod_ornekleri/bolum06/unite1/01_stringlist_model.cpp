/**
 * QStringListModel ve QListView Ornegi
 *
 * Bu program basit bir liste modeli olusturur ve
 * QListView ile gosterir. Ekleme ve silme butonlari
 * ile Model/View mimarisini gosterir.
 *
 * Ogrenilecek kavramlar:
 * - QStringListModel kullanimi
 * - QListView ile model gosterimi
 * - Model/View baglamasi
 *
 * Bolum: 06 - Qt Model/View Mimarisi
 * Unite: 1 - Model/View Kavrami
 *
 * Derleme: g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) -o program program.cpp
 */

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListView>
#include <QStringListModel>
#include <QInputDialog>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto *pencere = new QWidget;
    pencere->setWindowTitle("OBYS - Bolum Listesi (Model/View)");
    pencere->resize(400, 300);

    // Model olustur
    auto *model = new QStringListModel(pencere);
    model->setStringList({
        "Bilgisayar Bilimleri",
        "Yazilim Muhendisligi",
        "Elektrik-Elektronik",
        "Makine Muhendisligi"
    });

    // Gorunum olustur
    auto *listView = new QListView;
    listView->setModel(model);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Butonlar
    auto *ekleBtn = new QPushButton("Ekle");
    auto *silBtn = new QPushButton("Sil");

    // Ekle butonu
    QObject::connect(ekleBtn, &QPushButton::clicked, [=]() {
        bool ok;
        QString bolum = QInputDialog::getText(
            pencere, "Bolum Ekle", "Bolum adi:", QLineEdit::Normal, "", &ok);
        if (ok && !bolum.isEmpty()) {
            int satir = model->rowCount();
            model->insertRow(satir);
            model->setData(model->index(satir), bolum);
        }
    });

    // Sil butonu
    QObject::connect(silBtn, &QPushButton::clicked, [=]() {
        QModelIndex idx = listView->currentIndex();
        if (idx.isValid()) {
            model->removeRow(idx.row());
        } else {
            QMessageBox::information(pencere, "Bilgi", "Silinecek bir oge secin.");
        }
    });

    // Duzen
    auto *butonDuzen = new QHBoxLayout;
    butonDuzen->addWidget(ekleBtn);
    butonDuzen->addWidget(silBtn);

    auto *anaDuzen = new QVBoxLayout(pencere);
    anaDuzen->addWidget(listView);
    anaDuzen->addLayout(butonDuzen);

    pencere->show();
    return app.exec();
}

/*
BEKLENEN CIKTI:
---------------
Bir pencere acilir. Listede bolumler gosterilir.
"Ekle" ile yeni bolum eklenebilir.
"Sil" ile secili bolum silinebilir.
*/

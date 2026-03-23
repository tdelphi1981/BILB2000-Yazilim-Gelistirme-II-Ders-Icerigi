/**
 * ss01_stringlist_listview.cpp
 *
 * QStringListModel + QListView ornegi.
 * Bolum adlarini listeleyen, ekleme ve silme destekli basit bir pencere.
 *
 * Derleme:
 *   moc ss01_stringlist_listview.cpp -o ss01_stringlist_listview.moc
 *   g++ -std=c++20 $(pkg-config --cflags --libs Qt6Widgets) -fPIC \
 *       -o ss01 ss01_stringlist_listview.cpp
 */

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QStringListModel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>

class BolumListePenceresi : public QWidget {
    Q_OBJECT
public:
    explicit BolumListePenceresi(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setWindowTitle("QStringListModel Örneği");
        resize(500, 400);

        // Model olustur
        model_ = new QStringListModel(this);
        QStringList bolumler = {
            "Bilgisayar Bilimleri",
            "Yazılım Mühendisliği",
            "Elektrik-Elektronik",
            "Makine Mühendisliği",
            "İnşaat Mühendisliği"
        };
        model_->setStringList(bolumler);

        // Gorunum olustur
        listView_ = new QListView;
        listView_->setModel(model_);
        listView_->setEditTriggers(QAbstractItemView::NoEditTriggers);

        // Butonlar
        auto *ekleBtn = new QPushButton("Ekle");
        auto *silBtn  = new QPushButton("Sil");

        auto *butonLayout = new QHBoxLayout;
        butonLayout->addStretch();
        butonLayout->addWidget(ekleBtn);
        butonLayout->addWidget(silBtn);

        auto *anaLayout = new QVBoxLayout(this);
        anaLayout->addWidget(listView_);
        anaLayout->addLayout(butonLayout);

        connect(ekleBtn, &QPushButton::clicked, this, &BolumListePenceresi::bolumEkle);
        connect(silBtn,  &QPushButton::clicked, this, &BolumListePenceresi::bolumSil);
    }

private slots:
    void bolumEkle() {
        bool ok = false;
        QString yeniBolum = QInputDialog::getText(
            this, "Bölüm Ekle", "Bölüm adı:", QLineEdit::Normal, "", &ok);
        if (ok && !yeniBolum.isEmpty()) {
            int satir = model_->rowCount();
            model_->insertRow(satir);
            model_->setData(model_->index(satir), yeniBolum);
        }
    }

    void bolumSil() {
        QModelIndex secili = listView_->currentIndex();
        if (secili.isValid()) {
            model_->removeRow(secili.row());
        } else {
            QMessageBox::warning(this, "Uyarı", "Lütfen silinecek bir bölüm seçin.");
        }
    }

private:
    QStringListModel *model_ = nullptr;
    QListView *listView_     = nullptr;
};

#include "ss01_stringlist_listview.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BolumListePenceresi pencere;
    pencere.show();

    return app.exec();
}

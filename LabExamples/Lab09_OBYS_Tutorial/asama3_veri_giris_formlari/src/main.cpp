#include "obys_domain.h"
#include "obys_servis.h"
#include "not_giris_dialog.h"
#include "ui_ogrenci_ekle_dialog.h"
#include "ui_ders_ekle_dialog.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

// Öğrenci ekleme diyalogunu aç (.ui dosyası)
void ogrenciEkleTest(QWidget* parent, OBYSServis& servis) {
    QDialog dlg(parent);
    Ui::OgrenciEkleDialog ui;
    ui.setupUi(&dlg);

    QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted,
                     &dlg, &QDialog::accept);
    QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected,
                     &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    const QString numara = ui.numaraEdit->text().trimmed();
    const QString adSoyad = ui.adSoyadEdit->text().trimmed();
    if (numara.isEmpty() || adSoyad.isEmpty()) return;

    int yeniId = 1;
    for (const auto& o : servis.tumOgrenciler())
        if (o.id >= yeniId) yeniId = o.id + 1;

    servis.ogrenciEkle({yeniId, numara, adSoyad});
    qDebug() << "Öğrenci eklendi:" << adSoyad;
}

// Ders ekleme diyalogunu aç (.ui dosyası)
void dersEkleTest(QWidget* parent, OBYSServis& servis) {
    QDialog dlg(parent);
    Ui::DersEkleDialog ui;
    ui.setupUi(&dlg);

    QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted,
                     &dlg, &QDialog::accept);
    QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected,
                     &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    const QString kod = ui.kodEdit->text().trimmed();
    const QString ad = ui.adEdit->text().trimmed();
    const int kredi = ui.krediSpin->value();
    if (kod.isEmpty() || ad.isEmpty()) return;

    int yeniId = 1;
    for (const auto& d : servis.tumDersler())
        if (d.id >= yeniId) yeniId = d.id + 1;

    servis.dersEkle({yeniId, kod, ad, kredi});
    qDebug() << "Ders eklendi:" << ad;
}

// Not giriş diyalogunu aç (C++ kodu ile oluşturulan form)
void notEkleTest(QWidget* parent, OBYSServis& servis) {
    NotGirisDialog dlg(parent);

    QList<QPair<int, QString>> ogrenciler;
    for (const auto& o : servis.tumOgrenciler())
        ogrenciler.append({o.id, o.adSoyad});

    QList<QPair<int, QString>> dersler;
    for (const auto& d : servis.tumDersler())
        dersler.append({d.id, QString("%1 %2").arg(d.kod, d.ad)});

    dlg.ogrencileriYukle(ogrenciler);
    dlg.dersleriYukle(dersler);

    if (dlg.exec() == QDialog::Accepted) {
        servis.notEkle(dlg.notDTO());
        qDebug() << "Not eklendi";
    }
}

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QCoreApplication::setOrganizationName("KTU");
    QCoreApplication::setApplicationName("OBYS Tutorial");
    QCoreApplication::setApplicationVersion("1.0.0");

    OBYSServis servis;
    servis.ornekVerileriYukle();

    QMainWindow pencere;
    pencere.setWindowTitle("OBYS Tutorial - Aşama 3: Form Testi");
    pencere.resize(400, 200);

    auto* merkez = new QWidget(&pencere);
    auto* layout = new QVBoxLayout(merkez);

    auto* btnOgrenci = new QPushButton("Öğrenci Ekle (.ui)", merkez);
    auto* btnDers    = new QPushButton("Ders Ekle (.ui)", merkez);
    auto* btnNot     = new QPushButton("Not Ekle (C++ form)", merkez);

    layout->addWidget(btnOgrenci);
    layout->addWidget(btnDers);
    layout->addWidget(btnNot);

    pencere.setCentralWidget(merkez);

    QObject::connect(btnOgrenci, &QPushButton::clicked,
                     [&]{ ogrenciEkleTest(&pencere, servis); });
    QObject::connect(btnDers, &QPushButton::clicked,
                     [&]{ dersEkleTest(&pencere, servis); });
    QObject::connect(btnNot, &QPushButton::clicked,
                     [&]{ notEkleTest(&pencere, servis); });

    pencere.show();
    return uygulama.exec();
}

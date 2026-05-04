#include "AnaPencere.h"
#include "DisaAktarmaServisi.h"
#include "IceAktarmaServisi.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QStandardItem>
#include <QStatusBar>
#include <QThread>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

AnaPencere::AnaPencere(QWidget* parent)
    : QMainWindow(parent), depo_(std::make_shared<Depo<Ogrenci>>()) {
    uiKur();
    menuKur();
    setWindowTitle("OBYS - Responsive");
    resize(900, 600);
    statusBar()->showMessage(tr("Hazir"), 2000);
}

void AnaPencere::uiKur() {
    auto* merkez = new QWidget(this);
    auto* y = new QVBoxLayout(merkez);

    tablo_ = new QTableView(this);
    model_ = new QStandardItemModel(0, 4, this);
    model_->setHorizontalHeaderLabels({"Numara", "Ad", "Bolum", "GPA"});
    tablo_->setModel(model_);
    tablo_->setSelectionBehavior(QAbstractItemView::SelectRows);
    y->addWidget(tablo_);

    setCentralWidget(merkez);
}

void AnaPencere::menuKur() {
    auto* dosya = menuBar()->addMenu(tr("&Dosya"));
    auto* ice = dosya->addAction(tr("CSV &Ice Aktar..."));
    auto* disa = dosya->addAction(tr("CSV &Disa Aktar..."));
    dosya->addSeparator();
    auto* cik = dosya->addAction(tr("Cikis"));

    auto* islem = menuBar()->addMenu(tr("&Islem"));
    auto* hesap = islem->addAction(tr("&GPA Hesapla"));
    auto* test = islem->addAction(tr("Rastgele &Test Verisi"));
    islem->addSeparator();
    auto* yenile = islem->addAction(tr("&Tabloyu Yenile"));

    connect(ice, &QAction::triggered, this, &AnaPencere::csvIceAktarTetikle);
    connect(disa, &QAction::triggered, this, &AnaPencere::csvDisaAktarTetikle);
    connect(hesap, &QAction::triggered, this, &AnaPencere::gpaHesaplaTetikle);
    connect(test, &QAction::triggered, this, &AnaPencere::rastgeleVeriOlustur);
    connect(yenile, &QAction::triggered, this, &AnaPencere::tabloyuYenile);
    connect(cik, &QAction::triggered, this, &QMainWindow::close);
}

void AnaPencere::csvIceAktarTetikle() {
    QString yol = QFileDialog::getOpenFileName(
        this, tr("CSV Sec"), QString(), tr("CSV (*.csv)"));
    if (yol.isEmpty()) return;

    auto* thread = new QThread(this);
    auto* worker = new IceAktarmaServisi(depo_);
    worker->moveToThread(thread);

    auto* dialog = new QProgressDialog(
        tr("Hazirlaniyor..."), tr("Iptal"), 0, 0, this);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setMinimumDuration(500);
    dialog->setAutoClose(false);
    dialog->setAutoReset(false);

    connect(thread, &QThread::started, worker, [worker, yol] {
        worker->csvIceAktar(yol);
    });

    connect(worker, &IceAktarmaServisi::ilerleme, dialog,
            [dialog](qsizetype i, qsizetype n) {
                dialog->setMaximum(static_cast<int>(n));
                dialog->setValue(static_cast<int>(i));
            });

    connect(worker, &IceAktarmaServisi::durum, dialog,
            &QProgressDialog::setLabelText);

    connect(dialog, &QProgressDialog::canceled, worker,
            &IceAktarmaServisi::durdur, Qt::DirectConnection);

    connect(worker, &IceAktarmaServisi::tamamlandi, this,
            [this, dialog, thread, worker](qsizetype b, qsizetype h) {
                dialog->setValue(dialog->maximum());

                QString mesaj;
                if (dialog->wasCanceled()) {
                    mesaj = tr("Islem iptal edildi.\n"
                              "%1 kayit yuklendi.").arg(b);
                } else {
                    mesaj = tr("%1 kayit yuklendi, %2 hata.").arg(b).arg(h);
                }
                QMessageBox::information(this, tr("Ice Aktarma"), mesaj);

                tabloyuYenile();
                dialog->deleteLater();
                thread->quit();
                thread->wait();
                worker->deleteLater();
                thread->deleteLater();
            });

    thread->start();
}

void AnaPencere::csvDisaAktarTetikle() {
    QString yol = QFileDialog::getSaveFileName(
        this, tr("Disa Aktar"), QString(), tr("CSV (*.csv)"));
    if (yol.isEmpty()) return;

    auto* thread = new QThread(this);
    auto* worker = new DisaAktarmaServisi(depo_);
    worker->moveToThread(thread);

    auto* dialog = new QProgressDialog(
        tr("Yaziliyor..."), tr("Iptal"), 0, 0, this);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setMinimumDuration(300);

    connect(thread, &QThread::started, worker, [worker, yol] {
        worker->csvDisaAktar(yol);
    });

    connect(worker, &DisaAktarmaServisi::ilerleme, dialog,
            [dialog](qsizetype i, qsizetype n) {
                dialog->setMaximum(static_cast<int>(n));
                dialog->setValue(static_cast<int>(i));
            });

    connect(dialog, &QProgressDialog::canceled, worker,
            &DisaAktarmaServisi::durdur, Qt::DirectConnection);

    connect(worker, &DisaAktarmaServisi::tamamlandi, this,
            [this, dialog, thread, worker](qsizetype y) {
                QMessageBox::information(this, tr("Disa Aktarma"),
                    tr("%1 kayit yazildi.").arg(y));
                dialog->deleteLater();
                thread->quit();
                thread->wait();
                worker->deleteLater();
                thread->deleteLater();
            });

    thread->start();
}

void AnaPencere::gpaHesaplaTetikle() {
    auto* worker = new HesaplamaServisi(depo_, this);

    auto* dialog = new QProgressDialog(
        tr("GPA hesaplaniyor..."), tr("Iptal"), 0, 100, this);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setMinimumDuration(500);

    connect(worker, &HesaplamaServisi::ilerleme, dialog,
            [dialog](int i, int n) {
                dialog->setMaximum(n);
                dialog->setValue(i);
            });

    connect(dialog, &QProgressDialog::canceled, worker,
            &HesaplamaServisi::durdur, Qt::DirectConnection);

    connect(worker, &HesaplamaServisi::tamamlandi, this,
            [this, dialog, worker](QList<GPASonuc> sonuclar) {
                QMessageBox::information(this, tr("Hesaplama"),
                    tr("%1 ogrencinin GPA'si hesaplandi.")
                        .arg(sonuclar.size()));
                dialog->deleteLater();
                worker->deleteLater();
            });

    connect(worker, &HesaplamaServisi::iptalEdildi, this,
            [this, dialog, worker] {
                QMessageBox::warning(this, tr("Iptal"),
                    tr("Hesaplama iptal edildi."));
                dialog->deleteLater();
                worker->deleteLater();
            });

    worker->hesapla();
}

void AnaPencere::rastgeleVeriOlustur() {
    constexpr int N = 10000;
    for (int i = 1; i <= N; ++i) {
        depo_->ekle(100000 + i, std::make_shared<Ogrenci>(
            100000 + i,
            QString("Ogrenci_%1").arg(i),
            (i % 3 == 0) ? "Bilgisayar" :
            (i % 3 == 1) ? "Makine" : "Elektrik",
            2.0 + (i % 200) * 0.01));
    }
    tabloyuYenile();
    statusBar()->showMessage(
        tr("%1 rastgele kayit olusturuldu").arg(N), 3000);
}

void AnaPencere::tabloyuYenile() {
    auto kayitlar = depo_->tumKayitlar();

    model_->setRowCount(0);
    for (const auto& o : kayitlar) {
        QList<QStandardItem*> satir;
        satir << new QStandardItem(QString::number(o->numara()));
        satir << new QStandardItem(o->ad());
        satir << new QStandardItem(o->bolum());
        satir << new QStandardItem(QString::number(o->gpa(), 'f', 2));
        model_->appendRow(satir);
    }
    tablo_->resizeColumnsToContents();

    statusBar()->showMessage(
        tr("%1 ogrenci goruntuleniyor").arg(kayitlar.size()), 2000);
}

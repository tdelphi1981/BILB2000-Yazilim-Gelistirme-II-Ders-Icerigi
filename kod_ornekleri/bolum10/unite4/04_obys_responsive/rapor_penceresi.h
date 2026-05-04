/**
 * RaporPenceresi - OBYS toplu GPA hesaplama arayüzü
 *
 * QtConcurrent::mapped ile 10.000 öğrencinin GPA'sını paralel hesaplar;
 * QFutureWatcher ile ilerlemeyi QProgressDialog'a bağlar.
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 4 - Qt ile Çok Kanallı Programlama
 */
#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QWidget>
#include <QProgressDialog>
#include <QFutureWatcher>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QList>
#include <QString>
#include <QThread>
#include <QStatusBar>
#include <QMap>

struct DersNotu {
    QString dersKodu;
    int kredi;
    QString harf;
};

struct Ogrenci {
    int numara;
    QString ad;
    QString bolum;
    QList<DersNotu> notlar;
};

struct GPASonuc {
    int numara;
    QString ad;
    double gpa;
    int toplamKredi;
};

inline double harfPuan(const QString& harf) {
    static const QMap<QString, double> tablo = {
        {"AA", 4.0}, {"BA", 3.5}, {"BB", 3.0},
        {"CB", 2.5}, {"CC", 2.0}, {"DC", 1.5},
        {"DD", 1.0}, {"FF", 0.0}
    };
    return tablo.value(harf, 0.0);
}

inline GPASonuc hesaplaGPA(const Ogrenci& o) {
    QThread::msleep(2);  // Gerçekçi gecikme
    double kredi = 0, puan = 0;
    for (const auto& n : o.notlar) {
        kredi += n.kredi;
        puan += n.kredi * harfPuan(n.harf);
    }
    return {o.numara, o.ad,
            kredi > 0 ? puan / kredi : 0.0,
            static_cast<int>(kredi)};
}

class RaporPenceresi : public QMainWindow {
    Q_OBJECT
public:
    explicit RaporPenceresi(QList<Ogrenci> ogrenciler,
                            QWidget* parent = nullptr)
        : QMainWindow(parent)
        , ogrenciler_(std::move(ogrenciler))
    {
        uiKur();
        connect(raporButonu_, &QPushButton::clicked,
                this, &RaporPenceresi::raporuBaslat);
    }

private slots:
    void raporuBaslat() {
        raporButonu_->setEnabled(false);

        progressDialog_ = new QProgressDialog(
            "GPA hesaplanıyor...", "İptal",
            0, ogrenciler_.size(), this);
        progressDialog_->setWindowModality(Qt::WindowModal);
        progressDialog_->setMinimumDuration(200);

        watcher_ = new QFutureWatcher<GPASonuc>(this);

        connect(watcher_, &QFutureWatcher<GPASonuc>::progressRangeChanged,
                progressDialog_, &QProgressDialog::setRange);
        connect(watcher_, &QFutureWatcher<GPASonuc>::progressValueChanged,
                progressDialog_, &QProgressDialog::setValue);
        connect(watcher_, &QFutureWatcher<GPASonuc>::finished,
                this, &RaporPenceresi::raporBitti);
        connect(progressDialog_, &QProgressDialog::canceled,
                watcher_, &QFutureWatcher<GPASonuc>::cancel);

        QFuture<GPASonuc> gelecek =
            QtConcurrent::mapped(ogrenciler_, hesaplaGPA);
        watcher_->setFuture(gelecek);
    }

    void raporBitti() {
        raporButonu_->setEnabled(true);
        if (watcher_->isCanceled()) {
            statusBar()->showMessage("İptal edildi", 3000);
        } else {
            QList<GPASonuc> sonuclar = watcher_->future().results();
            tabloYenile(sonuclar);
            statusBar()->showMessage(
                QString("%1 öğrenci hesaplandı").arg(sonuclar.size()),
                3000);
        }
        progressDialog_->deleteLater();
        watcher_->deleteLater();
        progressDialog_ = nullptr;
        watcher_ = nullptr;
    }

private:
    void uiKur() {
        auto* merkez = new QWidget(this);
        auto* yerlesim = new QVBoxLayout(merkez);
        raporButonu_ = new QPushButton(
            "Tüm Dönem Raporunu Hesapla", this);
        yerlesim->addWidget(raporButonu_);
        tablo_ = new QTableView(this);
        model_ = new QStandardItemModel(0, 4, this);
        model_->setHorizontalHeaderLabels(
            {"Numara", "Ad", "GPA", "Kredi"});
        tablo_->setModel(model_);
        yerlesim->addWidget(tablo_);
        setCentralWidget(merkez);
        resize(700, 500);
        setWindowTitle("OBYS - Dönem Raporu (Paralel)");
    }

    void tabloYenile(const QList<GPASonuc>& sonuclar) {
        model_->removeRows(0, model_->rowCount());
        for (const auto& s : sonuclar) {
            QList<QStandardItem*> satir;
            satir << new QStandardItem(QString::number(s.numara));
            satir << new QStandardItem(s.ad);
            satir << new QStandardItem(
                QString::number(s.gpa, 'f', 2));
            satir << new QStandardItem(
                QString::number(s.toplamKredi));
            model_->appendRow(satir);
        }
        tablo_->resizeColumnsToContents();
    }

    QList<Ogrenci> ogrenciler_;
    QPushButton* raporButonu_;
    QTableView* tablo_;
    QStandardItemModel* model_;
    QFutureWatcher<GPASonuc>* watcher_ = nullptr;
    QProgressDialog* progressDialog_ = nullptr;
};

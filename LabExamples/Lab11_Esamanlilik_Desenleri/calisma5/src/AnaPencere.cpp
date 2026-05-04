#include "AnaPencere.h"
#include "Worker.h"

#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

AnaPencere::AnaPencere(QWidget* parent) : QMainWindow(parent) {
    auto* w = new QWidget(this);
    auto* y = new QVBoxLayout(w);

    iceButonu_ = new QPushButton(tr("CSV Ice Aktar..."), this);
    etiket_ = new QLabel(tr("Hazir"), this);

    y->addWidget(iceButonu_);
    y->addWidget(etiket_);
    y->addStretch();

    setCentralWidget(w);
    resize(400, 200);
    setWindowTitle("Lab 11 - Calisma 5");

    connect(iceButonu_, &QPushButton::clicked,
            this, &AnaPencere::csvIceAktarTetikle);
}

void AnaPencere::csvIceAktarTetikle() {
    QString yol = QFileDialog::getOpenFileName(
        this, tr("CSV Sec"), QString(), tr("CSV (*.csv)"));
    if (yol.isEmpty()) return;

    // GOREV 1: Yeni QThread olustur
    //   - auto* thread = new QThread(this);

    // GOREV 2: Worker olustur ve thread'e tasi
    //   - auto* worker = new Worker;
    //   - worker->moveToThread(thread);

    // GOREV 3: QProgressDialog kur
    //   - "Yukleniyor...", "Iptal", 0, 0, this
    //   - WindowModal, minimumDuration=500

    // GOREV 4: thread::started -> worker::calistir baglantisi
    //   - lambda ile yol parametresini gec

    // GOREV 5: ilerleme sinyali -> dialog::setMaximum/setValue

    // GOREV 6: dialog::canceled -> worker::durdur (DirectConnection!)

    // GOREV 7: tamamlandi sinyali -> mesaj kutusu + temizlik
    //   - dialog->deleteLater()
    //   - thread->quit(); thread->wait();
    //   - worker->deleteLater(); thread->deleteLater();

    // GOREV 8: thread->start();
}

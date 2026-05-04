/**
 * HesaplamaController - Worker'ı yöneten sınıf
 *
 * Bu sınıf UI izleğinde yaşar; Worker'ı QThread'e taşır ve
 * UI ile Worker arasında sinyal köprüsü kurar.
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 4 - Qt ile Çok Kanallı Programlama
 */
#pragma once

#include <QObject>
#include <QThread>
#include "hesaplama_worker.h"

class HesaplamaController : public QObject {
    Q_OBJECT
public:
    explicit HesaplamaController(QObject* parent = nullptr)
        : QObject(parent)
    {
        izlek_ = new QThread(this);
        worker_ = new HesaplamaWorker();
        worker_->moveToThread(izlek_);

        // UI -> Worker (Queued sayesinde güvenli)
        connect(this, &HesaplamaController::baslatTalep,
                worker_, &HesaplamaWorker::hesaplamayiBaslat);
        connect(this, &HesaplamaController::iptalTalep,
                worker_, &HesaplamaWorker::iptalEt);

        // Worker -> UI (forward)
        connect(worker_, &HesaplamaWorker::ilerlemeDegisti,
                this, &HesaplamaController::ilerlemeDegisti);
        connect(worker_, &HesaplamaWorker::sonucHazir,
                this, &HesaplamaController::sonucHazir);
        connect(worker_, &HesaplamaWorker::tumSonuclarHazir,
                this, &HesaplamaController::tumSonuclarHazir);
        connect(worker_, &HesaplamaWorker::iptalEdildi,
                this, &HesaplamaController::iptalEdildi);

        // Temizleme
        connect(izlek_, &QThread::finished,
                worker_, &QObject::deleteLater);

        izlek_->start();
    }

    ~HesaplamaController() {
        izlek_->quit();
        izlek_->wait();
    }

    void hesaplamayiBaslat(const QVector<HesaplamaVerisi>& veriler) {
        emit baslatTalep(veriler);
    }

    void iptal() { emit iptalTalep(); }

signals:
    // İç sinyaller (UI'den worker'a)
    void baslatTalep(QVector<HesaplamaVerisi> veriler);
    void iptalTalep();

    // Dış sinyaller (worker'dan UI'ye)
    void ilerlemeDegisti(int yapilan, int toplam);
    void sonucHazir(int id, double sonuc);
    void tumSonuclarHazir();
    void iptalEdildi();

private:
    QThread* izlek_;
    HesaplamaWorker* worker_;
};

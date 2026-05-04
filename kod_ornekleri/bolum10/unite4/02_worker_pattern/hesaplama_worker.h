/**
 * HesaplamaWorker - Worker Pattern için işçi sınıfı
 *
 * Bu sınıf moveToThread ile arka plan izleğine taşınır.
 * UI ile yalnızca sinyal/slot üzerinden haberleşir.
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 4 - Qt ile Çok Kanallı Programlama
 */
#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QCoreApplication>
#include <QThread>
#include <atomic>

struct HesaplamaVerisi {
    int id;
    double girdi;
};

class HesaplamaWorker : public QObject {
    Q_OBJECT
public:
    explicit HesaplamaWorker(QObject* parent = nullptr)
        : QObject(parent) {}

public slots:
    // UI'den gelen tetikleyici
    void hesaplamayiBaslat(QVector<HesaplamaVerisi> veriler) {
        iptalIstendi_.store(false);
        for (int i = 0; i < veriler.size(); ++i) {
            // UI'den gelen iptalEt slot'unun işlenmesine izin ver
            QCoreApplication::processEvents();
            if (iptalIstendi_.load()) {
                emit iptalEdildi();
                return;
            }

            // Hesaplama (simüle edilmiş)
            QThread::msleep(50);
            double sonuc = veriler[i].girdi * veriler[i].girdi;

            emit ilerlemeDegisti(i + 1, veriler.size());
            emit sonucHazir(veriler[i].id, sonuc);
        }
        emit tumSonuclarHazir();
    }

    void iptalEt() { iptalIstendi_.store(true); }

signals:
    void ilerlemeDegisti(int yapilan, int toplam);
    void sonucHazir(int id, double sonuc);
    void tumSonuclarHazir();
    void iptalEdildi();

private:
    std::atomic<bool> iptalIstendi_{false};
};

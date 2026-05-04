#pragma once
#include "Depo.h"
#include "Ogrenci.h"
#include <QFutureWatcher>
#include <QList>
#include <QObject>
#include <memory>

/**
 * GPA hesaplama sonucu
 */
struct GPASonuc {
    int numara = 0;
    QString ad;
    double gpa = 0.0;
};

Q_DECLARE_METATYPE(GPASonuc)

/**
 * HesaplamaServisi - QtConcurrent::mapped ile GPA hesaplama
 */
class HesaplamaServisi : public QObject {
    Q_OBJECT
public:
    explicit HesaplamaServisi(std::shared_ptr<Depo<Ogrenci>> depo,
                               QObject* parent = nullptr);

public slots:
    void hesapla();
    void durdur();

signals:
    void ilerleme(int islenen, int toplam);
    void tamamlandi(QList<GPASonuc> sonuclar);
    void iptalEdildi();

private:
    std::shared_ptr<Depo<Ogrenci>> depo_;
    QFutureWatcher<GPASonuc>* watcher_ = nullptr;
};

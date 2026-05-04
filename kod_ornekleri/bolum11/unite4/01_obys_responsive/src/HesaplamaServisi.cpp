#include "HesaplamaServisi.h"
#include <QtConcurrent>
#include <QThread>

static GPASonuc hesaplaTekOgrenci(const std::shared_ptr<Ogrenci>& o) {
    // Gerçekçi gecikme simülasyonu (asıl projede ders notları üzerinde hesap)
    QThread::msleep(2);

    GPASonuc s;
    s.numara = o->numara();
    s.ad = o->ad();
    // Basit hesap: var olan gpa veya numara modülü
    s.gpa = (o->gpa() > 0.0) ? o->gpa() : (2.0 + (o->numara() % 200) * 0.01);
    return s;
}

HesaplamaServisi::HesaplamaServisi(std::shared_ptr<Depo<Ogrenci>> depo,
                                     QObject* parent)
    : QObject(parent), depo_(std::move(depo)) {}

void HesaplamaServisi::hesapla() {
    if (watcher_) {
        // Önceki iş hâlâ devam ediyor
        return;
    }

    auto kayitlar = depo_->tumKayitlar();
    QList<std::shared_ptr<Ogrenci>> liste(kayitlar.begin(), kayitlar.end());

    if (liste.isEmpty()) {
        emit tamamlandi({});
        return;
    }

    watcher_ = new QFutureWatcher<GPASonuc>(this);

    connect(watcher_, &QFutureWatcher<GPASonuc>::progressValueChanged,
            this, [this](int v) {
        emit ilerleme(v, watcher_->progressMaximum());
    });

    connect(watcher_, &QFutureWatcher<GPASonuc>::finished,
            this, [this] {
        if (watcher_->isCanceled()) {
            emit iptalEdildi();
        } else {
            QList<GPASonuc> sonuclar = watcher_->future().results();
            emit tamamlandi(sonuclar);
        }
        watcher_->deleteLater();
        watcher_ = nullptr;
    });

    auto gelecek = QtConcurrent::mapped(liste, hesaplaTekOgrenci);
    watcher_->setFuture(gelecek);
}

void HesaplamaServisi::durdur() {
    if (watcher_) {
        watcher_->cancel();
    }
}

#pragma once
#include "Depo.h"
#include "Ogrenci.h"
#include <QObject>
#include <QString>
#include <memory>
#include <stop_token>

/**
 * IceAktarmaServisi
 *
 * CSV dosyasından öğrenci verisi okur ve thread-safe Depo'ya ekler.
 * QObject türevi; Worker pattern'ile başka bir QThread'te yaşar.
 */
class IceAktarmaServisi : public QObject {
    Q_OBJECT
public:
    explicit IceAktarmaServisi(std::shared_ptr<Depo<Ogrenci>> depo,
                                QObject* parent = nullptr);

public slots:
    void csvIceAktar(const QString& yol);
    void durdur();

signals:
    void ilerleme(qsizetype islenen, qsizetype toplam);
    void durum(const QString& mesaj);
    void hata(const QString& mesaj);
    void tamamlandi(qsizetype basariliKayit, qsizetype hataKayit);

private:
    std::shared_ptr<Depo<Ogrenci>> depo_;
    std::stop_source stop_;
};

#pragma once
#include "Depo.h"
#include "Ogrenci.h"
#include <QObject>
#include <QString>
#include <memory>
#include <stop_token>

/**
 * DisaAktarmaServisi
 *
 * Thread-safe Depo'dan öğrencileri CSV dosyasına yazar.
 */
class DisaAktarmaServisi : public QObject {
    Q_OBJECT
public:
    explicit DisaAktarmaServisi(std::shared_ptr<Depo<Ogrenci>> depo,
                                 QObject* parent = nullptr);

public slots:
    void csvDisaAktar(const QString& yol);
    void durdur();

signals:
    void ilerleme(qsizetype yazilan, qsizetype toplam);
    void durum(const QString& mesaj);
    void hata(const QString& mesaj);
    void tamamlandi(qsizetype yazilan);

private:
    std::shared_ptr<Depo<Ogrenci>> depo_;
    std::stop_source stop_;
};

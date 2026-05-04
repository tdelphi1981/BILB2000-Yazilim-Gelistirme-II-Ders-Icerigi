#pragma once
#include <QObject>
#include <QString>
#include <stop_token>

class Worker : public QObject {
    Q_OBJECT
public slots:
    void calistir(const QString& yol);

    // GOREV 1: durdur slot'unu yaz - stop_.request_stop() cagirsin

signals:
    void ilerleme(qsizetype islenen, qsizetype toplam);
    void tamamlandi(qsizetype basarili);
    void hata(const QString& mesaj);

private:
    std::stop_source stop_;
};

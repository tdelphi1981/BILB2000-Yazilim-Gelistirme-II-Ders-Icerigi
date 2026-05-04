/**
 * Lab 12 - Çalışma 2: Qt TCP İstemci sınıfı (header)
 *
 * QTcpSocket'i sinyal/slot ile kullanan basit bir istemci.
 * Çalışma 1'deki POSIX echo sunucusuna bağlanır.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>

class QTcpSocket;

class Istemci : public QObject {
    Q_OBJECT

public:
    explicit Istemci(QObject* parent = nullptr);

    // Sunucuya bağlanmak için public slot olabilir.
    // GOREV 1: connectToHost cagrisi yapan baglan() slot'unu yaz.
    //          Imza: void baglan(const QString& host, quint16 port);
    //          Govde: socket_->connectToHost(host, port);

public slots:
    void baglan(const QString& host, quint16 port);

    // Stdin'den satır gönderen slot
    void mesajGonder(const QByteArray& mesaj);

signals:
    void baglandi();
    void koptu();
    void mesajGeldi(const QByteArray& veri);
    void hataOlustu(const QString& aciklama);

private slots:
    // GOREV 2: readyRead sinyalinin slot'u. Imza:
    //          void okumaHazirSlot();
    //          Govde: socket_->readAll() ile tum baytlari al,
    //                 mesajGeldi sinyali ile yukari yolla.
    void okumaHazirSlot();

private:
    QTcpSocket* socket_;  // GOREV 1 icinde new QTcpSocket(this) ile olusturulacak
};

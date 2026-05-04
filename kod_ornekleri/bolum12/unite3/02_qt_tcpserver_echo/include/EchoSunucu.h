/**
 * EchoSunucu.h — QTcpServer tabanlı echo sunucu sınıfı
 *
 * Bölüm 12 Ünite 3 Konu 2
 */

#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class EchoSunucu : public QObject {
    Q_OBJECT
public:
    explicit EchoSunucu(QObject* anaNesne = nullptr);

    // Belirtilen porta bağlanıp dinlemeye başla
    bool dinlemeBaslat(quint16 port);

private slots:
    // QTcpServer yeni bağlantı geldiğinde tetiklenir
    void yeniBaglanti();

    // Her istemciden veri gelince tetiklenir
    void istemciVeri();

    // İstemci bağlantısı koptuğunda tetiklenir
    void istemciAyrildi();

private:
    QTcpServer* m_sunucu;
};

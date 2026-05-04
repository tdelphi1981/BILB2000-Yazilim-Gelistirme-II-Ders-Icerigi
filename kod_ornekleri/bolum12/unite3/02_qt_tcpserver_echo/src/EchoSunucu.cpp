/**
 * EchoSunucu.cpp — QTcpServer tabanlı echo sunucu gerçekleştirimi
 *
 * Bölüm 12 Ünite 3 Konu 2
 */

#include "EchoSunucu.h"

#include <QDebug>
#include <QHostAddress>

EchoSunucu::EchoSunucu(QObject* anaNesne)
    : QObject(anaNesne), m_sunucu(new QTcpServer(this)) {
    // Yeni bağlantı geldiğinde yeniBaglanti() slotunu çağır
    connect(m_sunucu, &QTcpServer::newConnection,
            this, &EchoSunucu::yeniBaglanti);
}

bool EchoSunucu::dinlemeBaslat(quint16 port) {
    if (!m_sunucu->listen(QHostAddress::Any, port)) {
        qCritical() << "Dinleme başlatılamadı:" << m_sunucu->errorString();
        return false;
    }
    qInfo() << "Echo sunucu" << port << "portunda dinliyor.";
    return true;
}

void EchoSunucu::yeniBaglanti() {
    // Kuyrukta bekleyen tüm yeni bağlantıları al
    while (m_sunucu->hasPendingConnections()) {
        QTcpSocket* istemci = m_sunucu->nextPendingConnection();

        qInfo() << "Bağlandı:" << istemci->peerAddress().toString()
                << ":" << istemci->peerPort();

        // İstemcinin kendi sinyallerini ilgili slotlara bağla
        // sender() içinde hangi soketin tetiklediğini bulmak için
        // doğrudan slotu kullanıyoruz
        connect(istemci, &QTcpSocket::readyRead,
                this, &EchoSunucu::istemciVeri);
        connect(istemci, &QTcpSocket::disconnected,
                this, &EchoSunucu::istemciAyrildi);
    }
}

void EchoSunucu::istemciVeri() {
    // sender() bu slotu tetikleyen QObject'i döner
    auto* istemci = qobject_cast<QTcpSocket*>(sender());
    if (!istemci) return;

    // Tüm bekleyen veriyi oku ve aynısını geri yaz (echo)
    QByteArray gelenVeri = istemci->readAll();
    qInfo() << "Echo:" << gelenVeri.size() << "bayt yansıtılıyor.";
    istemci->write(gelenVeri);
}

void EchoSunucu::istemciAyrildi() {
    auto* istemci = qobject_cast<QTcpSocket*>(sender());
    if (!istemci) return;

    qInfo() << "Ayrıldı:" << istemci->peerAddress().toString();
    // deleteLater: event loop bir sonraki tura geçtiğinde nesneyi siler
    istemci->deleteLater();
}

/**
 * Istemci.cpp — QTcpSocket sarmalayıcı sınıf gerçekleştirimi
 *
 * Bölüm 12 Ünite 3 Konu 1
 */

#include "Istemci.h"

#include <QDebug>

Istemci::Istemci(QObject* anaNesne)
    : QObject(anaNesne), m_soket(new QTcpSocket(this)) {
    // Yeni connect syntax: derleme zamanı tip kontrolü yapar
    connect(m_soket, &QTcpSocket::connected,
            this, &Istemci::baglandi);
    connect(m_soket, &QTcpSocket::disconnected,
            this, &Istemci::baglantiKesildi);
    connect(m_soket, &QTcpSocket::readyRead,
            this, &Istemci::verigeldi);
    connect(m_soket, &QTcpSocket::errorOccurred,
            this, &Istemci::hataIsle);
}

void Istemci::baglan(const QString& sunucu, quint16 port) {
    qInfo() << "Bağlanılıyor:" << sunucu << ":" << port;
    m_soket->connectToHost(sunucu, port);
}

void Istemci::satirGonder(const QString& satir) {
    if (m_soket->state() != QAbstractSocket::ConnectedState) {
        emit hataOlustu(QStringLiteral("Soket bağlı değil."));
        return;
    }
    QByteArray paket = satir.toUtf8();
    paket.append('\n');
    m_soket->write(paket);
}

void Istemci::kapat() {
    if (m_soket->state() != QAbstractSocket::UnconnectedState) {
        m_soket->disconnectFromHost();
    }
}

void Istemci::verigeldi() {
    // Gelen tüm veriyi tampona ekle
    m_tampon.append(m_soket->readAll());

    // Tamponda tam satırlar varsa (yani '\n' içeriyorsa) bunları
    // birer birer çıkar ve sinyal yay
    int idx;
    while ((idx = m_tampon.indexOf('\n')) != -1) {
        QByteArray satirBaytlar = m_tampon.left(idx);
        m_tampon.remove(0, idx + 1);
        // CR-LF normalize: sondaki '\r' karakterini at
        if (!satirBaytlar.isEmpty() && satirBaytlar.endsWith('\r')) {
            satirBaytlar.chop(1);
        }
        emit satirAlindi(QString::fromUtf8(satirBaytlar));
    }
}

void Istemci::hataIsle(QAbstractSocket::SocketError hata) {
    Q_UNUSED(hata);
    emit hataOlustu(m_soket->errorString());
}

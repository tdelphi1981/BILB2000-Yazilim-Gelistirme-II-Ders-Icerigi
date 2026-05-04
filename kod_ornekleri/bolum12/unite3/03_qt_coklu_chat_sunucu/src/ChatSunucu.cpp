/**
 * ChatSunucu.cpp — Çoklu istemcili chat sunucu gerçekleştirimi
 *
 * Bölüm 12 Ünite 3 Konu 3
 */

#include "ChatSunucu.h"

#include <QDebug>
#include <QHostAddress>

ChatSunucu::ChatSunucu(QObject* anaNesne)
    : QObject(anaNesne), m_sunucu(new QTcpServer(this)) {
    connect(m_sunucu, &QTcpServer::newConnection,
            this, &ChatSunucu::yeniBaglanti);
}

bool ChatSunucu::dinlemeBaslat(quint16 port) {
    if (!m_sunucu->listen(QHostAddress::Any, port)) {
        qCritical() << "Dinleme başlatılamadı:" << m_sunucu->errorString();
        return false;
    }
    qInfo() << "Chat sunucu" << port << "portunda dinliyor.";
    return true;
}

void ChatSunucu::yeniBaglanti() {
    while (m_sunucu->hasPendingConnections()) {
        QTcpSocket* istemci = m_sunucu->nextPendingConnection();

        // Otomatik takma ad ata: kullanici1, kullanici2, ...
        QString takmaAd = QStringLiteral("kullanici%1").arg(m_sonrakiNo++);
        m_takmaAd[istemci] = takmaAd;
        m_tampon[istemci]  = QByteArray();
        m_istemciler.append(istemci);

        qInfo() << "Bağlandı:" << takmaAd
                << "(" << istemci->peerAddress().toString() << ")";

        // İstemciye hoş geldin mesajı
        QByteArray hosgeldin = QStringLiteral("[sunucu] Hoş geldin %1\n")
                                   .arg(takmaAd).toUtf8();
        istemci->write(hosgeldin);

        // Diğerlerine duyur
        QByteArray duyuru = QStringLiteral("[sunucu] %1 odaya katıldı.\n")
                                .arg(takmaAd).toUtf8();
        yayinla(duyuru, istemci);

        connect(istemci, &QTcpSocket::readyRead,
                this, &ChatSunucu::istemciVeri);
        connect(istemci, &QTcpSocket::disconnected,
                this, &ChatSunucu::istemciAyrildi);
    }
}

void ChatSunucu::istemciVeri() {
    auto* istemci = qobject_cast<QTcpSocket*>(sender());
    if (!istemci) return;

    // Tampona ekle
    m_tampon[istemci].append(istemci->readAll());

    // Tam satırları işle
    QByteArray& tampon = m_tampon[istemci];
    int idx;
    while ((idx = tampon.indexOf('\n')) != -1) {
        QByteArray satir = tampon.left(idx);
        tampon.remove(0, idx + 1);
        if (!satir.isEmpty() && satir.endsWith('\r')) {
            satir.chop(1);
        }

        const QString takmaAd = m_takmaAd.value(istemci);
        QString metin = QString::fromUtf8(satir);

        qInfo() << takmaAd << ":" << metin;

        QByteArray paket = QStringLiteral("<%1> %2\n")
                               .arg(takmaAd, metin).toUtf8();
        // Mesajı gönderene de yansıtmadan diğerlerine yay
        yayinla(paket, istemci);
    }
}

void ChatSunucu::istemciAyrildi() {
    auto* istemci = qobject_cast<QTcpSocket*>(sender());
    if (!istemci) return;

    const QString takmaAd = m_takmaAd.value(istemci);
    qInfo() << "Ayrıldı:" << takmaAd;

    m_istemciler.removeAll(istemci);
    m_takmaAd.remove(istemci);
    m_tampon.remove(istemci);

    QByteArray duyuru = QStringLiteral("[sunucu] %1 odadan ayrıldı.\n")
                            .arg(takmaAd).toUtf8();
    yayinla(duyuru, nullptr);

    // Event loop güvenli silme
    istemci->deleteLater();
}

void ChatSunucu::yayinla(const QByteArray& mesaj, QTcpSocket* haricTutulan) {
    for (QTcpSocket* alici : m_istemciler) {
        if (alici == haricTutulan) continue;
        if (alici->state() != QAbstractSocket::ConnectedState) continue;
        alici->write(mesaj);
    }
}

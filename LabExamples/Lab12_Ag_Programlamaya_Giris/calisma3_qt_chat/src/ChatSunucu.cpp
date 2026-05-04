/**
 * Lab 12 - Çalışma 3: Qt TCP Chat Sunucusu (kaynak)
 *
 * GOREV-1: yeniBaglanti slot'u (newConnection -> nextPendingConnection)
 * GOREV-2: okumaHazir slot'u (readAll + cerceveCoz)
 * GOREV-3: tumIstemcilereYayinla (broadcast)
 */

#include "ChatSunucu.h"
#include "cerceveleme.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

ChatSunucu::ChatSunucu(QObject* parent)
    : QObject(parent)
    , server_(new QTcpServer(this))
{
    connect(server_, &QTcpServer::newConnection,
            this, &ChatSunucu::yeniBaglanti);
}

bool ChatSunucu::baslat(quint16 port)
{
    if (!server_->listen(QHostAddress::Any, port)) {
        qWarning() << "Sunucu baslatilamadi:" << server_->errorString();
        return false;
    }
    qInfo() << "Chat sunucusu" << port << "portunda dinliyor (length-prefix).";
    return true;
}

void ChatSunucu::yeniBaglanti()
{
    // ============================================================
    // GOREV 1: Bekleyen baglantilari kabul et ve listeye ekle.
    //
    // while (server_->hasPendingConnections()) {
    //     QTcpSocket* istemci = server_->nextPendingConnection();
    //     istemciler_.insert(istemci, QByteArray{});
    //
    //     connect(istemci, &QTcpSocket::readyRead,
    //             this, &ChatSunucu::okumaHazir);
    //     connect(istemci, &QTcpSocket::disconnected,
    //             this, &ChatSunucu::baglantiKesildi);
    //
    //     qInfo() << "Yeni istemci:" << istemci->peerAddress().toString()
    //             << ":" << istemci->peerPort();
    //     emit istemciKatildi(istemci);
    // }
    // ============================================================

    // GOREV 1 buraya...
}

void ChatSunucu::okumaHazir()
{
    // ============================================================
    // GOREV 2: Sender soketten gelen baytlari ilgili istemcinin
    //          akumulator buffer'ina ekle ve tam cerceveleri cikar.
    //
    // QTcpSocket* gonderen = qobject_cast<QTcpSocket*>(sender());
    // if (!gonderen) return;
    //
    // auto it = istemciler_.find(gonderen);
    // if (it == istemciler_.end()) return;
    //
    // QByteArray& buffer = it.value();
    // buffer.append(gonderen->readAll());
    //
    // try {
    //     QList<QByteArray> cerceveler = cerceveleme::cerceveCoz(buffer);
    //     for (const QByteArray& payload : cerceveler) {
    //         qInfo() << "Mesaj alindi (" << payload.size() << "bayt):"
    //                 << QString::fromUtf8(payload);
    //         tumIstemcilereYayinla(payload, gonderen);
    //     }
    // } catch (const std::exception& e) {
    //     qWarning() << "Protokol hatasi, baglanti kesiliyor:" << e.what();
    //     gonderen->disconnectFromHost();
    // }
    // ============================================================

    // GOREV 2 buraya...
}

void ChatSunucu::baglantiKesildi()
{
    QTcpSocket* istemci = qobject_cast<QTcpSocket*>(sender());
    if (!istemci) return;
    istemciler_.remove(istemci);
    qInfo() << "Istemci ayrildi:" << istemci->peerAddress().toString();
    emit istemciAyrildi(istemci);
    istemci->deleteLater();
}

void ChatSunucu::tumIstemcilereYayinla(const QByteArray& payload,
                                        QTcpSocket* gonderen)
{
    // ============================================================
    // GOREV 3: Tum istemciler_ uzerinde dolas, gonderen DAHIL
    //          herkese cerceveSar(payload) yaz. (Echo degil chat:
    //          gonderen kendi mesajini gormek istemiyorsa atlanabilir
    //          ama klasik chat'te kendi yazdigini da gorur.)
    //
    // QByteArray cerceve = cerceveleme::cerceveSar(payload);
    // for (auto it = istemciler_.constBegin();
    //      it != istemciler_.constEnd(); ++it) {
    //     QTcpSocket* alici = it.key();
    //     // Gonderen DAHIL herkese yolla (chat: kendi mesajinda da gor):
    //     alici->write(cerceve);
    // }
    // emit mesajYayinlandi(payload);
    // ============================================================

    // GOREV 3 buraya...
    Q_UNUSED(payload);
    Q_UNUSED(gonderen);
}

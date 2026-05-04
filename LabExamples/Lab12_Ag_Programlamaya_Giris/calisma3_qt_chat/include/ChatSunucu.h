/**
 * Lab 12 - Çalışma 3: Qt TCP Chat Sunucusu (header)
 *
 * QTcpServer + length-prefix çerçeveleme ile çoklu istemci destekli
 * basit bir broadcast chat sunucusu. Her istemcinin gonderdigi mesaji
 * diger TUM istemcilere yayinlar (echo degil).
 */

#pragma once

#include <QObject>
#include <QHash>
#include <QByteArray>

class QTcpServer;
class QTcpSocket;

class ChatSunucu : public QObject {
    Q_OBJECT

public:
    explicit ChatSunucu(QObject* parent = nullptr);

    bool baslat(quint16 port);

signals:
    void istemciKatildi(QTcpSocket* istemci);
    void istemciAyrildi(QTcpSocket* istemci);
    void mesajYayinlandi(const QByteArray& payload);

private slots:
    // GOREV 1: yeniBaglanti slot'u. QTcpServer::newConnection sinyalinden
    //          tetiklenir. server_->nextPendingConnection() ile soket al,
    //          istemciler_ map'ine bos QByteArray buffer ile ekle.
    //          readyRead ve disconnected sinyallerini bagla.
    void yeniBaglanti();

    // GOREV 2: okumaHazir slot'u. Sender soketten readAll() ile baytlari
    //          al, buffer'a ekle, cerceveleme::cerceveCoz ile tam cerceveleri
    //          cikar.
    void okumaHazir();

    // istemciAyrildi sinyaliyle haritadan sil.
    void baglantiKesildi();

private:
    // GOREV 3: tumIstemcilereYayinla helper'i. Tum istemciler_ uzerinde
    //          dolas, gonderen haric herkese cerceveSar(payload) yaz.
    void tumIstemcilereYayinla(const QByteArray& payload, QTcpSocket* gonderen);

    QTcpServer* server_;

    // Her istemcinin biriktirilmis kismi verisini tutan map.
    // QHash<istemci_pointer, akumulator_buffer>
    QHash<QTcpSocket*, QByteArray> istemciler_;
};

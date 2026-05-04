/**
 * Lab 12 - Çalışma 2: Qt TCP İstemci sınıfı (kaynak)
 *
 * GOREV-1: socket_ olusturma + connect() bagli sinyaller + baglan() govde.
 * GOREV-2: readyRead slot'unun govdesi (readAll + emit mesajGeldi).
 * GOREV-3: mesajGonder slot'unu doldur (socket_->write).
 */

#include "Istemci.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>

Istemci::Istemci(QObject* parent)
    : QObject(parent)
    , socket_(nullptr)
{
    // ============================================================
    // GOREV 1 (a): socket_ uyesini olustur ve sinyalleri bagla.
    //
    // socket_ = new QTcpSocket(this);
    //
    // connect(socket_, &QTcpSocket::connected,
    //         this, &Istemci::baglandi);
    //
    // connect(socket_, &QTcpSocket::disconnected,
    //         this, &Istemci::koptu);
    //
    // connect(socket_, &QTcpSocket::readyRead,
    //         this, &Istemci::okumaHazirSlot);
    //
    // connect(socket_, &QTcpSocket::errorOccurred,
    //         this, [this](QAbstractSocket::SocketError) {
    //             emit hataOlustu(socket_->errorString());
    //         });
    //
    // UYARI: ESKI SIGNAL/SLOT macro'larini KULLANMA.
    //        Yeni connect syntax (uye fonksiyon isaretcisi) kullan.
    // ============================================================

    // GOREV 1 (a) buraya...
}

void Istemci::baglan(const QString& host, quint16 port)
{
    // ============================================================
    // GOREV 1 (b): socket_->connectToHost(host, port);
    //
    // connectToHost ASENKRON: hemen geri doner. Baglanti
    // kuruldugunda 'connected' sinyali emit edilir; biz onu
    // baglandi() sinyalimize bagladik.
    // ============================================================

    // GOREV 1 (b) buraya...
}

void Istemci::okumaHazirSlot()
{
    // ============================================================
    // GOREV 2: socket_->readAll() ile gelen baytlari al ve
    //          mesajGeldi sinyali ile yukari yolla.
    //
    // QByteArray veri = socket_->readAll();
    // emit mesajGeldi(veri);
    //
    // NOT: TCP bayt akisi oldugu icin readyRead her tetiklendiginde
    //      tam mesajin geldigi GARANTI EDILEMEZ. Cerceveleme bu lab'in
    //      Calisma 3'unde isleniyor; burada echo sunucusuyla ham bayt
    //      olarak yazdirmak yeterli.
    // ============================================================

    // GOREV 2 buraya...
}

void Istemci::mesajGonder(const QByteArray& mesaj)
{
    // ============================================================
    // GOREV 3: socket_->write(mesaj) ile sokete bayt yaz.
    //
    // if (socket_->state() != QAbstractSocket::ConnectedState) {
    //     emit hataOlustu("Baglanti yok");
    //     return;
    // }
    // socket_->write(mesaj);
    //
    // NOT: write() asenkrondur; veriyi Qt'nin gonderim tamponuna
    //      kopyalayip hemen geri doner.
    // ============================================================

    // GOREV 3 buraya...
}

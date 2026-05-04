/**
 * ChatSunucu.h — Çoklu istemcili chat sunucu sınıf bildirimi
 *
 * Bağlanan tüm istemcileri bir liste içinde tutar. Bir istemciden
 * gelen mesajı diğer tüm istemcilere yayınlar (broadcast).
 *
 * Bölüm 12 Ünite 3 Konu 3
 */

#pragma once

#include <QHash>
#include <QList>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class ChatSunucu : public QObject {
    Q_OBJECT
public:
    explicit ChatSunucu(QObject* anaNesne = nullptr);

    bool dinlemeBaslat(quint16 port);

private slots:
    void yeniBaglanti();
    void istemciVeri();
    void istemciAyrildi();

private:
    // Bir mesajı belirtilen istemci hariç tüm bağlı istemcilere yayar
    void yayinla(const QByteArray& mesaj, QTcpSocket* haricTutulan);

    QTcpServer*        m_sunucu;
    QList<QTcpSocket*> m_istemciler;
    // Soket -> takma ad (kullanıcı adı) eşleşmesi
    QHash<QTcpSocket*, QString> m_takmaAd;
    // Soket -> satır tamponu eşleşmesi
    QHash<QTcpSocket*, QByteArray> m_tampon;
    int m_sonrakiNo = 1;
};

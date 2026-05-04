/**
 * Istemci.h — QTcpSocket sarmalayıcı sınıf bildirimi
 *
 * Bu sınıf bir TCP sunucusuna bağlanır, satır bazlı mesaj alıp
 * gönderir ve sinyal/slot mekanizmasıyla olayları dış dünyaya
 * bildirir.
 *
 * Bölüm 12 Ünite 3 Konu 1
 */

#pragma once

#include <QHostAddress>
#include <QObject>
#include <QString>
#include <QTcpSocket>

class Istemci : public QObject {
    Q_OBJECT
public:
    explicit Istemci(QObject* anaNesne = nullptr);

    // Sunucuya bağlan ("localhost", 9000 gibi)
    void baglan(const QString& sunucu, quint16 port);

    // Bir satır gönder (otomatik olarak '\n' eklenir)
    void satirGonder(const QString& satir);

    // Bağlantıyı kapat
    void kapat();

signals:
    // Bağlantı olayları
    void baglandi();
    void baglantiKesildi();
    void hataOlustu(const QString& mesaj);

    // Sunucudan tam bir satır geldi
    void satirAlindi(const QString& satir);

private slots:
    // QTcpSocket'tan veri gelince çağrılır, satır tamponuna ekler
    void verigeldi();
    void hataIsle(QAbstractSocket::SocketError hata);

private:
    QTcpSocket* m_soket;
    QByteArray  m_tampon;  // Satır tamamlanana kadar veri biriktir
};

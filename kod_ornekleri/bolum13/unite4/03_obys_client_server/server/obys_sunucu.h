/**
 * ObysSunucu - QTcpServer Tabanlı OBYS Sunucusu
 *
 * Bölüm 13 mini projesinin sunucu tarafı. QTcpServer üzerinden
 * gelen istemci bağlantılarını kabul eder, basit JSON komutlarını
 * (LOGIN, OGRENCI_LISTE) işler ve cevap döner.
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 4 - Güvenlik ve Tasarım
 */
#pragma once

#include <QHash>
#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

struct Ogrenci {
    QString numara;
    QString adSoyad;
    double gano;
};

class ObysSunucu : public QObject {
    Q_OBJECT
public:
    explicit ObysSunucu(QObject* parent = nullptr);
    bool baslat(quint16 port);

private slots:
    void yeniBaglanti();
    void veriHazir();
    void baglantiKesildi();

private:
    QString komutuIsle(QTcpSocket* istemci, const QString& satir);
    QString jsonAlanOku(const QString& metin, const QString& anahtar) const;
    QString uretToken() const;

    QTcpServer* server_{nullptr};
    QHash<QTcpSocket*, QString> oturumlar_;     // socket -> token
    QHash<QString, QString> kullaniciSifreler_; // kullanici -> hash
    QVector<Ogrenci> ogrenciler_;
};

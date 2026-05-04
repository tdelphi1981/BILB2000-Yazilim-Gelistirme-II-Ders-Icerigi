/**
 * Sunucu.h — OBYS protokol sunucusu (length-prefix + JSON dispatch)
 *
 * Bölüm 12 Ünite 4 Konu 3
 */

#pragma once

#include "CerceveCozucu.h"
#include "Protokol.h"

#include <QHash>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#include <functional>

// Çok basit bir öğrenci kaydı (örnek amaçlı)
struct OgrenciKaydi {
    int     numara;
    QString ad;
    QString bolum;
};

class Sunucu : public QObject {
    Q_OBJECT
public:
    explicit Sunucu(QObject* anaNesne = nullptr);

    bool dinlemeBaslat(quint16 port);

private slots:
    void yeniBaglanti();
    void istemciVeri();
    void istemciAyrildi();

private:
    // Komut adından komut işleyici fonksiyona eşleşme
    using KomutIsleyici = std::function<Cevap(Sunucu*, QTcpSocket*, const Komut&)>;

    void komutGonder(QTcpSocket* hedef, const Cevap& cvp);
    void komutTetikle(QTcpSocket* gonderen, const Komut& k);

    // Komut işleyicileri
    Cevap login(QTcpSocket* s, const Komut& k);
    Cevap ping(QTcpSocket* s, const Komut& k);
    Cevap ogrenciListele(QTcpSocket* s, const Komut& k);
    Cevap ogrenciEkle(QTcpSocket* s, const Komut& k);

    QTcpServer* m_sunucu;

    // Soket başına çerçeve çözücü ve oturum durumu
    QHash<QTcpSocket*, CerceveCozucu> m_cozucu;
    QHash<QTcpSocket*, bool>          m_girisYapildi;

    // Bellek içi öğrenci deposu (basit demo)
    QHash<int, OgrenciKaydi> m_ogrenciler;

    // Komut tablosu
    QHash<QString, KomutIsleyici> m_komutlar;
};

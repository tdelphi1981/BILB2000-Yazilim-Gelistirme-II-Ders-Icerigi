/**
 * CerceveCozucu.h — Length-prefix çerçeveleme yardımcıları
 *
 * Bölüm 12 Ünite 4 Konu 3
 */

#pragma once

#include <QByteArray>

#include <optional>

// Bir paketi length-prefix (4-byte big-endian) ile sar
QByteArray cerceveyeSar(const QByteArray& paket);

// Akış üzerinden parça parça beslenen çerçeve çözücü.
// Her tamamlanan paketi sonrakiPaket() ile döndürür.
class CerceveCozucu {
public:
    void besle(const QByteArray& parca);

    // Bir tam paket varsa onun gövdesini döndürür; yoksa nullopt
    std::optional<QByteArray> sonrakiPaket();

    qsizetype tamponBoyutu() const { return m_tampon.size(); }

private:
    QByteArray m_tampon;
};

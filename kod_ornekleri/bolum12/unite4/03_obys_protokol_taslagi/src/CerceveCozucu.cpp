/**
 * CerceveCozucu.cpp — Length-prefix yardımcılarının gerçekleştirimi
 *
 * Bölüm 12 Ünite 4 Konu 3
 */

#include "CerceveCozucu.h"

#include <QtEndian>

#include <stdexcept>

QByteArray cerceveyeSar(const QByteArray& paket) {
    QByteArray cerceve;
    cerceve.reserve(4 + paket.size());

    // 4 baytlık big-endian uzunluk önekı
    quint32 boyut = static_cast<quint32>(paket.size());
    QByteArray onek(4, 0);
    qToBigEndian(boyut, reinterpret_cast<uchar*>(onek.data()));

    cerceve.append(onek);
    cerceve.append(paket);
    return cerceve;
}

void CerceveCozucu::besle(const QByteArray& parca) {
    m_tampon.append(parca);
}

std::optional<QByteArray> CerceveCozucu::sonrakiPaket() {
    if (m_tampon.size() < 4) return std::nullopt;

    quint32 uzunluk = qFromBigEndian<quint32>(
        reinterpret_cast<const uchar*>(m_tampon.constData())
    );

    // Sağlamlık: aşırı büyük paketleri reddet (16 MiB sınır)
    if (uzunluk > 16u * 1024u * 1024u) {
        throw std::runtime_error("Paket çok büyük (> 16 MiB)");
    }

    if (m_tampon.size() < static_cast<qsizetype>(4 + uzunluk)) {
        return std::nullopt;
    }

    QByteArray govde = m_tampon.mid(4, static_cast<qsizetype>(uzunluk));
    m_tampon.remove(0, static_cast<qsizetype>(4 + uzunluk));
    return govde;
}

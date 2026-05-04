/**
 * Lab 12 - Çalışma 3: Length-Prefix Çerçeveleme Yardımcıları (kaynak)
 *
 * Bu dosya OGRENCI tarafindan TAMAMEN dolduruluyor degil; size temel
 * yardimcilar veriliyor (bilinmesi beklenmiyor). Yine de calismaya
 * basla- madan once mantigi anlamak icin okuyun.
 */

#include "cerceveleme.h"

#include <QtEndian>
#include <stdexcept>
#include <cstring>

namespace cerceveleme {

QByteArray cerceveSar(const QByteArray& payload)
{
    QByteArray cerceve;
    cerceve.reserve(4 + payload.size());

    // Big-endian uzunluk (network byte order)
    quint32 uzunluk_he = static_cast<quint32>(payload.size());
    quint32 uzunluk_be = qToBigEndian(uzunluk_he);

    cerceve.append(reinterpret_cast<const char*>(&uzunluk_be), 4);
    cerceve.append(payload);
    return cerceve;
}

QList<QByteArray> cerceveCoz(QByteArray& buffer, std::size_t maks_payload)
{
    QList<QByteArray> cerceveler;

    while (true) {
        // HEADER_BEKLE: 4 bayttan az ise dur
        if (buffer.size() < 4) break;

        // Uzunlugu cek (big-endian -> host)
        quint32 uzunluk_be = 0;
        std::memcpy(&uzunluk_be, buffer.constData(), 4);
        quint32 uzunluk = qFromBigEndian(uzunluk_be);

        if (uzunluk > maks_payload) {
            // Asiri buyuk: protokol ihlali, baglantiyi kes
            throw std::runtime_error("Cerceve cok buyuk");
        }

        // PAYLOAD_BEKLE: yeterince bayt yoksa dur
        if (static_cast<std::size_t>(buffer.size()) < 4 + uzunluk) break;

        // Tam cerceve hazir: cikar, listeye ekle, tampondan sil
        QByteArray payload = buffer.mid(4, static_cast<int>(uzunluk));
        cerceveler.append(payload);
        buffer.remove(0, 4 + static_cast<int>(uzunluk));
    }

    return cerceveler;
}

}  // namespace cerceveleme

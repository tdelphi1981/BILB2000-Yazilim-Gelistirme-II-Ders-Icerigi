/**
 * Lab 12 - Çalışma 3: Length-Prefix Çerçeveleme Yardımcıları (header)
 *
 * Bölüm 12 Ünite 4 Konu 1'deki encoder/decoder mantığının Qt-uyumlu
 * (QByteArray tabanlı) versiyonu.
 *
 * Çerçeve formatı:
 *   +----------+----------------------------------+
 *   | 4 bayt   |  N bayt payload                  |
 *   | uzunluk  |  (big-endian, htonl)             |
 *   +----------+----------------------------------+
 */

#pragma once

#include <QByteArray>
#include <QList>
#include <cstdint>

namespace cerceveleme {

// Bir payload'i 4 bayt big-endian uzunluk + payload olarak sarar.
QByteArray cerceveSar(const QByteArray& payload);

// Akumulator buffer'a yeni baytlari ekler ve buffer'da bulunan TUM tam
// cerceveleri cikarip donduren yardimci. Cikarilan baytlar buffer'dan
// silinir; tamamlanmamis kismi veri buffer'da kalir.
//
// Cerceve boyutu maks_payload'i asarsa istisna firlatir.
QList<QByteArray> cerceveCoz(QByteArray& buffer,
                              std::size_t maks_payload = 16 * 1024 * 1024);

}  // namespace cerceveleme

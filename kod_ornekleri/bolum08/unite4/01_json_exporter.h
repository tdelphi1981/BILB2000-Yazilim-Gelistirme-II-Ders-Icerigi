/**
 * 01_json_exporter.h -- JsonExporter sinifi (standalone ornek)
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 4 - OBYS GUI Mini Proje Tamamlama
 *
 * Bu sinif, OBYSServis icindeki ogrenci, ders ve not verisini insan
 * okunabilir bir JSON belgesine cevirir. QDataStream ile kaydedilen
 * .obys dosyasi "kendi formatimiz" (canonical) iken, JSON formati
 * dis sistemlerle paylasim (interchange) amacina hizmet eder.
 *
 * Tasarim karari: JsonExporter yalnizca DISA AKTARIM (export) yapar.
 * Geri okuma (import) kasten tanimlanmamistir; cunku canonical format
 * olarak .obys kullanilir ve JSON'dan geri donus tek yonlu olmalidir.
 *
 * NotTuru (std::variant) uzerinde std::visit ile tipe ozel JSON
 * temsili uretilir:
 *   SayisalNot -> { "tur": "sayisal", "deger": 87.5 }
 *   HarfNotu   -> { "tur": "harf",    "harf":  "AA"  }
 *   MuafNot    -> { "tur": "muaf" }
 *
 * Uretilen JSON hiyerarsisi:
 *   {
 *     "surum": "1.0.0",
 *     "olusturulmaZamani": "2026-04-11T14:05:00",
 *     "ogrenciler": [ {id, numara, adSoyad}, ... ],
 *     "dersler":    [ {id, kod, ad, kredi}, ... ],
 *     "notlar":     [ {ogrenciId, dersId, not}, ... ]
 *   }
 *
 * Derleme: Qt6::Core yeterlidir.
 */

#ifndef JSON_EXPORTER_H
#define JSON_EXPORTER_H

#include "obys_servis.h"

#include <QJsonObject>
#include <QString>

class JsonExporter {
public:
    JsonExporter() = default;

    // Verilen yola JSON dosyasini yazar. Basariysa true doner.
    // Hata durumunda sonHata() ile aciklayici bir metin alinabilir.
    bool disaAktar(const QString& yol, const OBYSServis& servis);

    // Son hatanin aciklamasi (UI'de QMessageBox ile gosterilebilir)
    QString sonHata() const { return m_sonHata; }

private:
    // Tekil kayitlari JSON nesnesine cevirme yardimcilari
    QJsonObject ogrenciyeJson(const Ogrenci& o)    const;
    QJsonObject dersyeJson(const Ders& d)          const;
    QJsonObject notKaydiJson(const DersKaydi& k)   const;

    // NotTuru (variant) -> QJsonObject
    QJsonObject notTuruyuJson(const NotTuru& n)    const;

    QString m_sonHata;
};

#endif // JSON_EXPORTER_H

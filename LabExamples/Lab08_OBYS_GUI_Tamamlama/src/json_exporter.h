/**
 * json_exporter.h -- JsonExporter sinifi (OBYS v4 surumu)
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 4 - OBYS GUI Mini Proje Tamamlama
 *
 * OBYSServis icindeki ogrenci, ders ve not verisini insan okunabilir
 * bir JSON dosyasina cevirir. Tasarim karari geregi bu sinif YALNIZCA
 * export yapar; JSON'dan geri okuma (import) bilincli olarak
 * tanimlanmamistir. Kendi verimizin canonical formati .obys'dir;
 * JSON ise dis sistemlerle paylasim icin tek yonlu bir ciktidir.
 */

#ifndef JSON_EXPORTER_H
#define JSON_EXPORTER_H

#include "obys_servis.h"

#include <QJsonObject>
#include <QString>

class JsonExporter {
public:
    JsonExporter() = default;

    bool disaAktar(const QString& yol, const OBYSServis& servis);
    QString sonHata() const { return m_sonHata; }

private:
    QJsonObject ogrenciyeJson(const Ogrenci& o)    const;
    QJsonObject dersyeJson(const Ders& d)          const;
    QJsonObject notKaydiJson(const DersKaydi& k)   const;
    QJsonObject notTuruyuJson(const NotTuru& n)    const;

    QString m_sonHata;
};

#endif // JSON_EXPORTER_H

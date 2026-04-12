#ifndef JSON_EXPORTER_H
#define JSON_EXPORTER_H

#include "obys_servis.h"
#include <QJsonObject>
#include <QString>

class JsonExporter {
public:
    bool disaAktar(const QString& yol,
                   const OBYSServis& servis);
    QString sonHata() const
        { return m_sonHata; }

private:
    QJsonObject ogrenciyeJson(
        const Ogrenci& o) const;
    QJsonObject dersyeJson(
        const Ders& d) const;
    QJsonObject notKaydiJson(
        const DersKaydi& k) const;
    QJsonObject notTuruyuJson(
        const NotTuru& n) const;

    QString m_sonHata;
};

#endif // JSON_EXPORTER_H

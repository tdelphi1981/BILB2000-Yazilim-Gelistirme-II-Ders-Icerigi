/**
 * notlar_model.h -- OBYS not kayitlari icin QAbstractTableModel
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * NotlarModel, OBYSServis uzerindeki DersKaydi listesinin basit bir
 * tablo gorunumunu saglar. Dort sutundan olusur:
 *   0: Id       (DersKaydi indeksinin 1 tabanli numarasi)
 *   1: Ogrenci  (ad soyad)
 *   2: Not      (sayisal deger ya da MUAF)
 *   3: Harf     (sayisal degerden hesaplanan harf notu)
 *
 * IstatistikModel bu modeli gozler ve 3. sutundan (Harf) histogram
 * cikarir.
 */

#ifndef NOTLAR_MODEL_H
#define NOTLAR_MODEL_H

#include "obys_servis.h"

#include <QAbstractTableModel>

class NotlarModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit NotlarModel(OBYSServis* servis,
                         QObject* parent = nullptr);

    // Servisteki veriler disaridan degistiginde (ornegin dialog ile
    // yeni not eklendikten sonra) MainWindow bu yontemi cagirir.
    void yenile();

    // QAbstractTableModel arayuzu
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& idx,
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orient,
                        int role = Qt::DisplayRole) const override;

private:
    OBYSServis* m_servis{nullptr};
};

#endif // NOTLAR_MODEL_H

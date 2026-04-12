/**
 * istatistik_model.h -- Harf notu histogrami icin proxy model
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * IstatistikModel, bir kaynak QAbstractItemModel (NotlarModel) uzerinde
 * "Harf" sutununu izleyerek 8 satirlik bir histogram tablosu sunar.
 * Kaynaktaki dataChanged/rowsInserted/modelReset sinyallerini dinler;
 * kendi tablosunu gunceller ve dataChanged sinyalini dogru aralikla
 * yayinlar. Bu sayede bir QVBarModelMapper ile QBarSeries'e baglandiginda
 * grafik otomatik olarak yenilenir.
 *
 * Tablo yapisi:
 *   satirlar: AA, BA, BB, CB, CC, DC, DD, FF
 *   sutunlar: 0 -> Harf, 1 -> Sayi
 */

#ifndef ISTATISTIK_MODEL_H
#define ISTATISTIK_MODEL_H

#include <QAbstractTableModel>
#include <QPointer>
#include <QStringList>
#include <QVector>

class IstatistikModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit IstatistikModel(int harfSutunu = 3,
                             QObject* parent = nullptr);

    void setKaynakModel(QAbstractItemModel* kaynak);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& idx,
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orient,
                        int role = Qt::DisplayRole) const override;

private slots:
    void kaynakDegisti();

private:
    void histogramiHesapla();

    QPointer<QAbstractItemModel> m_kaynak;
    int                          m_harfSutunu;
    QStringList                  m_harfler;
    QVector<int>                 m_sayilar;
};

#endif // ISTATISTIK_MODEL_H

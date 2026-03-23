// ogrencimodel.h - Ogrenci veri modeli (QAbstractTableModel)
#ifndef OGRENCIMODEL_H
#define OGRENCIMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QVector>

struct Ogrenci {
    QString numara;
    QString ad;
    QString bolum;
    int sinif;
    double gpa;
};

class OgrenciModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit OgrenciModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void ogrenciEkle(const Ogrenci &ogrenci);
    void ogrenciSil(int satir);
    int ogrenciSayisi() const { return m_ogrenciler.size(); }

private:
    QVector<Ogrenci> m_ogrenciler;
};

#endif // OGRENCIMODEL_H

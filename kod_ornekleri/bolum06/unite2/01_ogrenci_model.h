/**
 * OgrenciModel Sinif Tanimi
 *
 * QAbstractTableModel'den tureyen ozel tablo modeli.
 * OBYS ogrenci verilerini Model/View mimarisinde sunar.
 *
 * Ogrenilecek kavramlar:
 * - QAbstractTableModel turetme
 * - Roller ile kosullu gosterim
 * - beginInsertRows/endInsertRows
 *
 * Bolum: 06 - Qt Model/View Mimarisi
 * Unite: 2 - QTableView ve Ozel Model
 *
 * Derleme: Qt6 projesi olarak CMake ile
 */

#ifndef OGRENCI_MODEL_H
#define OGRENCI_MODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QBrush>
#include <QFont>
#include <QString>

// Ogrenci veri yapisi
struct Ogrenci {
    QString numara;
    QString adSoyad;
    QString bolum;
    int sinif = 1;
    double gpa = 0.0;
};

class OgrenciModel : public QAbstractTableModel {
    Q_OBJECT

public:
    // Sutun indeksleri
    enum Sutunlar {
        Numara = 0,
        AdSoyad,
        Bolum,
        Sinif,
        GPA,
        SutunSayisi
    };

    explicit OgrenciModel(QObject *parent = nullptr);

    // Zorunlu override'lar
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Duzenleme desteği
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Veri yonetimi
    void ogrenciEkle(const Ogrenci& ogrenci);
    void ogrenciSil(int satir);

signals:
    void dogrulamaHatasi(const QString& mesaj);

private:
    QVector<Ogrenci> m_ogrenciler;
};

#endif // OGRENCI_MODEL_H

#ifndef ISTATISTIK_MODEL_H
#define ISTATISTIK_MODEL_H

#include <QAbstractTableModel>
#include <QPointer>
#include <QStringList>
#include <QVector>

class IstatistikModel
    : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit IstatistikModel(
        int harfSutunu = 3,
        QObject* parent = nullptr);

    void setKaynakModel(
        QAbstractItemModel* kaynak);

    int rowCount(const QModelIndex& parent
        = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent
        = QModelIndex()) const override;
    QVariant data(const QModelIndex& idx,
        int role = Qt::DisplayRole)
        const override;
    QVariant headerData(int section,
        Qt::Orientation orient,
        int role = Qt::DisplayRole)
        const override;

private slots:
    void kaynakDegisti();

private:
    void histogramiHesapla();

    QPointer<QAbstractItemModel> m_kaynak;
    int          m_harfSutunu;
    QStringList  m_harfler;
    QVector<int> m_sayilar;
};

#endif // ISTATISTIK_MODEL_H

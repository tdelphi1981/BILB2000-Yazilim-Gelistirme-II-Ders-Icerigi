#ifndef NOTLAR_MODEL_H
#define NOTLAR_MODEL_H

#include "obys_servis.h"
#include <QAbstractTableModel>

class NotlarModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit NotlarModel(OBYSServis* servis,
                         QObject* parent = nullptr);

    void yenile();

    int rowCount(const QModelIndex& parent
        = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent
        = QModelIndex()) const override;
    QVariant data(const QModelIndex& idx,
        int role = Qt::DisplayRole) const override;
    QVariant headerData(int section,
        Qt::Orientation orient,
        int role = Qt::DisplayRole) const override;

private:
    OBYSServis* m_servis{nullptr};
};

#endif // NOTLAR_MODEL_H

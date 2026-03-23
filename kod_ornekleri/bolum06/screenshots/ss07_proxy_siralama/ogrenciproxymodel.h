// ogrenciproxymodel.h - Turkce siralamayi ve metin filtrelemeyi destekleyen proxy model
#ifndef OGRENCIPROXYMODEL_H
#define OGRENCIPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QCollator>

class OgrenciProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit OgrenciProxyModel(QObject *parent = nullptr);

    void setMetinFiltresi(const QString &metin);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QCollator m_collator;
    QString m_metinFiltresi;
};

#endif // OGRENCIPROXYMODEL_H

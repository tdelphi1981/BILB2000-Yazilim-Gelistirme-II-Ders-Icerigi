// ogrenciproxymodel.h - Gelismis coklu filtre destekli proxy model
#ifndef OGRENCIPROXYMODEL_H
#define OGRENCIPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QCollator>

class OgrenciProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit OgrenciProxyModel(QObject *parent = nullptr);

    void setBolumFiltresi(const QString &bolum);
    void setMinGpa(double gpa);
    void setMetinFiltresi(const QString &metin);

    int filtrelenenSayisi() const;

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QCollator m_collator;
    QString m_bolumFiltresi;
    double m_minGpa = 0.0;
    QString m_metinFiltresi;
};

#endif // OGRENCIPROXYMODEL_H

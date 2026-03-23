// ogrenciproxymodel.cpp - Proxy model uygulamasi
#include "ogrenciproxymodel.h"
#include <QLocale>

OgrenciProxyModel::OgrenciProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_collator.setLocale(QLocale(QLocale::Turkish, QLocale::Turkey));
    m_collator.setCaseSensitivity(Qt::CaseInsensitive);
}

void OgrenciProxyModel::setMetinFiltresi(const QString &metin)
{
    m_metinFiltresi = metin;
    invalidateFilter();
}

bool OgrenciProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant solVeri = sourceModel()->data(left);
    QVariant sagVeri = sourceModel()->data(right);

    int sutun = left.column();

    // GPA sutunu (4) - sayisal siralama
    if (sutun == 4) {
        return solVeri.toDouble() < sagVeri.toDouble();
    }

    // Sinif sutunu (3) - sayisal siralama
    if (sutun == 3) {
        return solVeri.toInt() < sagVeri.toInt();
    }

    // Metin sutunlari - Turkce collator ile siralama
    return m_collator.compare(solVeri.toString(), sagVeri.toString()) < 0;
}

bool OgrenciProxyModel::filterAcceptsRow(int sourceRow,
                                         const QModelIndex &sourceParent) const
{
    if (m_metinFiltresi.isEmpty())
        return true;

    // Tum sutunlarda ara
    for (int col = 0; col < sourceModel()->columnCount(); ++col) {
        QModelIndex idx = sourceModel()->index(sourceRow, col, sourceParent);
        QString veri = sourceModel()->data(idx).toString();
        if (veri.contains(m_metinFiltresi, Qt::CaseInsensitive))
            return true;
    }

    return false;
}

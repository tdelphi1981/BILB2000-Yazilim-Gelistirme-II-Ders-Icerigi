// ogrenciproxymodel.cpp - Gelismis proxy model uygulamasi
#include "ogrenciproxymodel.h"
#include <QLocale>

OgrenciProxyModel::OgrenciProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_collator.setLocale(QLocale(QLocale::Turkish, QLocale::Turkey));
    m_collator.setCaseSensitivity(Qt::CaseInsensitive);
}

void OgrenciProxyModel::setBolumFiltresi(const QString &bolum)
{
    m_bolumFiltresi = bolum;
    invalidateFilter();
}

void OgrenciProxyModel::setMinGpa(double gpa)
{
    m_minGpa = gpa;
    invalidateFilter();
}

void OgrenciProxyModel::setMetinFiltresi(const QString &metin)
{
    m_metinFiltresi = metin;
    invalidateFilter();
}

int OgrenciProxyModel::filtrelenenSayisi() const
{
    return rowCount();
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
    // Bolum filtresi
    if (!m_bolumFiltresi.isEmpty()) {
        QModelIndex bolumIdx = sourceModel()->index(sourceRow, 2, sourceParent);
        QString bolum = sourceModel()->data(bolumIdx).toString();
        if (bolum != m_bolumFiltresi)
            return false;
    }

    // Minimum GPA filtresi
    if (m_minGpa > 0.0) {
        QModelIndex gpaIdx = sourceModel()->index(sourceRow, 4, sourceParent);
        double gpa = sourceModel()->data(gpaIdx).toDouble();
        if (gpa < m_minGpa)
            return false;
    }

    // Metin arama filtresi (tum sutunlarda)
    if (!m_metinFiltresi.isEmpty()) {
        bool bulundu = false;
        for (int col = 0; col < sourceModel()->columnCount(); ++col) {
            QModelIndex idx = sourceModel()->index(sourceRow, col, sourceParent);
            QString veri = sourceModel()->data(idx).toString();
            if (veri.contains(m_metinFiltresi, Qt::CaseInsensitive)) {
                bulundu = true;
                break;
            }
        }
        if (!bulundu)
            return false;
    }

    return true;
}

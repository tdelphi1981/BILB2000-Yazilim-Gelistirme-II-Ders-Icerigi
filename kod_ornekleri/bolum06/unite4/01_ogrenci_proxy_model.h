/**
 * OgrenciProxyModel - Ozel Siralama ve Filtreleme
 *
 * QSortFilterProxyModel'den tureyen ozel proxy model.
 * Turkce siralama ve coklu filtre destegi saglar.
 *
 * Bolum: 06 - Qt Model/View Mimarisi
 * Unite: 4 - Siralama ve Filtreleme
 */

#ifndef OGRENCI_PROXY_MODEL_H
#define OGRENCI_PROXY_MODEL_H

#include <QSortFilterProxyModel>
#include <QCollator>
#include "01_ogrenci_model.h"

class OgrenciProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit OgrenciProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent) {
        // Turkce locale ile siralama
        m_collator.setLocale(QLocale(QLocale::Turkish));
        m_collator.setCaseSensitivity(Qt::CaseInsensitive);
    }

    // Filtre ayarlari
    void setBolumFiltresi(const QString& bolum) {
        m_bolumFiltresi = bolum;
        invalidateFilter();
    }

    void setMinGpa(double gpa) {
        m_minGpa = gpa;
        invalidateFilter();
    }

    void setSinifFiltresi(int sinif) {
        m_sinifFiltresi = sinif;
        invalidateFilter();
    }

protected:
    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override {
        QVariant solVeri = sourceModel()->data(left);
        QVariant sagVeri = sourceModel()->data(right);

        // Sayisal sutunlar
        if (left.column() == OgrenciModel::GPA) {
            return solVeri.toDouble() < sagVeri.toDouble();
        }
        if (left.column() == OgrenciModel::Sinif) {
            return solVeri.toInt() < sagVeri.toInt();
        }

        // Metin sutunlari: Turkce locale
        return m_collator.compare(
            solVeri.toString(), sagVeri.toString()) < 0;
    }

    bool filterAcceptsRow(int sourceRow,
            const QModelIndex& sourceParent) const override {
        // Metin filtresi (arama cubugu)
        if (!filterRegularExpression().pattern().isEmpty()) {
            bool metinUyumu = false;
            for (int col = 0; col < sourceModel()->columnCount(); ++col) {
                QModelIndex idx = sourceModel()->index(
                    sourceRow, col, sourceParent);
                if (sourceModel()->data(idx).toString()
                        .contains(filterRegularExpression())) {
                    metinUyumu = true;
                    break;
                }
            }
            if (!metinUyumu) return false;
        }

        // Bolum filtresi
        if (!m_bolumFiltresi.isEmpty()) {
            QModelIndex bolumIdx = sourceModel()->index(
                sourceRow, OgrenciModel::Bolum, sourceParent);
            QString bolum = sourceModel()->data(bolumIdx).toString();
            if (!bolum.contains(m_bolumFiltresi, Qt::CaseInsensitive))
                return false;
        }

        // Minimum GPA filtresi
        if (m_minGpa > 0.0) {
            QModelIndex gpaIdx = sourceModel()->index(
                sourceRow, OgrenciModel::GPA, sourceParent);
            double gpa = sourceModel()->data(gpaIdx).toDouble();
            if (gpa < m_minGpa)
                return false;
        }

        // Sinif filtresi
        if (m_sinifFiltresi > 0) {
            QModelIndex sinifIdx = sourceModel()->index(
                sourceRow, OgrenciModel::Sinif, sourceParent);
            int sinif = sourceModel()->data(sinifIdx).toInt();
            if (sinif != m_sinifFiltresi)
                return false;
        }

        return true;
    }

private:
    QCollator m_collator;
    QString m_bolumFiltresi;
    double m_minGpa = 0.0;
    int m_sinifFiltresi = 0;
};

#endif // OGRENCI_PROXY_MODEL_H

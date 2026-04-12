/**
 * 03_grafik_model_baglama.cpp -- IstatistikModel uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * Not: Bu dosya bir kutuphane dosyasidir. Tek basina calistirilabilir
 * bir program degildir; 04_obys_v3_grafik/ projesi icindeki
 * istatistik_model sinifi ile ayni gorevi Qt Charts demo amaciyla
 * gerceklestirir. 03 numaranin amaci sinifi bagimsiz bir baslik/cpp
 * ikilisi olarak incelenebilir kilmaktir.
 */

#include "03_grafik_model_baglama.h"

#include <QStringLiteral>

IstatistikModel::IstatistikModel(int harfSutunu, QObject* parent)
    : QAbstractTableModel(parent)
    , m_harfSutunu(harfSutunu)
    , m_harfler{
          QStringLiteral("AA"), QStringLiteral("BA"),
          QStringLiteral("BB"), QStringLiteral("CB"),
          QStringLiteral("CC"), QStringLiteral("DC"),
          QStringLiteral("DD"), QStringLiteral("FF")
      }
    , m_sayilar(8, 0)
{
}

void IstatistikModel::setKaynakModel(QAbstractItemModel* kaynak) {
    // Eski kaynaktan gelen tum sinyalleri kes
    if (m_kaynak) {
        disconnect(m_kaynak, nullptr, this, nullptr);
    }

    m_kaynak = kaynak;

    if (m_kaynak) {
        connect(m_kaynak, &QAbstractItemModel::dataChanged,
                this, &IstatistikModel::kaynakDegisti);
        connect(m_kaynak, &QAbstractItemModel::rowsInserted,
                this, &IstatistikModel::kaynakDegisti);
        connect(m_kaynak, &QAbstractItemModel::rowsRemoved,
                this, &IstatistikModel::kaynakDegisti);
        connect(m_kaynak, &QAbstractItemModel::modelReset,
                this, &IstatistikModel::kaynakDegisti);
    }

    // Kaynak degisti -> histogrami bastan hesapla
    beginResetModel();
    histogramiHesapla();
    endResetModel();
}

int IstatistikModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) { return 0; }
    return static_cast<int>(m_harfler.size());
}

int IstatistikModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) { return 0; }
    return 2; // Harf, Sayi
}

QVariant IstatistikModel::data(const QModelIndex& idx, int role) const {
    if (!idx.isValid()) { return {}; }
    if (role != Qt::DisplayRole && role != Qt::EditRole) { return {}; }

    const int satir = idx.row();
    if (satir < 0 || satir >= m_harfler.size()) { return {}; }

    if (idx.column() == 0) {
        return m_harfler.at(satir);
    }
    if (idx.column() == 1) {
        return m_sayilar.value(satir, 0);
    }
    return {};
}

QVariant IstatistikModel::headerData(int section,
                                     Qt::Orientation orient,
                                     int role) const {
    if (role != Qt::DisplayRole) { return {}; }
    if (orient == Qt::Horizontal) {
        if (section == 0) return QStringLiteral("Harf");
        if (section == 1) return QStringLiteral("Sayi");
    }
    return section + 1;
}

void IstatistikModel::kaynakDegisti() {
    histogramiHesapla();
    // Yalnizca "Sayi" sutunundaki tum satirlar degisti
    const QModelIndex sol  = index(0, 1);
    const QModelIndex sag  = index(rowCount() - 1, 1);
    emit dataChanged(sol, sag, {Qt::DisplayRole});
}

void IstatistikModel::histogramiHesapla() {
    m_sayilar.fill(0, 8);

    if (!m_kaynak) { return; }

    const int satirSayisi = m_kaynak->rowCount();
    for (int r = 0; r < satirSayisi; ++r) {
        const QModelIndex hi = m_kaynak->index(r, m_harfSutunu);
        const QString harf = m_kaynak->data(hi, Qt::DisplayRole)
                                .toString().trimmed().toUpper();
        const int idx = m_harfler.indexOf(harf);
        if (idx >= 0 && idx < m_sayilar.size()) {
            ++m_sayilar[idx];
        }
    }
}

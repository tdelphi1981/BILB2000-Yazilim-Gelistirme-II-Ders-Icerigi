#include "istatistik_model.h"

IstatistikModel::IstatistikModel(
    int harfSutunu, QObject* parent)
    : QAbstractTableModel(parent)
    , m_harfSutunu(harfSutunu)
    , m_harfler{"AA","BA","BB","CB",
                "CC","DC","DD","FF"}
    , m_sayilar(8, 0)
{}

void IstatistikModel::setKaynakModel(
    QAbstractItemModel* kaynak) {
    if (m_kaynak)
        disconnect(m_kaynak, nullptr,
                   this, nullptr);

    m_kaynak = kaynak;

    if (m_kaynak) {
        connect(m_kaynak,
            &QAbstractItemModel::dataChanged,
            this, &IstatistikModel::kaynakDegisti);
        connect(m_kaynak,
            &QAbstractItemModel::modelReset,
            this, &IstatistikModel::kaynakDegisti);
        connect(m_kaynak,
            &QAbstractItemModel::rowsInserted,
            this, &IstatistikModel::kaynakDegisti);
        connect(m_kaynak,
            &QAbstractItemModel::rowsRemoved,
            this, &IstatistikModel::kaynakDegisti);
    }

    beginResetModel();
    histogramiHesapla();
    endResetModel();
}

int IstatistikModel::rowCount(
    const QModelIndex& parent) const {
    return parent.isValid()
        ? 0 : m_harfler.size();
}

int IstatistikModel::columnCount(
    const QModelIndex& parent) const {
    return parent.isValid() ? 0 : 2;
}

QVariant IstatistikModel::data(
    const QModelIndex& idx, int role) const {
    if (!idx.isValid()) return {};
    if (role != Qt::DisplayRole
        && role != Qt::EditRole) return {};
    const int r = idx.row();
    if (r < 0 || r >= m_harfler.size()) return {};
    return idx.column() == 0
        ? QVariant(m_harfler.at(r))
        : QVariant(m_sayilar.value(r, 0));
}

QVariant IstatistikModel::headerData(
    int section, Qt::Orientation orient,
    int role) const {
    if (role != Qt::DisplayRole) return {};
    if (orient == Qt::Horizontal) {
        return section == 0 ? "Harf" : "Sayi";
    }
    return section + 1;
}

void IstatistikModel::kaynakDegisti() {
    histogramiHesapla();
    const QModelIndex sol = index(0, 1);
    const QModelIndex sag =
        index(rowCount() - 1, 1);
    emit dataChanged(sol, sag,
                     {Qt::DisplayRole});
}

void IstatistikModel::histogramiHesapla() {
    m_sayilar.fill(0, 8);
    if (!m_kaynak) return;
    for (int r = 0;
         r < m_kaynak->rowCount(); ++r) {
        const QString harf =
            m_kaynak->data(
                m_kaynak->index(r,
                    m_harfSutunu))
            .toString().trimmed().toUpper();
        const int idx =
            m_harfler.indexOf(harf);
        if (idx >= 0) ++m_sayilar[idx];
    }
}

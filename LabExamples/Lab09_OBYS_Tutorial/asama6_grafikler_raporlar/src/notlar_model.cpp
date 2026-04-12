#include "notlar_model.h"
#include "obys_domain.h"
#include <variant>

namespace {
QString sayisalHarf(double puan) {
    if (puan >= 90) return "AA";
    if (puan >= 85) return "BA";
    if (puan >= 80) return "BB";
    if (puan >= 75) return "CB";
    if (puan >= 65) return "CC";
    if (puan >= 58) return "DC";
    if (puan >= 50) return "DD";
    return "FF";
}

QString ogrenciAdi(int id,
                   const OBYSServis& servis) {
    for (const auto& o : servis.tumOgrenciler()) {
        if (o.id == id) return o.adSoyad;
    }
    return "?";
}
} // namespace

NotlarModel::NotlarModel(OBYSServis* servis,
                         QObject* parent)
    : QAbstractTableModel(parent)
    , m_servis(servis) {}

void NotlarModel::yenile() {
    beginResetModel();
    endResetModel();
}

int NotlarModel::rowCount(
    const QModelIndex& parent) const {
    if (parent.isValid() || !m_servis) return 0;
    return m_servis->tumNotlar().size();
}

int NotlarModel::columnCount(
    const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return 4; // #, Ogrenci, Not, Harf
}

QVariant NotlarModel::data(const QModelIndex& idx,
                           int role) const {
    if (!idx.isValid() || !m_servis) return {};
    if (role != Qt::DisplayRole) return {};

    const auto notlar = m_servis->tumNotlar();
    const int satir = idx.row();
    if (satir < 0 || satir >= notlar.size())
        return {};

    const DersKaydi& k = notlar.at(satir);

    switch (idx.column()) {
    case 0: return satir + 1;
    case 1: return ogrenciAdi(k.ogrenciId,
                              *m_servis);
    case 2: {
        QString s;
        std::visit([&s](const auto& d) {
            using T = std::decay_t<decltype(d)>;
            if constexpr (
                std::is_same_v<T, SayisalNot>)
                s = QString::number(d.deger, 'f', 1);
            else if constexpr (
                std::is_same_v<T, HarfNotu>)
                s = d.harf;
            else if constexpr (
                std::is_same_v<T, MuafNot>)
                s = "MUAF";
        }, k.notu);
        return s;
    }
    case 3: {
        QString h;
        std::visit([&h](const auto& d) {
            using T = std::decay_t<decltype(d)>;
            if constexpr (
                std::is_same_v<T, SayisalNot>)
                h = sayisalHarf(d.deger);
            else if constexpr (
                std::is_same_v<T, HarfNotu>)
                h = d.harf;
            else if constexpr (
                std::is_same_v<T, MuafNot>)
                h = "-";
        }, k.notu);
        return h;
    }
    default: return {};
    }
}

QVariant NotlarModel::headerData(
    int section, Qt::Orientation orient,
    int role) const {
    if (role != Qt::DisplayRole) return {};
    if (orient == Qt::Horizontal) {
        switch (section) {
        case 0: return "#";
        case 1: return "Öğrenci";
        case 2: return "Not";
        case 3: return "Harf";
        }
    }
    return section + 1;
}

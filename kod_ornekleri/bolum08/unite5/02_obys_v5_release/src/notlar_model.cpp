/**
 * notlar_model.cpp -- NotlarModel uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 */

#include "notlar_model.h"

#include "obys_domain.h"

#include <QStringLiteral>

#include <variant>

namespace {

// Sayisal puani KTU harf notu sistemine donusturur.
QString sayisalHarf(double puan) {
    if (puan >= 90) return QStringLiteral("AA");
    if (puan >= 85) return QStringLiteral("BA");
    if (puan >= 80) return QStringLiteral("BB");
    if (puan >= 75) return QStringLiteral("CB");
    if (puan >= 65) return QStringLiteral("CC");
    if (puan >= 58) return QStringLiteral("DC");
    if (puan >= 50) return QStringLiteral("DD");
    return QStringLiteral("FF");
}

QString ogrenciAdi(int id, const OBYSServis& servis) {
    for (const auto& o : servis.tumOgrenciler()) {
        if (o.id == id) { return o.adSoyad; }
    }
    return QStringLiteral("?");
}

} // namespace

NotlarModel::NotlarModel(OBYSServis* servis, QObject* parent)
    : QAbstractTableModel(parent)
    , m_servis(servis)
{
}

void NotlarModel::yenile() {
    beginResetModel();
    endResetModel();
}

int NotlarModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid() || m_servis == nullptr) { return 0; }
    return m_servis->tumNotlar().size();
}

int NotlarModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) { return 0; }
    return 4;
}

QVariant NotlarModel::data(const QModelIndex& idx, int role) const {
    if (!idx.isValid() || m_servis == nullptr) { return {}; }
    if (role != Qt::DisplayRole && role != Qt::EditRole) { return {}; }

    const auto notlar = m_servis->tumNotlar();
    const int  satir  = idx.row();
    if (satir < 0 || satir >= notlar.size()) { return {}; }

    const DersKaydi& k = notlar.at(satir);

    switch (idx.column()) {
    case 0:
        return satir + 1;
    case 1:
        return ogrenciAdi(k.ogrenciId, *m_servis);
    case 2: {
        QString sonuc;
        std::visit([&sonuc](const auto& d) {
            using T = std::decay_t<decltype(d)>;
            if constexpr (std::is_same_v<T, SayisalNot>) {
                sonuc = QString::number(d.deger, 'f', 1);
            } else if constexpr (std::is_same_v<T, HarfNotu>) {
                sonuc = d.harf;
            } else if constexpr (std::is_same_v<T, MuafNot>) {
                sonuc = QStringLiteral("MUAF");
            }
        }, k.notu);
        return sonuc;
    }
    case 3: {
        QString harf;
        std::visit([&harf](const auto& d) {
            using T = std::decay_t<decltype(d)>;
            if constexpr (std::is_same_v<T, SayisalNot>) {
                harf = sayisalHarf(d.deger);
            } else if constexpr (std::is_same_v<T, HarfNotu>) {
                harf = d.harf;
            } else if constexpr (std::is_same_v<T, MuafNot>) {
                harf = QStringLiteral("-");
            }
        }, k.notu);
        return harf;
    }
    default:
        return {};
    }
}

QVariant NotlarModel::headerData(int section, Qt::Orientation orient,
                                 int role) const {
    if (role != Qt::DisplayRole) { return {}; }
    if (orient == Qt::Horizontal) {
        switch (section) {
        case 0: return QStringLiteral("#");
        case 1: return QStringLiteral("Ogrenci");
        case 2: return QStringLiteral("Not");
        case 3: return QStringLiteral("Harf");
        }
    }
    return section + 1;
}

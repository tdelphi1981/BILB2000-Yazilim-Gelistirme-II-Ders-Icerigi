/**
 * OgrenciModel Uygulama Dosyasi
 *
 * QAbstractTableModel'den tureyen ozel tablo modeli.
 * Roller ile kosullu renklendirme, hizalama ve ipuclari.
 *
 * Bolum: 06 - Qt Model/View Mimarisi
 * Unite: 2 - QTableView ve Ozel Model
 */

#include "01_ogrenci_model.h"

OgrenciModel::OgrenciModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int OgrenciModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_ogrenciler.size());
}

int OgrenciModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;
    return SutunSayisi;
}

QVariant OgrenciModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_ogrenciler.size())
        return QVariant();

    const Ogrenci& ogr = m_ogrenciler.at(index.row());

    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch (index.column()) {
                case Numara:  return ogr.numara;
                case AdSoyad: return ogr.adSoyad;
                case Bolum:   return ogr.bolum;
                case Sinif:   return ogr.sinif;
                case GPA:     return QString::number(ogr.gpa, 'f', 2);
            }
            break;

        case Qt::TextAlignmentRole:
            if (index.column() == Sinif || index.column() == Numara)
                return QVariant(Qt::AlignCenter);
            if (index.column() == GPA)
                return QVariant(Qt::AlignRight | Qt::AlignVCenter);
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);

        case Qt::BackgroundRole:
            if (ogr.gpa < 2.0)
                return QBrush(QColor(255, 200, 200));
            if (ogr.gpa >= 3.5)
                return QBrush(QColor(200, 255, 200));
            break;

        case Qt::FontRole:
            if (index.column() == AdSoyad && ogr.gpa >= 3.5) {
                QFont font;
                font.setBold(true);
                return font;
            }
            break;

        case Qt::ToolTipRole:
            return QString("%1 (%2) - GPA: %3")
                   .arg(ogr.adSoyad, ogr.bolum)
                   .arg(ogr.gpa, 0, 'f', 2);
    }

    return QVariant();
}

QVariant OgrenciModel::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case Numara:  return "Numara";
            case AdSoyad: return "Ad Soyad";
            case Bolum:   return "Bolum";
            case Sinif:   return "Sinif";
            case GPA:     return "GPA";
        }
    }

    if (orientation == Qt::Vertical)
        return section + 1;

    return QVariant();
}

bool OgrenciModel::setData(const QModelIndex& index,
                           const QVariant& value, int role) {
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    if (index.row() >= m_ogrenciler.size())
        return false;

    Ogrenci& ogr = m_ogrenciler[index.row()];

    switch (index.column()) {
        case AdSoyad: {
            QString ad = value.toString().trimmed();
            if (ad.isEmpty()) {
                emit dogrulamaHatasi("Ad Soyad bos olamaz!");
                return false;
            }
            ogr.adSoyad = ad;
            break;
        }
        case Bolum: {
            QString bolum = value.toString().trimmed();
            if (bolum.isEmpty()) {
                emit dogrulamaHatasi("Bolum bos olamaz!");
                return false;
            }
            ogr.bolum = bolum;
            break;
        }
        case Sinif: {
            bool ok;
            int sinif = value.toInt(&ok);
            if (!ok || sinif < 1 || sinif > 4) {
                emit dogrulamaHatasi("Sinif 1-4 arasi olmali!");
                return false;
            }
            ogr.sinif = sinif;
            break;
        }
        case GPA: {
            bool ok;
            double gpa = value.toDouble(&ok);
            if (!ok || gpa < 0.0 || gpa > 4.0) {
                emit dogrulamaHatasi("GPA 0.00-4.00 arasi olmali!");
                return false;
            }
            ogr.gpa = gpa;
            break;
        }
        default:
            return false;
    }

    // Tum satiri guncelle
    QModelIndex ilk = this->index(index.row(), 0);
    QModelIndex son = this->index(index.row(), SutunSayisi - 1);
    emit dataChanged(ilk, son);
    return true;
}

Qt::ItemFlags OgrenciModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags varsayilan = QAbstractTableModel::flags(index);

    // Numara sutunu duzenlenemez
    if (index.column() == Numara)
        return varsayilan;

    return varsayilan | Qt::ItemIsEditable;
}

void OgrenciModel::ogrenciEkle(const Ogrenci& ogrenci) {
    int satir = m_ogrenciler.size();
    beginInsertRows(QModelIndex(), satir, satir);
    m_ogrenciler.append(ogrenci);
    endInsertRows();
}

void OgrenciModel::ogrenciSil(int satir) {
    if (satir < 0 || satir >= m_ogrenciler.size())
        return;
    beginRemoveRows(QModelIndex(), satir, satir);
    m_ogrenciler.removeAt(satir);
    endRemoveRows();
}

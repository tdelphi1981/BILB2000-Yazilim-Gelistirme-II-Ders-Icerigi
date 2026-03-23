// ogrencimodel.cpp - Ogrenci veri modeli uygulamasi
#include "ogrencimodel.h"
#include <QBrush>
#include <QFont>

OgrenciModel::OgrenciModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_ogrenciler = {
        {"2024001", QString::fromUtf8("Ayşe Yılmaz"),    QString::fromUtf8("Bilgisayar Bilimleri"),       3, 3.45},
        {"2024002", QString::fromUtf8("Mehmet Kaya"),     QString::fromUtf8("Yazılım Mühendisliği"),       2, 2.80},
        {"2024003", QString::fromUtf8("Elif Demir"),      QString::fromUtf8("Bilgisayar Bilimleri"),       4, 3.72},
        {"2024004", QString::fromUtf8("Ahmet Çelik"),     QString::fromUtf8("Elektrik-Elektronik"),        1, 3.10},
        {"2024005", QString::fromUtf8("Fatma Öztürk"),    QString::fromUtf8("Makine Mühendisliği"),        2, 2.55},
        {"2024006", QString::fromUtf8("Burak Şahin"),     QString::fromUtf8("İnşaat Mühendisliği"),        3, 3.90},
        {"2024007", QString::fromUtf8("Zeynep Arslan"),   QString::fromUtf8("Bilgisayar Bilimleri"),       1, 3.25},
        {"2024008", QString::fromUtf8("Emre Güneş"),      QString::fromUtf8("Yazılım Mühendisliği"),       4, 2.95},
    };
}

int OgrenciModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_ogrenciler.size());
}

int OgrenciModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 5;
}

QVariant OgrenciModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_ogrenciler.size())
        return {};

    const auto &ogr = m_ogrenciler[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return ogr.numara;
        case 1: return ogr.ad;
        case 2: return ogr.bolum;
        case 3: return ogr.sinif;
        case 4: return QString::number(ogr.gpa, 'f', 2);
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 3 || index.column() == 4)
            return QVariant(Qt::AlignCenter);
    }

    if (role == Qt::BackgroundRole) {
        if (index.column() == 4) {
            if (ogr.gpa >= 3.50)
                return QBrush(QColor(200, 255, 200));
            else if (ogr.gpa < 2.00)
                return QBrush(QColor(255, 200, 200));
        }
    }

    if (role == Qt::FontRole) {
        if (index.column() == 0) {
            QFont font;
            font.setBold(true);
            return font;
        }
    }

    if (role == Qt::ToolTipRole) {
        return QString::fromUtf8("%1 - %2. sınıf, GPA: %3")
            .arg(ogr.ad)
            .arg(ogr.sinif)
            .arg(ogr.gpa, 0, 'f', 2);
    }

    return {};
}

QVariant OgrenciModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return QString::fromUtf8("Numara");
        case 1: return QString::fromUtf8("Ad Soyad");
        case 2: return QString::fromUtf8("Bölüm");
        case 3: return QString::fromUtf8("Sınıf");
        case 4: return QString::fromUtf8("GPA");
        }
    } else {
        return section + 1;
    }

    return {};
}

bool OgrenciModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    auto &ogr = m_ogrenciler[index.row()];

    switch (index.column()) {
    case 0: ogr.numara = value.toString(); break;
    case 1: ogr.ad = value.toString(); break;
    case 2: ogr.bolum = value.toString(); break;
    case 3: ogr.sinif = value.toInt(); break;
    case 4: ogr.gpa = value.toDouble(); break;
    default: return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags OgrenciModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void OgrenciModel::ogrenciEkle(const Ogrenci &ogrenci)
{
    int satir = static_cast<int>(m_ogrenciler.size());
    beginInsertRows(QModelIndex(), satir, satir);
    m_ogrenciler.append(ogrenci);
    endInsertRows();
}

void OgrenciModel::ogrenciSil(int satir)
{
    if (satir < 0 || satir >= m_ogrenciler.size())
        return;
    beginRemoveRows(QModelIndex(), satir, satir);
    m_ogrenciler.removeAt(satir);
    endRemoveRows();
}

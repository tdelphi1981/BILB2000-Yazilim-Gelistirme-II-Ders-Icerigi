/**
 * 03_grafik_model_baglama.h -- IstatistikModel sinifi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * IstatistikModel, kaynak model olarak verilen bir QAbstractTableModel
 * (ornegin NotlarModel) uzerindeki verileri gozleyerek harf notu
 * dagilimi histogramini hesaplar ve kendisi de bir QAbstractTableModel
 * olarak davranir. Bu sayede QVBarModelMapper ile dogrudan bir
 * QBarSeries'e baglanabilir.
 *
 * Tablo yapisi:
 *   8 satir (AA, BA, BB, CB, CC, DC, DD, FF)
 *   2 sutun (Harf, Sayi)
 *
 * Kaynak modeldeki veri degistiginde kaynakDegisti() slot'u cagrilir,
 * histogram yeniden hesaplanir ve IstatistikModel kendi dataChanged
 * sinyalini yayinlar; boylece ona bagli olan QVBarModelMapper otomatik
 * olarak grafigi gunceller.
 */

#ifndef GRAFIK_MODEL_BAGLAMA_H
#define GRAFIK_MODEL_BAGLAMA_H

#include <QAbstractTableModel>
#include <QPointer>
#include <QStringList>
#include <QVector>

class IstatistikModel : public QAbstractTableModel {
    Q_OBJECT

public:
    // Kaynak modelde harf notunun bulundugu sutun indeksi
    // (NotlarModel'de 3. sutun "Harf" varsayilir).
    explicit IstatistikModel(int harfSutunu = 3,
                             QObject* parent = nullptr);

    // Izlenecek kaynak modeli baglar/degisir. Eski baglantilar kesilir.
    void setKaynakModel(QAbstractItemModel* kaynak);

    // QAbstractTableModel arayuzu
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& idx,
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orient,
                        int role = Qt::DisplayRole) const override;

private slots:
    // Kaynaktaki dataChanged / rowsInserted / modelReset sinyalleri
    // bu slota baglanir. Histogram yeniden hesaplanir ve dataChanged
    // yayinlanir.
    void kaynakDegisti();

private:
    void histogramiHesapla();

    QPointer<QAbstractItemModel> m_kaynak;
    int                          m_harfSutunu;
    QStringList                  m_harfler;   // AA..FF
    QVector<int>                 m_sayilar;   // 8 elemanli histogram
};

#endif // GRAFIK_MODEL_BAGLAMA_H

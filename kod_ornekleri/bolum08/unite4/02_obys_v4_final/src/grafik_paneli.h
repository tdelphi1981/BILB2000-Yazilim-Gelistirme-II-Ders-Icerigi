/**
 * grafik_paneli.h -- Harf notu bar chart widgeti
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * GrafikPaneli, icinde QChart + QChartView + QVBarModelMapper
 * sarmalayan bir QWidget'tir. Dardan bir QAbstractItemModel verirsiniz;
 * panel bu modeldeki "Sayi" sutununu bir QBarSeries'e bagliyarak
 * cubuk grafik uretir. Kaynak modeldeki dataChanged otomatik olarak
 * mapper araciligiyla grafige yansir.
 */

#ifndef GRAFIK_PANELI_H
#define GRAFIK_PANELI_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

namespace QtCharts {}

class QChart;
class QChartView;
class QBarSeries;
class QBarCategoryAxis;
class QValueAxis;
class QVBarModelMapper;

class GrafikPaneli : public QWidget {
    Q_OBJECT

public:
    explicit GrafikPaneli(QWidget* parent = nullptr);

    // istatistikModel -> sutun 0: Harf, sutun 1: Sayi
    void setModel(QAbstractItemModel* istatistikModel);

private:
    QChart*            m_grafik{nullptr};
    QChartView*        m_gorunum{nullptr};
    QBarSeries*        m_seri{nullptr};
    QBarCategoryAxis*  m_xEksen{nullptr};
    QValueAxis*        m_yEksen{nullptr};
    QVBarModelMapper*  m_mapper{nullptr};
};

#endif // GRAFIK_PANELI_H

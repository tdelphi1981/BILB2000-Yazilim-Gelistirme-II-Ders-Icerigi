#ifndef GRAFIK_PANELI_H
#define GRAFIK_PANELI_H

#include <QWidget>

class QAbstractItemModel;
class QChart;
class QChartView;
class QBarSeries;
class QBarCategoryAxis;
class QValueAxis;
class QVBarModelMapper;

class GrafikPaneli : public QWidget {
    Q_OBJECT
public:
    explicit GrafikPaneli(
        QWidget* parent = nullptr);

    void setModel(
        QAbstractItemModel* model);

private:
    QChart*            m_grafik{nullptr};
    QChartView*        m_gorunum{nullptr};
    QBarSeries*        m_seri{nullptr};
    QBarCategoryAxis*  m_xEksen{nullptr};
    QValueAxis*        m_yEksen{nullptr};
    QVBarModelMapper*  m_mapper{nullptr};
};

#endif // GRAFIK_PANELI_H

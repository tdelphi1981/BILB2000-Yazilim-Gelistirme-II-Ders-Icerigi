#include "grafik_paneli.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QVBarModelMapper>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QPainter>

GrafikPaneli::GrafikPaneli(QWidget* parent)
    : QWidget(parent) {
    m_seri = new QBarSeries(this);
    m_seri->setLabelsVisible(true);

    m_grafik = new QChart();
    m_grafik->addSeries(m_seri);
    m_grafik->setTitle("Harf Notu Dağılımı");
    m_grafik->setAnimationOptions(
        QChart::SeriesAnimations);
    m_grafik->legend()->setVisible(true);
    m_grafik->legend()->setAlignment(
        Qt::AlignBottom);

    QStringList harfler{
        "AA","BA","BB","CB",
        "CC","DC","DD","FF"};
    m_xEksen = new QBarCategoryAxis();
    m_xEksen->append(harfler);
    m_grafik->addAxis(m_xEksen,
                      Qt::AlignBottom);
    m_seri->attachAxis(m_xEksen);

    m_yEksen = new QValueAxis();
    m_yEksen->setRange(0, 10);
    m_yEksen->setTickCount(11);
    m_yEksen->setLabelFormat("%d");
    m_yEksen->setTitleText("Öğrenci Sayısı");
    m_grafik->addAxis(m_yEksen,
                      Qt::AlignLeft);
    m_seri->attachAxis(m_yEksen);

    m_gorunum = new QChartView(
        m_grafik, this);
    m_gorunum->setRenderHint(
        QPainter::Antialiasing);

    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(m_gorunum);
}

void GrafikPaneli::setModel(
    QAbstractItemModel* model) {
    delete m_mapper;
    m_mapper = nullptr;
    m_seri->clear();
    if (!model) return;

    m_mapper = new QVBarModelMapper(this);
    m_mapper->setFirstBarSetColumn(1);
    m_mapper->setLastBarSetColumn(1);
    m_mapper->setFirstRow(0);
    m_mapper->setRowCount(8);
    m_mapper->setSeries(m_seri);
    m_mapper->setModel(model);

    if (!m_seri->barSets().isEmpty())
        m_seri->barSets().first()->setLabel(
            "Öğrenci Sayısı");
}

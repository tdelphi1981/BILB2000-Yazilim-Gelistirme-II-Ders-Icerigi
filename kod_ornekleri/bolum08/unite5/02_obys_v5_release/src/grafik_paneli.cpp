/**
 * grafik_paneli.cpp -- GrafikPaneli uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 */

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
#include <QStringList>

GrafikPaneli::GrafikPaneli(QWidget* parent)
    : QWidget(parent)
{
    m_seri  = new QBarSeries(this);
    m_seri->setLabelsVisible(true);

    m_grafik = new QChart();
    m_grafik->addSeries(m_seri);
    m_grafik->setTitle(QStringLiteral("Harf Notu Dagilimi"));
    m_grafik->setAnimationOptions(QChart::SeriesAnimations);
    m_grafik->legend()->setVisible(true);
    m_grafik->legend()->setAlignment(Qt::AlignBottom);

    // X ekseni -- harf kategorileri
    const QStringList harfler{
        QStringLiteral("AA"), QStringLiteral("BA"),
        QStringLiteral("BB"), QStringLiteral("CB"),
        QStringLiteral("CC"), QStringLiteral("DC"),
        QStringLiteral("DD"), QStringLiteral("FF")
    };
    m_xEksen = new QBarCategoryAxis();
    m_xEksen->append(harfler);
    m_grafik->addAxis(m_xEksen, Qt::AlignBottom);
    m_seri->attachAxis(m_xEksen);

    // Y ekseni -- sayim
    m_yEksen = new QValueAxis();
    m_yEksen->setRange(0, 10);
    m_yEksen->setTickCount(11);
    m_yEksen->setLabelFormat(QStringLiteral("%d"));
    m_yEksen->setTitleText(QStringLiteral("Ogrenci Sayisi"));
    m_grafik->addAxis(m_yEksen, Qt::AlignLeft);
    m_seri->attachAxis(m_yEksen);

    m_gorunum = new QChartView(m_grafik, this);
    m_gorunum->setRenderHint(QPainter::Antialiasing);

    auto* yerlesim = new QVBoxLayout(this);
    yerlesim->setContentsMargins(0, 0, 0, 0);
    yerlesim->addWidget(m_gorunum);
}

void GrafikPaneli::setModel(QAbstractItemModel* istatistikModel) {
    if (m_mapper) {
        delete m_mapper;
        m_mapper = nullptr;
    }

    // Var olan QBarSet'leri temizle
    m_seri->clear();

    if (istatistikModel == nullptr) { return; }

    m_mapper = new QVBarModelMapper(this);
    m_mapper->setFirstBarSetColumn(1); // "Sayi" sutunu
    m_mapper->setLastBarSetColumn(1);
    m_mapper->setFirstRow(0);
    m_mapper->setRowCount(8);
    m_mapper->setSeries(m_seri);
    m_mapper->setModel(istatistikModel);

    // Olusan set icin etiket ver
    if (!m_seri->barSets().isEmpty()) {
        m_seri->barSets().first()->setLabel(
            QStringLiteral("Ogrenci Sayisi"));
    }
}

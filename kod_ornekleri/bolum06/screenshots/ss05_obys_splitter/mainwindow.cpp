// mainwindow.cpp - Splitter ile kayit formu ve tablo gorunumu uygulamasi
#include "mainwindow.h"
#include "ogrencimodel.h"

#include <QSplitter>
#include <QFormLayout>
#include <QPushButton>
#include <QWidget>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString::fromUtf8("OBYS - Öğrenci Kayıt ve Tablo"));
    resize(1000, 550);

    m_model = new OgrenciModel(this);

    // Sol panel: kayit formu
    auto *formWidget = new QWidget;
    auto *formLayout = new QFormLayout(formWidget);

    m_numaraEdit = new QLineEdit;
    m_numaraEdit->setPlaceholderText("2024009");
    formLayout->addRow(QString::fromUtf8("Numara:"), m_numaraEdit);

    m_adEdit = new QLineEdit;
    m_adEdit->setPlaceholderText(QString::fromUtf8("Ad Soyad"));
    formLayout->addRow(QString::fromUtf8("Ad Soyad:"), m_adEdit);

    m_bolumEdit = new QLineEdit;
    m_bolumEdit->setPlaceholderText(QString::fromUtf8("Bölüm"));
    formLayout->addRow(QString::fromUtf8("Bölüm:"), m_bolumEdit);

    m_sinifSpin = new QSpinBox;
    m_sinifSpin->setRange(1, 4);
    m_sinifSpin->setValue(1);
    formLayout->addRow(QString::fromUtf8("Sınıf:"), m_sinifSpin);

    m_gpaSpin = new QDoubleSpinBox;
    m_gpaSpin->setRange(0.00, 4.00);
    m_gpaSpin->setDecimals(2);
    m_gpaSpin->setSingleStep(0.05);
    m_gpaSpin->setValue(2.50);
    formLayout->addRow("GPA:", m_gpaSpin);

    auto *ekleBtn = new QPushButton(QString::fromUtf8("Ekle"));
    formLayout->addRow(ekleBtn);

    connect(ekleBtn, &QPushButton::clicked, this, &MainWindow::ogrenciEkle);

    // Sag panel: tablo
    m_tableView = new QTableView;
    m_tableView->setModel(m_model);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setAlternatingRowColors(true);

    // Splitter
    auto *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(formWidget);
    splitter->addWidget(m_tableView);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    setCentralWidget(splitter);
}

void MainWindow::ogrenciEkle()
{
    Ogrenci yeni;
    yeni.numara = m_numaraEdit->text();
    yeni.ad = m_adEdit->text();
    yeni.bolum = m_bolumEdit->text();
    yeni.sinif = m_sinifSpin->value();
    yeni.gpa = m_gpaSpin->value();

    if (!yeni.numara.isEmpty() && !yeni.ad.isEmpty()) {
        m_model->ogrenciEkle(yeni);
        m_numaraEdit->clear();
        m_adEdit->clear();
        m_bolumEdit->clear();
        m_sinifSpin->setValue(1);
        m_gpaSpin->setValue(2.50);
    }
}

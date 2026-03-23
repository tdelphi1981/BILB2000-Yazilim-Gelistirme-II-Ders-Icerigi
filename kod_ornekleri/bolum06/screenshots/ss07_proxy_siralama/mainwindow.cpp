// mainwindow.cpp - Proxy model ile siralama ve filtreleme uygulamasi
#include "mainwindow.h"
#include "ogrencimodel.h"
#include "ogrenciproxymodel.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString::fromUtf8("Proxy Model - Sıralama ve Filtreleme"));
    resize(900, 500);

    m_model = new OgrenciModel(this);
    m_proxyModel = new OgrenciProxyModel(this);
    m_proxyModel->setSourceModel(m_model);

    // Arama alani
    m_aramaEdit = new QLineEdit;
    m_aramaEdit->setPlaceholderText(QString::fromUtf8("Ara..."));
    m_aramaEdit->setClearButtonEnabled(true);

    connect(m_aramaEdit, &QLineEdit::textChanged,
            m_proxyModel, &OgrenciProxyModel::setMetinFiltresi);

    // Tablo
    m_tableView = new QTableView;
    m_tableView->setModel(m_proxyModel);
    m_tableView->setSortingEnabled(true);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->sortByColumn(0, Qt::AscendingOrder);

    // Duzenleme
    auto *centralWidget = new QWidget;
    auto *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(new QLabel(QString::fromUtf8("Filtre:")));
    layout->addWidget(m_aramaEdit);
    layout->addWidget(m_tableView);

    setCentralWidget(centralWidget);
}

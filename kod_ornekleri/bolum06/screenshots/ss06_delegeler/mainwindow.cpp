// mainwindow.cpp - Ozel delegeler ile tablo gorunumu uygulamasi
#include "mainwindow.h"
#include "ogrencimodel.h"
#include "delegeler.h"

#include <QHeaderView>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString::fromUtf8("Özel Delegeler - GPA ve Bölüm Editörleri"));
    resize(900, 500);

    m_model = new OgrenciModel(this);

    m_tableView = new QTableView;
    m_tableView->setModel(m_model);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setAlternatingRowColors(true);

    // Ozel delegeler
    m_tableView->setItemDelegateForColumn(4, new GpaDelegate(this));
    m_tableView->setItemDelegateForColumn(2, new BolumDelegate(this));

    setCentralWidget(m_tableView);
}

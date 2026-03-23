// mainwindow.cpp - OBYS arama ve filtreleme uygulamasi
#include "mainwindow.h"
#include "ogrencimodel.h"
#include "ogrenciproxymodel.h"
#include "delegeler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString::fromUtf8("OBYS - Öğrenci Bilgi Yönetim Sistemi"));
    resize(1000, 600);

    m_model = new OgrenciModel(this);
    m_proxyModel = new OgrenciProxyModel(this);
    m_proxyModel->setSourceModel(m_model);

    // Ust toolbar
    auto *toolbar = new QToolBar;
    toolbar->setMovable(false);

    auto *aramaLabel = new QLabel(QString::fromUtf8(" Ara: "));
    m_aramaEdit = new QLineEdit;
    m_aramaEdit->setPlaceholderText(QString::fromUtf8("Ad, numara veya bölüm ara..."));
    m_aramaEdit->setClearButtonEnabled(true);
    m_aramaEdit->setMinimumWidth(250);

    auto *bolumLabel = new QLabel(QString::fromUtf8("  Bölüm: "));
    m_bolumCombo = new QComboBox;
    m_bolumCombo->addItems({
        QString::fromUtf8("Tümü"),
        QString::fromUtf8("Bilgisayar Bilimleri"),
        QString::fromUtf8("Yazılım Mühendisliği"),
        QString::fromUtf8("Elektrik-Elektronik"),
        QString::fromUtf8("Makine Mühendisliği"),
        QString::fromUtf8("İnşaat Mühendisliği")
    });
    m_bolumCombo->setMinimumWidth(200);

    toolbar->addWidget(aramaLabel);
    toolbar->addWidget(m_aramaEdit);
    toolbar->addWidget(bolumLabel);
    toolbar->addWidget(m_bolumCombo);
    addToolBar(toolbar);

    // Tablo
    m_tableView = new QTableView;
    m_tableView->setModel(m_proxyModel);
    m_tableView->setSortingEnabled(true);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->sortByColumn(0, Qt::AscendingOrder);

    // Delegeler
    m_tableView->setItemDelegateForColumn(4, new GpaDelegate(this));
    m_tableView->setItemDelegateForColumn(2, new BolumDelegate(this));

    setCentralWidget(m_tableView);

    // Durum cubugu
    m_durumLabel = new QLabel;
    statusBar()->addPermanentWidget(m_durumLabel);

    // Sinyaller
    connect(m_aramaEdit, &QLineEdit::textChanged,
            this, &MainWindow::aramaGuncelle);
    connect(m_bolumCombo, &QComboBox::currentIndexChanged,
            this, &MainWindow::bolumFiltreGuncelle);

    durumCubugGuncelle();
}

void MainWindow::aramaGuncelle(const QString &metin)
{
    m_proxyModel->setMetinFiltresi(metin);
    durumCubugGuncelle();
}

void MainWindow::bolumFiltreGuncelle(int index)
{
    if (index == 0) {
        m_proxyModel->setBolumFiltresi(QString());
    } else {
        m_proxyModel->setBolumFiltresi(m_bolumCombo->currentText());
    }
    durumCubugGuncelle();
}

void MainWindow::durumCubugGuncelle()
{
    int filtrelenen = m_proxyModel->filtrelenenSayisi();
    int toplam = m_model->ogrenciSayisi();
    m_durumLabel->setText(
        QString::fromUtf8("  Gösterilen: %1 / Toplam: %2 kayıt  ")
            .arg(filtrelenen)
            .arg(toplam));
}

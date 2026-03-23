// mainwindow.h - Proxy model ile siralama ve filtreleme penceresi
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLineEdit>

class OgrenciModel;
class OgrenciProxyModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QTableView *m_tableView;
    OgrenciModel *m_model;
    OgrenciProxyModel *m_proxyModel;
    QLineEdit *m_aramaEdit;
};

#endif // MAINWINDOW_H

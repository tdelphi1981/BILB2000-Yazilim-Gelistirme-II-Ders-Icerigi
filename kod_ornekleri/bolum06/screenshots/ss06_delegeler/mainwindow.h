// mainwindow.h - Ozel delegeler ile tablo gorunumu
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

class OgrenciModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QTableView *m_tableView;
    OgrenciModel *m_model;
};

#endif // MAINWINDOW_H

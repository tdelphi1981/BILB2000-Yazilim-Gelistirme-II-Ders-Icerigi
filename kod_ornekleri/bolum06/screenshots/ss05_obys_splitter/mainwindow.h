// mainwindow.h - Splitter ile kayit formu ve tablo gorunumu
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTableView>

class OgrenciModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void ogrenciEkle();

private:
    QLineEdit *m_numaraEdit;
    QLineEdit *m_adEdit;
    QLineEdit *m_bolumEdit;
    QSpinBox *m_sinifSpin;
    QDoubleSpinBox *m_gpaSpin;
    QTableView *m_tableView;
    OgrenciModel *m_model;
};

#endif // MAINWINDOW_H

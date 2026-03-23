// mainwindow.h - OBYS arama ve filtreleme ana penceresi
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLineEdit>
#include <QComboBox>
#include <QStatusBar>
#include <QLabel>

class OgrenciModel;
class OgrenciProxyModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void aramaGuncelle(const QString &metin);
    void bolumFiltreGuncelle(int index);
    void durumCubugGuncelle();

private:
    QTableView *m_tableView;
    OgrenciModel *m_model;
    OgrenciProxyModel *m_proxyModel;
    QLineEdit *m_aramaEdit;
    QComboBox *m_bolumCombo;
    QLabel *m_durumLabel;
};

#endif // MAINWINDOW_H

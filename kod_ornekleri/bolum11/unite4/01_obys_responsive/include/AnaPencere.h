#pragma once
#include "Depo.h"
#include "HesaplamaServisi.h"
#include "Ogrenci.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include <memory>

class QPushButton;
class QStatusBar;

class AnaPencere : public QMainWindow {
    Q_OBJECT
public:
    explicit AnaPencere(QWidget* parent = nullptr);

private slots:
    void csvIceAktarTetikle();
    void csvDisaAktarTetikle();
    void gpaHesaplaTetikle();
    void rastgeleVeriOlustur();
    void tabloyuYenile();

private:
    void uiKur();
    void menuKur();

    std::shared_ptr<Depo<Ogrenci>> depo_;
    QTableView* tablo_;
    QStandardItemModel* model_;
};

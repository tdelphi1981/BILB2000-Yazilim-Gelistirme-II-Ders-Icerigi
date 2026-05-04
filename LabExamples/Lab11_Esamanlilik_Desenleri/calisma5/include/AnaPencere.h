#pragma once
#include <QMainWindow>

class QPushButton;
class QLabel;

class AnaPencere : public QMainWindow {
    Q_OBJECT
public:
    AnaPencere(QWidget* parent = nullptr);

private slots:
    void csvIceAktarTetikle();

private:
    QPushButton* iceButonu_;
    QLabel* etiket_;
};

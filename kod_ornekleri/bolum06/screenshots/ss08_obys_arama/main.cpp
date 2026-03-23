// main.cpp - OBYS arama uygulamasi giris noktasi
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow pencere;
    pencere.show();

    return app.exec();
}

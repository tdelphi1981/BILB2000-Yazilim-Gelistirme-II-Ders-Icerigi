/**
 * main.cpp -- OBYS v2 giris noktasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 *
 * QApplication'i baslatir ve MainWindow'u gosterir. Unite 1'deki
 * giris noktasi ile ayni yapidadir; fark yalnizca uygulama adida.
 */

#include "main_window.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);
    QCoreApplication::setOrganizationName(
        QStringLiteral("KTU Bilgisayar Bilimleri"));
    QCoreApplication::setApplicationName(
        QStringLiteral("OBYS v2"));

    MainWindow pencere;
    pencere.show();
    return uygulama.exec();
}

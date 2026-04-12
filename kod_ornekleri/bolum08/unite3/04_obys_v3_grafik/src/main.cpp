/**
 * main.cpp -- OBYS v3 giris noktasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * QApplication'i baslatir ve MainWindow'u gosterir. Unite 2'deki
 * giris noktasi ile ayni yapidadir; farklar uygulama adi ve
 * ana pencere icerigidir (QTabWidget ile grafik sekmeleri eklendi).
 */

#include "main_window.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);
    QCoreApplication::setOrganizationName(
        QStringLiteral("KTU Bilgisayar Bilimleri"));
    QCoreApplication::setApplicationName(
        QStringLiteral("OBYS v3"));

    MainWindow pencere;
    pencere.show();
    return uygulama.exec();
}

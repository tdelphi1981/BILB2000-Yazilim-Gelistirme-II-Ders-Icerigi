/**
 * main.cpp -- OBYS v1 giris noktasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * QApplication'i baslatir ve MainWindow'u gosterir.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Widgets ile birlikte.
 */

#include "main_window.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);
    QCoreApplication::setOrganizationName(
        QStringLiteral("KTU Bilgisayar Bilimleri"));
    QCoreApplication::setApplicationName(
        QStringLiteral("OBYS v1"));

    MainWindow pencere;
    pencere.show();
    return uygulama.exec();
}

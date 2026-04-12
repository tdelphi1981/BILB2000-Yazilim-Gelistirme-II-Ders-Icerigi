#include "main_window.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QCoreApplication::setOrganizationName("KTU");
    QCoreApplication::setApplicationName(
        "OBYS Tutorial");
    QCoreApplication::setApplicationVersion(
        "1.0.0");

    MainWindow pencere;
    pencere.show();
    return uygulama.exec();
}

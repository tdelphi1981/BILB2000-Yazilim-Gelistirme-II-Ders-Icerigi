#include "AnaPencere.h"
#include "HesaplamaServisi.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("YG2");
    app.setApplicationName("OBYS Responsive");

    qRegisterMetaType<GPASonuc>("GPASonuc");
    qRegisterMetaType<QList<GPASonuc>>("QList<GPASonuc>");
    qRegisterMetaType<qsizetype>("qsizetype");

    AnaPencere pencere;
    pencere.show();
    return app.exec();
}

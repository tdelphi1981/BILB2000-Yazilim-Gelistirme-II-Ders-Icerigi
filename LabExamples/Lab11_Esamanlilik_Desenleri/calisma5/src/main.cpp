#include "AnaPencere.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<qsizetype>("qsizetype");

    AnaPencere pencere;
    pencere.show();
    return app.exec();
}

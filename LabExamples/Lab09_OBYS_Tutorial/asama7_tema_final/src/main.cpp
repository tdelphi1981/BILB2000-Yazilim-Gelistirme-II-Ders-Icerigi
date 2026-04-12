#include "main_window.h"
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QTextStream>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QCoreApplication::setOrganizationName("KTU");
    QCoreApplication::setApplicationName(
        "OBYS Tutorial");
    QCoreApplication::setApplicationVersion(
        "1.0.0");

    // Kayitli temayi yukle
    QSettings ayarlar;
    const QString temaAd =
        ayarlar.value("gorunum/tema", "light")
               .toString();
    const QString qssYol =
        (temaAd == "dark")
            ? ":/resources/themes/dark.qss"
            : ":/resources/themes/light.qss";

    QFile dosya(qssYol);
    if (dosya.open(QIODevice::ReadOnly
                   | QIODevice::Text)) {
        QTextStream akis(&dosya);
        uygulama.setStyleSheet(akis.readAll());
    }

    MainWindow pencere;
    pencere.show();
    return uygulama.exec();
}

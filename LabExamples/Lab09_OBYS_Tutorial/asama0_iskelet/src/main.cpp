#include <QApplication>
#include <QMainWindow>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QCoreApplication::setOrganizationName("KTU");
    QCoreApplication::setApplicationName("OBYS Tutorial");
    QCoreApplication::setApplicationVersion("1.0.0");

    QMainWindow pencere;
    pencere.setWindowTitle("OBYS Tutorial");
    pencere.resize(900, 600);
    pencere.show();

    return uygulama.exec();
}

#include "obys_domain.h"

#include <QApplication>
#include <QMainWindow>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QCoreApplication::setOrganizationName("KTU");
    QCoreApplication::setApplicationName("OBYS Tutorial");
    QCoreApplication::setApplicationVersion("1.0.0");

    // Domain siniflarini test et
    Ogrenci ogr{1, "2023001", "Ayşe Yılmaz"};
    Ders ders{10, "BILB2000", "Yazılım Geliştirme II", 4};
    DersKaydi kayit{1, 10, SayisalNot{92.0}};

    qDebug() << "Öğrenci:" << ogr.adSoyad;
    qDebug() << "Ders:" << ders.ad;
    qDebug() << "Not türü indeks:" << kayit.notu.index();

    QMainWindow pencere;
    pencere.setWindowTitle("OBYS Tutorial");
    pencere.resize(900, 600);
    pencere.show();

    return uygulama.exec();
}

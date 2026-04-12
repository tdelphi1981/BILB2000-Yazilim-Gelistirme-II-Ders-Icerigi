#include "obys_domain.h"
#include "obys_servis.h"
#include "persistence_manager.h"

#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QDir>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QCoreApplication::setOrganizationName("KTU");
    QCoreApplication::setApplicationName("OBYS Tutorial");
    QCoreApplication::setApplicationVersion("1.0.0");

    // Servis testi
    OBYSServis servis;
    servis.ornekVerileriYukle();

    qDebug() << "Öğrenci sayısı:" << servis.tumOgrenciler().size();
    qDebug() << "Ders sayısı:" << servis.tumDersler().size();

    // Not ekle ve test et
    servis.dersKaydiEkle({1, 10, SayisalNot{92.0}});
    servis.dersKaydiEkle({2, 10, SayisalNot{68.0}});
    qDebug() << "Not sayısı:" << servis.tumNotlar().size();

    // Persistence testi: kaydet ve geri yükle
    PersistenceManager pm;
    const QString dosya = QDir::temp().filePath("obys_test.obys");

    if (pm.kaydet(dosya, servis)) {
        qDebug() << "Kaydedildi:" << dosya;
    } else {
        qDebug() << "Kaydetme hatası:" << pm.sonHata();
    }

    OBYSServis servis2;
    if (pm.yukle(dosya, servis2)) {
        qDebug() << "Yüklendi - Öğrenci:" << servis2.tumOgrenciler().size()
                 << "Ders:" << servis2.tumDersler().size()
                 << "Not:" << servis2.tumNotlar().size();
    } else {
        qDebug() << "Yükleme hatası:" << pm.sonHata();
    }

    QMainWindow pencere;
    pencere.setWindowTitle("OBYS Tutorial - Aşama 2: Servis Testi");
    pencere.resize(900, 600);
    pencere.show();

    return uygulama.exec();
}

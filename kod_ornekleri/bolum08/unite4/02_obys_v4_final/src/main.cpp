/**
 * main.cpp -- OBYS v4 giris noktasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 4 - OBYS GUI Mini Proje Tamamlama
 *
 * QApplication baslatildiktan sonra:
 *   1) Organizasyon/uygulama adi ve surum bilgisi set edilir
 *      (QSettings'in platform bagimsiz yol olusturabilmesi icin).
 *   2) QSettings'ten kayitli tema adi okunur ("light" veya "dark").
 *   3) Ilgili QSS dosyasi QRC uzerinden yuklenir ve uygulamaya
 *      setStyleSheet ile uygulanir.
 *   4) MainWindow gosterilir.
 *
 * Tema adi MainWindow icindeki Gorunum menusunden degistirildiginde
 * yine QSettings'e yazilir ve bir sonraki calistirmada bu ayar geri
 * yuklenir.
 */

#include "main_window.h"

#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QString>
#include <QTextStream>

namespace {

// Verilen tema adina karsilik gelen QRC yolunu doner
QString temaQssYolu(const QString& temaAd) {
    if (temaAd == QStringLiteral("dark")) {
        return QStringLiteral(":/themes/dark.qss");
    }
    return QStringLiteral(":/themes/light.qss");
}

// QRC'den QSS icerigini okur. Basarisizsa bos string doner.
QString qssYukle(const QString& yol) {
    QFile dosya(yol);
    if (!dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {};
    }
    QTextStream akis(&dosya);
    return akis.readAll();
}

} // namespace

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // QSettings icin gerekli tanimlamalar
    QCoreApplication::setOrganizationName(
        QStringLiteral("KTU Bilgisayar Bilimleri"));
    QCoreApplication::setOrganizationDomain(
        QStringLiteral("ktu.edu.tr"));
    QCoreApplication::setApplicationName(
        QStringLiteral("OBYS v4"));
    QCoreApplication::setApplicationVersion(
        QStringLiteral("1.0.0"));

    // Tema tercihini yukle (ilk calistirmada varsayilan: light)
    QSettings ayarlar;
    const QString temaAd = ayarlar.value(
        QStringLiteral("gorunum/tema"),
        QStringLiteral("light")).toString();

    const QString qss = qssYukle(temaQssYolu(temaAd));
    if (!qss.isEmpty()) {
        uygulama.setStyleSheet(qss);
    }

    MainWindow pencere;
    pencere.show();
    return uygulama.exec();
}

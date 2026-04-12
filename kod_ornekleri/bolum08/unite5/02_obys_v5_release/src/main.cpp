/**
 * main.cpp -- OBYS v5 Release giris noktasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 5 - Kurulum Hazirlama ve Dagitilabilir Paket
 *
 * Unite 4'teki main.cpp uzerine asagidaki degisiklikler yapildi:
 *   - version.h include edilir, surum bilgisi oradan okunur.
 *   - QStandardPaths::AppDataLocation altinda kullanici veri dizini
 *     otomatik olusturulur (kurulan surum icin yazilabilir alan).
 *
 * QApplication baslatildiktan sonra:
 *   1) Organizasyon/uygulama adi ve OBYS_VERSION_STRING set edilir.
 *   2) AppDataLocation dizini yoksa olusturulur.
 *   3) QSettings'ten tema adi okunur ("light" veya "dark").
 *   4) Ilgili QSS dosyasi QRC uzerinden yuklenir ve uygulamaya
 *      setStyleSheet ile uygulanir.
 *   5) MainWindow gosterilir.
 */

#include "main_window.h"
#include "version.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
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

// Kullanici veri dizinini olusturur (yoksa)
void kullaniciVeriDiziniHazirla() {
    const QString veriYol = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    if (veriYol.isEmpty()) {
        return;
    }
    QDir().mkpath(veriYol);
}

} // namespace

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // QSettings / QStandardPaths icin gerekli tanimlamalar
    QCoreApplication::setOrganizationName(
        QStringLiteral("KTU Bilgisayar Bilimleri"));
    QCoreApplication::setOrganizationDomain(
        QStringLiteral("ktu.edu.tr"));
    QCoreApplication::setApplicationName(
        QStringLiteral("OBYS"));
    // Surum bilgisi version.h'dan
    QCoreApplication::setApplicationVersion(
        QStringLiteral(OBYS_VERSION_STRING));

    // Kullanici veri dizinini hazirla (kurulu surum icin yazilabilir alan)
    kullaniciVeriDiziniHazirla();

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

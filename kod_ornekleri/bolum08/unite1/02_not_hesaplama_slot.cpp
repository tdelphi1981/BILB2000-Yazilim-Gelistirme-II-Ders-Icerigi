/**
 * 02_not_hesaplama_slot.cpp -- Harf notu hesaplama + sinyal/slot
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * Bu ornek, sayisal not degisiminde anlik harf notu hesaplayan
 * bir slot'un sinyal/slot mekanizmasi ile nasil baglandigini
 * gosterir. Bagimsiz derlenebilir kucuk bir uygulamadir.
 *
 * Derleme (tek dosyalik ornek):
 *   cmake -B build
 *   cmake --build build
 * veya manuel:
 *   moc yok, lambda tabanli oldugu icin qmake/cmake sart degil;
 *   ancak QApplication gerektigi icin Qt6::Widgets baglanmalidir.
 */

#include <QApplication>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

// Sayisal puani harf notuna ceviren saf fonksiyon.
// Yan etkisi yoktur, test edilmesi kolaydir.
static QString sayisalDanHarfe(double puan) {
    if (puan >= 90.0) return QStringLiteral("AA");
    if (puan >= 85.0) return QStringLiteral("BA");
    if (puan >= 80.0) return QStringLiteral("BB");
    if (puan >= 75.0) return QStringLiteral("CB");
    if (puan >= 70.0) return QStringLiteral("CC");
    if (puan >= 65.0) return QStringLiteral("DC");
    if (puan >= 60.0) return QStringLiteral("DD");
    if (puan >= 50.0) return QStringLiteral("FD");
    return QStringLiteral("FF");
}

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // Basit bir pencere: spinbox + harf notu etiketi
    QWidget pencere;
    pencere.setWindowTitle(QObject::tr("Harf Notu Hesaplayici"));

    auto* spin = new QDoubleSpinBox(&pencere);
    spin->setRange(0.0, 100.0);
    spin->setDecimals(1);
    spin->setSingleStep(0.5);
    spin->setValue(0.0);

    auto* etiket = new QLabel(
        QObject::tr("Harf Notu: FF"), &pencere);

    auto* yerlesim = new QVBoxLayout(&pencere);
    yerlesim->addWidget(spin);
    yerlesim->addWidget(etiket);

    // Sinyal/slot baglantisi: lambda slot kullaniyoruz.
    // Her deger degisiminde harf notu guncellenir.
    QObject::connect(
        spin,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        &pencere,
        [etiket](double yeniDeger) {
            const QString harf = sayisalDanHarfe(yeniDeger);
            etiket->setText(
                QObject::tr("Harf Notu: %1").arg(harf));
        });

    pencere.resize(260, 120);
    pencere.show();
    return uygulama.exec();
}

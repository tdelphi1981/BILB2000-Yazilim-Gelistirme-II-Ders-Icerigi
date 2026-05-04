#include "Worker.h"
#include <QFile>
#include <QTextStream>
#include <QThread>

void Worker::calistir(const QString& yol) {
    stop_ = std::stop_source{};

    QFile dosya(yol);
    if (!dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit hata(tr("Dosya acilamadi"));
        emit tamamlandi(0);
        return;
    }

    // Toplam satır sayısını ön taramayla bul
    qsizetype toplam = 0;
    while (!dosya.atEnd()) {
        dosya.readLine();
        ++toplam;
    }
    dosya.seek(0);
    if (toplam > 0) --toplam;  // başlık hariç

    QTextStream akis(&dosya);
    akis.readLine();  // başlık atla

    auto st = stop_.get_token();
    qsizetype basarili = 0, islenen = 0;

    while (!akis.atEnd()) {
        // GOREV 2: stop_token kontrolu - stop_requested ise emit hata + break

        QString satir = akis.readLine();
        // Basit parse simülasyonu (gerçek projede CSV parse)
        QThread::msleep(1);
        ++basarili;
        ++islenen;

        // GOREV 3: Her 50 satirda bir emit ilerleme(islenen, toplam)

    }

    // GOREV 4: Sonunda emit ilerleme(islenen, toplam) ve emit tamamlandi(basarili)

}

#include "IceAktarmaServisi.h"
#include <QFile>
#include <QTextStream>

IceAktarmaServisi::IceAktarmaServisi(std::shared_ptr<Depo<Ogrenci>> depo,
                                      QObject* parent)
    : QObject(parent), depo_(std::move(depo)) {}

void IceAktarmaServisi::csvIceAktar(const QString& yol) {
    stop_ = std::stop_source{};

    QFile dosya(yol);
    if (!dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit hata(tr("Dosya acilamadi: %1").arg(yol));
        emit tamamlandi(0, 0);
        return;
    }

    // Önce satır sayısını öğren (toplam için)
    qsizetype toplam = 0;
    while (!dosya.atEnd()) {
        dosya.readLine();
        ++toplam;
    }
    dosya.seek(0);
    if (toplam > 0) --toplam; // başlık hariç

    emit durum(tr("%1 satir isleniyor...").arg(toplam));

    QTextStream akis(&dosya);
    akis.readLine();   // başlığı atla

    auto st = stop_.get_token();
    qsizetype islenen = 0, basarili = 0, hataSayi = 0;

    while (!akis.atEnd()) {
        if (st.stop_requested()) {
            emit durum(tr("Iptal ediliyor..."));
            break;
        }

        QString satir = akis.readLine();
        try {
            auto o = Ogrenci::parseCSV(satir);
            depo_->ekle(o->numara(), o);
            ++basarili;
        } catch (const std::exception& ex) {
            ++hataSayi;
            emit hata(tr("Satir %1: %2")
                .arg(islenen + 1).arg(QString::fromUtf8(ex.what())));
        }

        ++islenen;
        if (islenen % 50 == 0) {
            emit ilerleme(islenen, toplam);
        }
    }

    emit ilerleme(islenen, toplam);
    emit tamamlandi(basarili, hataSayi);
}

void IceAktarmaServisi::durdur() {
    stop_.request_stop();
}

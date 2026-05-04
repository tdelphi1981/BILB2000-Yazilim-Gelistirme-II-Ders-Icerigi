#include "DisaAktarmaServisi.h"
#include <QFile>
#include <QTextStream>

DisaAktarmaServisi::DisaAktarmaServisi(std::shared_ptr<Depo<Ogrenci>> depo,
                                         QObject* parent)
    : QObject(parent), depo_(std::move(depo)) {}

void DisaAktarmaServisi::csvDisaAktar(const QString& yol) {
    stop_ = std::stop_source{};

    QFile dosya(yol);
    if (!dosya.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit hata(tr("Dosya yazilamadi: %1").arg(yol));
        emit tamamlandi(0);
        return;
    }

    auto kayitlar = depo_->tumKayitlar();
    qsizetype toplam = static_cast<qsizetype>(kayitlar.size());
    auto st = stop_.get_token();

    QTextStream akis(&dosya);
    akis << "numara,ad,bolum,gpa\n";

    qsizetype yazilan = 0;
    for (const auto& o : kayitlar) {
        if (st.stop_requested()) {
            emit durum(tr("Iptal ediliyor..."));
            break;
        }
        akis << o->toCSV() << '\n';
        ++yazilan;

        if (yazilan % 100 == 0) {
            emit ilerleme(yazilan, toplam);
        }
    }
    akis.flush();

    emit ilerleme(yazilan, toplam);
    emit tamamlandi(yazilan);
}

void DisaAktarmaServisi::durdur() {
    stop_.request_stop();
}

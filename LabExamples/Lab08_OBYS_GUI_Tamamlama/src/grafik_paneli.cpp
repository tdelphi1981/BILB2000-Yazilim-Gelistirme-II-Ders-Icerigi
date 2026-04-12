/**
 * grafik_paneli.cpp -- GrafikPaneli uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 */

#include "grafik_paneli.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QVBarModelMapper>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QPainter>
#include <QStringList>

GrafikPaneli::GrafikPaneli(QWidget* parent)
    : QWidget(parent)
{
    m_seri  = new QBarSeries(this);
    m_seri->setLabelsVisible(true);

    m_grafik = new QChart();
    m_grafik->addSeries(m_seri);
    m_grafik->setTitle(QStringLiteral("Harf Notu Dagilimi"));
    m_grafik->setAnimationOptions(QChart::SeriesAnimations);
    m_grafik->legend()->setVisible(true);
    m_grafik->legend()->setAlignment(Qt::AlignBottom);

    // X ekseni -- harf kategorileri
    const QStringList harfler{
        QStringLiteral("AA"), QStringLiteral("BA"),
        QStringLiteral("BB"), QStringLiteral("CB"),
        QStringLiteral("CC"), QStringLiteral("DC"),
        QStringLiteral("DD"), QStringLiteral("FF")
    };
    m_xEksen = new QBarCategoryAxis();
    m_xEksen->append(harfler);
    m_grafik->addAxis(m_xEksen, Qt::AlignBottom);
    m_seri->attachAxis(m_xEksen);

    // Y ekseni -- sayim
    m_yEksen = new QValueAxis();
    m_yEksen->setRange(0, 10);
    m_yEksen->setTickCount(11);
    m_yEksen->setLabelFormat(QStringLiteral("%d"));
    m_yEksen->setTitleText(QStringLiteral("Ogrenci Sayisi"));
    m_grafik->addAxis(m_yEksen, Qt::AlignLeft);
    m_seri->attachAxis(m_yEksen);

    m_gorunum = new QChartView(m_grafik, this);
    m_gorunum->setRenderHint(QPainter::Antialiasing);

    // GOREV-6: (Lab 08 / Calisma 3 / Talep 1) Dikey yerlesimin en
    // ustune "Ders:" etiketi ve QComboBox yerlestirin. ComboBox'un
    // ilk ogesi "(Tum Dersler)" metnini ve bos QVariant verisini
    // tasimalidir. QComboBox'u m_dersSecici olarak grafik_paneli.h
    // icinde saklayin.
    //
    // Ipucu:
    //     #include <QComboBox>
    //     #include <QHBoxLayout>
    //     #include <QLabel>
    //     auto* ustSerit = new QHBoxLayout();
    //     ustSerit->addWidget(new QLabel(tr("Ders:"), this));
    //     m_dersSecici = new QComboBox(this);
    //     m_dersSecici->addItem(tr("(Tum Dersler)"), QVariant{});
    //     ustSerit->addWidget(m_dersSecici, 1);

    auto* yerlesim = new QVBoxLayout(this);
    yerlesim->setContentsMargins(0, 0, 0, 0);
    // GOREV-6 (devam): Once yerlesim->addLayout(ustSerit); cagirin,
    // ardindan m_gorunum widget'ini ekleyin.
    yerlesim->addWidget(m_gorunum);

    // GOREV-8: (Lab 08 / Calisma 3 / Talep 2 ve 3) ComboBox'un
    // currentIndexChanged(int) sinyalini dersDegisti slot'una baglayin.
    // Slot, secilen derse gore m_seri'nin verilerini guncellesin.
    // "(Tum Dersler)" secildiginde birlestirilmis gorunum geri gelsin.
    // Ders hicbir not icermiyorsa m_grafik->scene() ustune "Veri yok"
    // metni icin bir QGraphicsTextItem yerlestirin.
    //
    // Ipucu:
    //     connect(m_dersSecici,
    //             qOverload<int>(&QComboBox::currentIndexChanged),
    //             this, &GrafikPaneli::dersDegisti);
}

void GrafikPaneli::dersDegisti(int indeks) {
    // GOREV-8 (devam): Secilen ders ID'sini alip m_seri icindeki
    // QBarSet verilerini bu derse gore yeniden hesaplayin.
    // indeks == 0 ise (Tum Dersler) icin tam veri gorunumu.
    //
    // Ipucu:
    //     const QVariant veri = m_dersSecici->itemData(indeks);
    //     if (!veri.isValid()) {
    //         // Tum dersler: mapper ile orijinal gorunume don
    //         return;
    //     }
    //     const int dersId = veri.toInt();
    //     // Kaynak modelin sadece bu dersin notlarini gosterdigi
    //     // yardimci bir listeden QBarSet degerlerini guncelleyin.
    (void)indeks;
}

void GrafikPaneli::setModel(QAbstractItemModel* istatistikModel) {
    if (m_mapper) {
        delete m_mapper;
        m_mapper = nullptr;
    }

    // Var olan QBarSet'leri temizle
    m_seri->clear();

    if (istatistikModel == nullptr) { return; }

    m_mapper = new QVBarModelMapper(this);
    m_mapper->setFirstBarSetColumn(1); // "Sayi" sutunu
    m_mapper->setLastBarSetColumn(1);
    m_mapper->setFirstRow(0);
    m_mapper->setRowCount(8);
    m_mapper->setSeries(m_seri);
    m_mapper->setModel(istatistikModel);

    // Olusan set icin etiket ver
    if (!m_seri->barSets().isEmpty()) {
        m_seri->barSets().first()->setLabel(
            QStringLiteral("Ogrenci Sayisi"));
    }

    // GOREV-7: (Lab 08 / Calisma 3 / Talep 1 ve 3) Mevcut ders
    // listesini m_dersSecici'ye yukleyin. Her ogenin QVariant'i ders
    // ID'sini tasisin. Ayrica istatistikModel::modelReset sinyalini
    // dinleyerek yeni ders eklendiginde ComboBox'u yeniden doldurun.
    // Sinyali yeniden doldururken currentIndexChanged'i tetiklememek
    // icin QSignalBlocker kullanin; mumkunse mevcut secim korunsun.
    //
    // Ipucu:
    //     if (m_dersSecici != nullptr) {
    //         const int onceki = m_dersSecici->currentData().toInt();
    //         QSignalBlocker bloker(m_dersSecici);
    //         m_dersSecici->clear();
    //         m_dersSecici->addItem(tr("(Tum Dersler)"), QVariant{});
    //         // dersleri servis / kaynak model uzerinden alip ekleyin
    //     }
    //     connect(istatistikModel, &QAbstractItemModel::modelReset,
    //             this, [this]() { /* ComboBox'u yenile */ });
}

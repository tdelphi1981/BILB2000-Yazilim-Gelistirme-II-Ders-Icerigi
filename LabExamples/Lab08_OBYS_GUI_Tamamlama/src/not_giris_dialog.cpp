/**
 * not_giris_dialog.cpp -- NotGirisDialog uygulamasi (proje surumu)
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * 01 numarali ornekten bu projeye kopyalanmistir. obys_domain.h
 * tarafindan saglanan NotTuru tipini kullanir.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Widgets ile birlikte.
 */

#include "not_giris_dialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

NotGirisDialog::NotGirisDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(tr("Ders Notu Girişi"));
    setModal(true);

    m_ogrenciSecici = new QComboBox(this);
    m_dersSecici    = new QComboBox(this);

    m_notTuruSecici = new QComboBox(this);
    m_notTuruSecici->addItem(tr("Sayısal"), 0);
    m_notTuruSecici->addItem(tr("Harf"),    1);
    m_notTuruSecici->addItem(tr("Muaf"),    2);

    m_notDegeri = new QDoubleSpinBox(this);
    m_notDegeri->setRange(0.0, 100.0);
    m_notDegeri->setDecimals(2);
    m_notDegeri->setSingleStep(0.5);
    m_notDegeri->setValue(0.0);

    m_harfNotuOnizleme = new QLabel(tr("Harf Önizleme: -"), this);

    // GOREV-4: (Lab 08 / Calisma 2 / Talep 2) "Fotograf Sec" butonu ve
    // yol onizleme etiketi ekleyin. Butona tiklandiginda
    // QFileDialog::getOpenFileName ile *.png *.jpg *.jpeg filtresiyle
    // dosya sectirin; kullanici iptal ederse mevcut yol korunsun.
    // Secilen yolu m_fotografYolu uyesinde saklayin ve etiket olarak
    // gosterin. m_fotografYolu uyesini not_giris_dialog.h icine de
    // ekleyin.
    //
    // Ipucu:
    //     auto* fotoBtn  = new QPushButton(tr("Fotograf Sec..."), this);
    //     m_fotografEtiket = new QLabel(tr("(secilmedi)"), this);
    //     connect(fotoBtn, &QPushButton::clicked, this, [this]() {
    //         const QString yol = QFileDialog::getOpenFileName(
    //             this, tr("Fotograf Sec"), QString(),
    //             tr("Goruntu (*.png *.jpg *.jpeg)"));
    //         if (!yol.isEmpty()) {
    //             m_fotografYolu = yol;
    //             m_fotografEtiket->setText(QFileInfo(yol).fileName());
    //         }
    //     });

    auto* form = new QFormLayout;
    form->addRow(tr("Öğrenci:"),    m_ogrenciSecici);
    form->addRow(tr("Ders:"),       m_dersSecici);
    form->addRow(tr("Not Türü:"),   m_notTuruSecici);
    form->addRow(tr("Not Değeri:"), m_notDegeri);
    form->addRow(m_harfNotuOnizleme);
    // GOREV-4 (devam): Butonu ve fotograf etiketini da form yerlesimine
    // asagidaki gibi ekleyin:
    //     form->addRow(tr("Fotograf:"), fotoBtn);
    //     form->addRow(m_fotografEtiket);

    m_butonlar = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    auto* ana = new QVBoxLayout(this);
    ana->addLayout(form);
    ana->addWidget(m_butonlar);

    connect(m_butonlar, &QDialogButtonBox::accepted,
            this,       &QDialog::accept);
    connect(m_butonlar, &QDialogButtonBox::rejected,
            this,       &QDialog::reject);

    connect(m_notDegeri,
            qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &NotGirisDialog::notDegistiginde);

    connect(m_notTuruSecici,
            qOverload<int>(&QComboBox::currentIndexChanged),
            this, &NotGirisDialog::notTuruDegistiginde);

    notTuruDegistiginde(0);
}

void NotGirisDialog::ogrencileriYukle(
    const QList<QPair<int, QString>>& ogrenciler) {
    m_ogrenciSecici->clear();
    for (const auto& [id, ad] : ogrenciler) {
        m_ogrenciSecici->addItem(ad, id);
    }
}

void NotGirisDialog::dersleriYukle(
    const QList<QPair<int, QString>>& dersler) {
    m_dersSecici->clear();
    for (const auto& [id, ad] : dersler) {
        m_dersSecici->addItem(ad, id);
    }
}

NotDTO NotGirisDialog::notDTO() const {
    NotDTO dto;
    dto.ogrenciId = m_ogrenciSecici->currentData().toInt();
    dto.dersId    = m_dersSecici->currentData().toInt();

    const int turIdx = m_notTuruSecici->currentIndex();
    switch (turIdx) {
        case 0:
            dto.notu = SayisalNot{m_notDegeri->value()};
            break;
        case 1: {
            HarfNotu h;
            h.harf = sayisalDanHarfe(m_notDegeri->value());
            dto.notu = h;
            break;
        }
        case 2:
            dto.notu = MuafNot{};
            break;
        default:
            dto.notu = SayisalNot{0.0};
            break;
    }
    return dto;
}

void NotGirisDialog::notDegistiginde(double yeniDeger) {
    const QString harf = sayisalDanHarfe(yeniDeger);
    m_harfNotuOnizleme->setText(
        tr("Harf Önizleme: %1").arg(harf));
}

void NotGirisDialog::notTuruDegistiginde(int indeks) {
    const bool sayisalAktif = (indeks != 2);
    m_notDegeri->setEnabled(sayisalAktif);
    m_harfNotuOnizleme->setVisible(sayisalAktif);
    if (sayisalAktif) {
        notDegistiginde(m_notDegeri->value());
    } else {
        m_harfNotuOnizleme->setText(tr("Harf Önizleme: MUAF"));
    }
}

QString NotGirisDialog::sayisalDanHarfe(double puan) {
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

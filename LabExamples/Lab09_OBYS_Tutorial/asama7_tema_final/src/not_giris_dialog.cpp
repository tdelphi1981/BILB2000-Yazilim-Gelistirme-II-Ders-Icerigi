#include "not_giris_dialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QVBoxLayout>

NotGirisDialog::NotGirisDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Ders Notu Girişi");
    setModal(true);

    m_ogrenciSecici = new QComboBox(this);
    m_dersSecici    = new QComboBox(this);

    m_notTuruSecici = new QComboBox(this);
    m_notTuruSecici->addItem("Sayısal", 0);
    m_notTuruSecici->addItem("Harf",    1);
    m_notTuruSecici->addItem("Muaf",    2);

    m_notDegeri = new QDoubleSpinBox(this);
    m_notDegeri->setRange(0.0, 100.0);
    m_notDegeri->setDecimals(2);
    m_notDegeri->setSingleStep(0.5);

    m_harfOnizleme = new QLabel(
        "Harf Önizleme: -", this);

    auto* form = new QFormLayout;
    form->addRow("Öğrenci:", m_ogrenciSecici);
    form->addRow("Ders:",    m_dersSecici);
    form->addRow("Not Türü:", m_notTuruSecici);
    form->addRow("Not Değeri:", m_notDegeri);
    form->addRow(m_harfOnizleme);

    m_butonlar = new QDialogButtonBox(
        QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel, this);

    auto* ana = new QVBoxLayout(this);
    ana->addLayout(form);
    ana->addWidget(m_butonlar);

    connect(m_butonlar,
            &QDialogButtonBox::accepted,
            this, &QDialog::accept);
    connect(m_butonlar,
            &QDialogButtonBox::rejected,
            this, &QDialog::reject);

    connect(m_notDegeri,
            qOverload<double>(
                &QDoubleSpinBox::valueChanged),
            this,
            &NotGirisDialog::notDegistiginde);
    connect(m_notTuruSecici,
            qOverload<int>(
                &QComboBox::currentIndexChanged),
            this,
            &NotGirisDialog::notTuruDegistiginde);

    notTuruDegistiginde(0);
}

void NotGirisDialog::ogrencileriYukle(
    const QList<QPair<int, QString>>& liste) {
    m_ogrenciSecici->clear();
    for (const auto& [id, ad] : liste) {
        m_ogrenciSecici->addItem(ad, id);
    }
}

void NotGirisDialog::dersleriYukle(
    const QList<QPair<int, QString>>& liste) {
    m_dersSecici->clear();
    for (const auto& [id, ad] : liste) {
        m_dersSecici->addItem(ad, id);
    }
}

NotDTO NotGirisDialog::notDTO() const {
    NotDTO dto;
    dto.ogrenciId =
        m_ogrenciSecici->currentData().toInt();
    dto.dersId =
        m_dersSecici->currentData().toInt();

    switch (m_notTuruSecici->currentIndex()) {
    case 0:
        dto.notu = SayisalNot{m_notDegeri->value()};
        break;
    case 1: {
        HarfNotu h;
        h.harf = sayisalDanHarfe(
                     m_notDegeri->value());
        dto.notu = h;
        break;
    }
    case 2:
        dto.notu = MuafNot{};
        break;
    }
    return dto;
}

void NotGirisDialog::notDegistiginde(
    double yeniDeger) {
    m_harfOnizleme->setText(
        QString("Harf: %1")
            .arg(sayisalDanHarfe(yeniDeger)));
}

void NotGirisDialog::notTuruDegistiginde(
    int indeks) {
    const bool aktif = (indeks != 2);
    m_notDegeri->setEnabled(aktif);
    m_harfOnizleme->setVisible(aktif);
    if (!aktif) {
        m_harfOnizleme->setText("Harf: MUAF");
    } else {
        notDegistiginde(m_notDegeri->value());
    }
}

QString NotGirisDialog::sayisalDanHarfe(
    double puan) {
    if (puan >= 90.0) return "AA";
    if (puan >= 85.0) return "BA";
    if (puan >= 80.0) return "BB";
    if (puan >= 75.0) return "CB";
    if (puan >= 70.0) return "CC";
    if (puan >= 65.0) return "DC";
    if (puan >= 60.0) return "DD";
    if (puan >= 50.0) return "FD";
    return "FF";
}

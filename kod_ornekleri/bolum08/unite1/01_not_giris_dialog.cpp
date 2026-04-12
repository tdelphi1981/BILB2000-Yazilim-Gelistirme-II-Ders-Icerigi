/**
 * 01_not_giris_dialog.cpp -- NotGirisDialog uygulamasi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * NotGirisDialog sinifinin uygulamasini icerir. Form alanlarini
 * kurar, sinyal/slot baglantilarini yapar ve DTO uretir.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Widgets ile birlikte.
 */

#include "01_not_giris_dialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QVBoxLayout>

NotGirisDialog::NotGirisDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(tr("Ders Notu Girişi"));
    setModal(true);

    // Form alanlari
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

    // Form yerlesimi
    auto* form = new QFormLayout;
    form->addRow(tr("Öğrenci:"),    m_ogrenciSecici);
    form->addRow(tr("Ders:"),       m_dersSecici);
    form->addRow(tr("Not Türü:"),   m_notTuruSecici);
    form->addRow(tr("Not Değeri:"), m_notDegeri);
    form->addRow(m_harfNotuOnizleme);

    m_butonlar = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    auto* ana = new QVBoxLayout(this);
    ana->addLayout(form);
    ana->addWidget(m_butonlar);

    // Sinyal/slot baglantilari (yeni syntax)
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

    // Baslangicta sayisal not modu aktif
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
        case 0: // Sayisal
            dto.notu = SayisalNot{m_notDegeri->value()};
            break;
        case 1: { // Harf: sayisal degerden harf uret
            HarfNotu h;
            h.harf = sayisalDanHarfe(m_notDegeri->value());
            dto.notu = h;
            break;
        }
        case 2: // Muaf
            dto.notu = MuafNot{};
            break;
        default:
            dto.notu = SayisalNot{0.0};
            break;
    }
    return dto;
}

void NotGirisDialog::notDegistiginde(double yeniDeger) {
    // Anlik harf notu onizlemesi
    const QString harf = sayisalDanHarfe(yeniDeger);
    m_harfNotuOnizleme->setText(
        tr("Harf Önizleme: %1").arg(harf));
}

void NotGirisDialog::notTuruDegistiginde(int indeks) {
    // Muaf secildiginde not degeri devre disi
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
    // KTU benzeri bagil olmayan basit eslem
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

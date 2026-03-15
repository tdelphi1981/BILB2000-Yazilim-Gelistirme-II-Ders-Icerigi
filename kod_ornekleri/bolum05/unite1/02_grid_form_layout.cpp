/**
 * Grid ve Form Düzenleri - QGridLayout ve QFormLayout
 *
 * Bu program, QGridLayout ile ızgara düzeni (hesap makinesi tarzı buton
 * ızgarası) ve QFormLayout ile form düzeni (öğrenci bilgi formu) kullanımını
 * gösterir. QGridLayout'ta satır/sütun birleştirme (spanning) tekniği de
 * örneklenmektedir.
 *
 * Öğrenilecek kavramlar:
 * - QGridLayout ile ızgara düzeni
 * - addWidget(widget, row, col, rowSpan, colSpan) kullanımı
 * - QFormLayout ile etiket-widget çiftleri
 * - addRow() ile form satırı ekleme
 * - QTabWidget ile sekmeli arayüz
 *
 * Bölüm: 05 - Qt Pencere ve Düzen Yönetimi
 * Ünite: 1 - Düzen Yöneticileri
 *
 * Derleme (CMake önerilir, CMakeLists.txt dosyasına bakınız):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 *
 * Doğrudan derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 02_grid_form_layout 02_grid_form_layout.cpp
 */

#include <QApplication>
#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) ANA PENCERE VE SEKMELER
    // =============================================
    QWidget pencere;
    pencere.setWindowTitle("Grid ve Form Düzenleri");
    pencere.setMinimumSize(500, 400);

    QVBoxLayout* anaDuzen = new QVBoxLayout(&pencere);

    QTabWidget* sekmeler = new QTabWidget();
    anaDuzen->addWidget(sekmeler);

    // =============================================
    // 2) SEKME 1: QGridLayout - Hesap Makinesi
    // =============================================
    QWidget* gridSekme = new QWidget();
    QVBoxLayout* gridAnaDuzen = new QVBoxLayout(gridSekme);

    QLabel* gridBaslik = new QLabel("QGridLayout - Hesap Makinesi Düzeni");
    gridBaslik->setStyleSheet("font-weight: bold; color: #2c3e50; padding: 5px;");
    gridAnaDuzen->addWidget(gridBaslik);

    // Sonuç etiketi - 4 sütunu kaplar (spanning)
    QGridLayout* gridDuzen = new QGridLayout();
    gridDuzen->setSpacing(5);

    QLabel* sonucEtiket = new QLabel("0");
    sonucEtiket->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    sonucEtiket->setStyleSheet(
        "font-size: 24px; background-color: #2c3e50; color: white; "
        "padding: 15px; border-radius: 6px;"
    );
    // addWidget(widget, satir, sutun, satirSpan, sutunSpan)
    gridDuzen->addWidget(sonucEtiket, 0, 0, 1, 4);  // 1 satır, 4 sütun kapla

    // Hesap makinesi butonları
    QStringList butonMetinleri = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", ".", "=", "+"
    };

    int satir = 1;
    int sutun = 0;
    QString butonStili =
        "QPushButton { font-size: 18px; padding: 15px; border-radius: 4px; "
        "border: 1px solid #bdc3c7; }"
        "QPushButton:hover { background-color: #eaf2f8; }";

    for (const auto& metin : butonMetinleri) {
        QPushButton* btn = new QPushButton(metin);
        btn->setStyleSheet(butonStili);
        gridDuzen->addWidget(btn, satir, sutun);

        // Butona tıklayınca sonuç etiketini güncelle
        QObject::connect(btn, &QPushButton::clicked, sonucEtiket,
                         [sonucEtiket, metin]() {
            sonucEtiket->setText(sonucEtiket->text() + metin);
        });

        sutun++;
        if (sutun > 3) {
            sutun = 0;
            satir++;
        }
    }

    // "C" butonu - 2 sütun kaplar
    QPushButton* temizleBtn = new QPushButton("C");
    temizleBtn->setStyleSheet(
        "QPushButton { font-size: 18px; padding: 15px; border-radius: 4px; "
        "background-color: #e74c3c; color: white; }"
        "QPushButton:hover { background-color: #c0392b; }"
    );
    gridDuzen->addWidget(temizleBtn, satir, 0, 1, 2);  // 2 sütun kapla

    QObject::connect(temizleBtn, &QPushButton::clicked, sonucEtiket,
                     [sonucEtiket]() { sonucEtiket->setText("0"); });

    gridAnaDuzen->addLayout(gridDuzen);
    sekmeler->addTab(gridSekme, "Grid Düzen");

    // =============================================
    // 3) SEKME 2: QFormLayout - Öğrenci Formu
    // =============================================
    QWidget* formSekme = new QWidget();
    QVBoxLayout* formAnaDuzen = new QVBoxLayout(formSekme);

    QLabel* formBaslik = new QLabel("QFormLayout - Öğrenci Bilgi Formu");
    formBaslik->setStyleSheet("font-weight: bold; color: #2c3e50; padding: 5px;");
    formAnaDuzen->addWidget(formBaslik);

    QFormLayout* formDuzen = new QFormLayout();
    formDuzen->setSpacing(10);
    formDuzen->setLabelAlignment(Qt::AlignRight);

    // Form alanları
    QLineEdit* adGiris = new QLineEdit();
    adGiris->setPlaceholderText("Örneğin: Ayşe Yılmaz");
    formDuzen->addRow("Ad Soyad:", adGiris);

    QSpinBox* numaraGiris = new QSpinBox();
    numaraGiris->setRange(1000, 9999);
    formDuzen->addRow("Öğrenci No:", numaraGiris);

    QComboBox* bolumSecim = new QComboBox();
    bolumSecim->addItems({"-- Seçiniz --", "Bilgisayar Bilimleri",
                          "Yazılım Mühendisliği", "Matematik"});
    formDuzen->addRow("Bölüm:", bolumSecim);

    QSpinBox* sinifGiris = new QSpinBox();
    sinifGiris->setRange(1, 4);
    formDuzen->addRow("Sınıf:", sinifGiris);

    formAnaDuzen->addLayout(formDuzen);
    formAnaDuzen->addStretch();

    sekmeler->addTab(formSekme, "Form Düzen");

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "Grid ve Form Düzenleri"
 * Boyut: 500x400 piksel (minimum)
 *
 * Sekme 1 - "Grid Düzen":
 *   [          0          ]   (sonuç etiketi, 4 sütun kaplar)
 *   [7] [8] [9] [/]
 *   [4] [5] [6] [*]
 *   [1] [2] [3] [-]
 *   [0] [.] [=] [+]
 *   [   C   ]               (2 sütun kaplar, kırmızı)
 *
 * Sekme 2 - "Form Düzen":
 *   Ad Soyad:     [__________________]
 *   Öğrenci No:   [1000 ^v]
 *   Bölüm:        [-- Seçiniz --]
 *   Sınıf:        [1 ^v]
 *
 * Davranış:
 *   - Hesap makinesi butonlarına tıklayınca sonuç etiketine metin eklenir
 *   - "C" butonu sonuç etiketini "0" yapar
 *   - Form alanları QFormLayout ile etiket-widget hizalıdır
 */

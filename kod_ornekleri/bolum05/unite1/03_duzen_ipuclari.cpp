/**
 * Düzen İpuçları - Stretch, Spacing, Margins ve QSizePolicy
 *
 * Bu program, düzen yöneticilerinde ince ayar yapılmasını gösterir.
 * Stretch faktörleri ile widget'ların oransal genişlemesi, kenar
 * boşlukları (margins), widget arası boşluk (spacing) ve
 * QSizePolicy ile boyutlandırma politikaları örneklenir.
 *
 * Öğrenilecek kavramlar:
 * - Stretch faktörleri (addWidget ile oransal genişleme)
 * - setContentsMargins() ile kenar boşlukları
 * - setSpacing() ile widget arası boşluk
 * - QSizePolicy::Fixed, Expanding, Preferred
 * - Minimum/Maximum boyut kısıtlamaları
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
 *       -o 03_duzen_ipuclari 03_duzen_ipuclari.cpp
 */

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSizePolicy>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QWidget pencere;
    pencere.setWindowTitle("Düzen İpuçları");
    pencere.setMinimumSize(600, 450);

    QVBoxLayout* anaDuzen = new QVBoxLayout(&pencere);
    anaDuzen->setSpacing(15);
    anaDuzen->setContentsMargins(20, 20, 20, 20);

    // =============================================
    // 1) STRETCH FAKTÖRLERI
    // =============================================
    QGroupBox* stretchGrup = new QGroupBox("Stretch Faktörleri");
    QHBoxLayout* stretchDuzen = new QHBoxLayout(stretchGrup);

    QPushButton* btn1 = new QPushButton("Stretch: 1");
    btn1->setStyleSheet("background-color: #3498db; color: white; padding: 10px;");

    QPushButton* btn2 = new QPushButton("Stretch: 2");
    btn2->setStyleSheet("background-color: #2ecc71; color: white; padding: 10px;");

    QPushButton* btn3 = new QPushButton("Stretch: 3");
    btn3->setStyleSheet("background-color: #e74c3c; color: white; padding: 10px;");

    // addWidget(widget, stretch) - stretch oranı belirler
    stretchDuzen->addWidget(btn1, 1);  // 1/6 alan
    stretchDuzen->addWidget(btn2, 2);  // 2/6 alan
    stretchDuzen->addWidget(btn3, 3);  // 3/6 alan

    anaDuzen->addWidget(stretchGrup);

    // =============================================
    // 2) SPACING VE MARGINS
    // =============================================
    QGroupBox* spacingGrup = new QGroupBox("Spacing ve Margins Karşılaştırması");
    QHBoxLayout* spacingAnaDuzen = new QHBoxLayout(spacingGrup);

    // Sol taraf: dar boşluklar
    QGroupBox* darGrup = new QGroupBox("Dar (spacing:2, margins:5)");
    QVBoxLayout* darDuzen = new QVBoxLayout(darGrup);
    darDuzen->setSpacing(2);
    darDuzen->setContentsMargins(5, 5, 5, 5);
    for (int i = 1; i <= 3; ++i) {
        QLabel* etiket = new QLabel(QString("Eleman %1").arg(i));
        etiket->setStyleSheet("background-color: #d5f5e3; padding: 4px;");
        darDuzen->addWidget(etiket);
    }

    // Sağ taraf: geniş boşluklar
    QGroupBox* genisGrup = new QGroupBox("Geniş (spacing:15, margins:20)");
    QVBoxLayout* genisDuzen = new QVBoxLayout(genisGrup);
    genisDuzen->setSpacing(15);
    genisDuzen->setContentsMargins(20, 20, 20, 20);
    for (int i = 1; i <= 3; ++i) {
        QLabel* etiket = new QLabel(QString("Eleman %1").arg(i));
        etiket->setStyleSheet("background-color: #eaf2f8; padding: 4px;");
        genisDuzen->addWidget(etiket);
    }

    spacingAnaDuzen->addWidget(darGrup);
    spacingAnaDuzen->addWidget(genisGrup);
    anaDuzen->addWidget(spacingGrup);

    // =============================================
    // 3) QSIZEPOLICY ÖRNEKLERİ
    // =============================================
    QGroupBox* policyGrup = new QGroupBox("QSizePolicy Örnekleri");
    QVBoxLayout* policyDuzen = new QVBoxLayout(policyGrup);

    // Fixed: Sabit boyutlu widget
    QPushButton* fixedBtn = new QPushButton("Fixed (Sabit - 150px)");
    fixedBtn->setFixedWidth(150);
    fixedBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    fixedBtn->setStyleSheet("background-color: #9b59b6; color: white; padding: 8px;");

    // Expanding: Genişleyebilen widget
    QPushButton* expandBtn = new QPushButton("Expanding (Genişleyebilir)");
    expandBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    expandBtn->setStyleSheet("background-color: #e67e22; color: white; padding: 8px;");

    // Minimum: En az kendi boyutu kadar
    QPushButton* minBtn = new QPushButton("Minimum");
    minBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    minBtn->setStyleSheet("background-color: #1abc9c; color: white; padding: 8px;");

    policyDuzen->addWidget(fixedBtn);
    policyDuzen->addWidget(expandBtn);
    policyDuzen->addWidget(minBtn);

    anaDuzen->addWidget(policyGrup);

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "Düzen İpuçları"
 * Boyut: 600x450 piksel (minimum)
 *
 * İçerik:
 *   [Stretch Faktörleri]
 *     [Stretch:1][  Stretch:2  ][     Stretch:3     ]
 *     (1:2:3 oranında genişler)
 *
 *   [Spacing ve Margins Karşılaştırması]
 *     Sol: Dar boşluklar       | Sağ: Geniş boşluklar
 *     [Eleman 1]               | [  Eleman 1  ]
 *     [Eleman 2]               | [  Eleman 2  ]
 *     [Eleman 3]               | [  Eleman 3  ]
 *
 *   [QSizePolicy Örnekleri]
 *     [Fixed(150px)]           (sabit, pencere büyüse de değişmez)
 *     [====== Expanding ======] (pencere ile birlikte genişler)
 *     [Minimum]                (en az kendi boyutu kadar yer kaplar)
 *
 * Davranış:
 *   - Pencere büyütüldüğünde Stretch butonları oransal olarak genişler
 *   - Fixed buton her zaman 150px genişlikte kalır
 *   - Expanding buton tüm boş alanı kaplar
 */

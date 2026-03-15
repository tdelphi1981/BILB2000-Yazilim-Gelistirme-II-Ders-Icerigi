/**
 * QPushButton ve QLabel - Buton Çeşitleri ve Etiket Özellikleri
 *
 * Bu program, Qt'nin en temel iki widget'ini detaylı olarak gösterir:
 * QPushButton (buton) ve QLabel (etiket). İkonlu buton, toggle (açma/
 * kapama) buton, HTML içerikli etiket, QPixmap ile görsel gösteren
 * etiket ve Qt Style Sheets (QSS) ile farklı buton stilleri
 * örneklenmiştir. OBYS uygulamasında bu widget'lar her yerde karşınıza
 * çıkacaktır.
 *
 * Öğrenilecek kavramlar:
 * - QPushButton: metin, ikon (QIcon), toggle (checkable) modu
 * - QLabel: düz metin, HTML zengin metin, QPixmap ile görsel
 * - Qt Style Sheets (QSS) ile widget görünümünü özelleştirme
 * - QSS pseudo-selector'lar: :hover, :pressed, :checked
 * - QVBoxLayout ve QHBoxLayout ile yerleştirme
 * - setToolTip() ile ipucu metni
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 4 - Temel Widget'lar
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 01_buton_etiket 01_buton_etiket.cpp
 *
 * Çalıştırma: ./01_buton_etiket
 */

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QStyle>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) ANA PENCERE
    // =============================================
    QWidget pencere;
    pencere.setWindowTitle("QPushButton ve QLabel Örnekleri");
    pencere.setMinimumSize(500, 550);

    QVBoxLayout* anaDuzen = new QVBoxLayout(&pencere);
    anaDuzen->setSpacing(12);
    anaDuzen->setContentsMargins(15, 15, 15, 15);

    // =============================================
    // 2) QLABEL - DÜZ METİN
    // =============================================
    QLabel* baslikEtiket = new QLabel("QPushButton ve QLabel Örnekleri");
    baslikEtiket->setAlignment(Qt::AlignCenter);
    baslikEtiket->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #2c3e50; "
        "padding: 10px;"
    );
    anaDuzen->addWidget(baslikEtiket);

    // =============================================
    // 3) QLABEL - HTML ZENGİN METİN
    // =============================================
    // QLabel, zengin metin (Rich Text) için HTML destekler.
    // setTextFormat(Qt::RichText) ile HTML açıkça etkinleştirilebilir,
    // ancak Qt çoğu zaman otomatik olarak algılar.
    QLabel* htmlEtiket = new QLabel();
    htmlEtiket->setTextFormat(Qt::RichText);
    htmlEtiket->setText(
        "<h3 style='color:#2c3e50; margin:0;'>OBYS Bilgi Paneli</h3>"
        "<p>Öğrenci sayısı: <b style='color:#27ae60;'>142</b></p>"
        "<p>Aktif dersler: <b style='color:#2980b9;'>18</b></p>"
        "<p>Ortalama başarı: <i style='color:#e67e22;'>%78.5</i></p>"
    );
    htmlEtiket->setWordWrap(true);
    htmlEtiket->setStyleSheet(
        "padding: 12px; background-color: #fdfefe; "
        "border: 2px solid #d5dbdb; border-radius: 8px;"
    );
    anaDuzen->addWidget(htmlEtiket);

    // =============================================
    // 4) QLABEL - QPIXMAP İLE GÖRSEL
    // =============================================
    // Gerçek bir görsel dosyası yerine, QPixmap ile programatik
    // olarak renkli bir dikdörtgen oluşturuyoruz. Gerçek projede
    // QPixmap("dosya_yolu.png") ile dosyadan yüklersiniz.
    QLabel* gorselEtiket = new QLabel();
    QPixmap kutuPixmap(120, 60);
    kutuPixmap.fill(QColor("#3498db"));  // Mavi dikdörtgen

    gorselEtiket->setPixmap(kutuPixmap);
    gorselEtiket->setAlignment(Qt::AlignCenter);
    gorselEtiket->setToolTip("Bu bir QPixmap görseli (120x60 piksel mavi dikdörtgen)");
    gorselEtiket->setStyleSheet(
        "padding: 5px; border: 2px dashed #bdc3c7; border-radius: 5px;"
    );
    anaDuzen->addWidget(gorselEtiket);

    // =============================================
    // 5) QPUSHBUTTON - İKONLU BUTONLAR
    // =============================================
    // Qt'nin yerleşik standart ikonlarını (QStyle::StandardPixmap)
    // kullanarak platform bağımsız ikonlu butonlar oluşturabiliriz.
    QHBoxLayout* ikonSatir = new QHBoxLayout();

    QPushButton* kaydetButon = new QPushButton("Kaydet");
    QIcon kaydetIkon = uygulama.style()->standardIcon(QStyle::SP_DialogSaveButton);
    kaydetButon->setIcon(kaydetIkon);
    kaydetButon->setIconSize(QSize(20, 20));
    kaydetButon->setToolTip("Kaydetme butonu (standart kaydet ikonu)");
    kaydetButon->setStyleSheet(
        "font-size: 14px; padding: 8px 16px; "
        "background-color: #27ae60; color: white; "
        "border-radius: 5px; border: none;"
    );

    QPushButton* bilgiButon = new QPushButton("Bilgi");
    QIcon bilgiIkon = uygulama.style()->standardIcon(QStyle::SP_MessageBoxInformation);
    bilgiButon->setIcon(bilgiIkon);
    bilgiButon->setIconSize(QSize(20, 20));
    bilgiButon->setStyleSheet(
        "font-size: 14px; padding: 8px 16px; "
        "background-color: #3498db; color: white; "
        "border-radius: 5px; border: none;"
    );

    QPushButton* uyariButon = new QPushButton("Uyarı");
    QIcon uyariIkon = uygulama.style()->standardIcon(QStyle::SP_MessageBoxWarning);
    uyariButon->setIcon(uyariIkon);
    uyariButon->setIconSize(QSize(20, 20));
    uyariButon->setStyleSheet(
        "font-size: 14px; padding: 8px 16px; "
        "background-color: #e67e22; color: white; "
        "border-radius: 5px; border: none;"
    );

    ikonSatir->addWidget(kaydetButon);
    ikonSatir->addWidget(bilgiButon);
    ikonSatir->addWidget(uyariButon);
    anaDuzen->addLayout(ikonSatir);

    // =============================================
    // 6) QPUSHBUTTON - TOGGLE (CHECKABLE) BUTON
    // =============================================
    // setCheckable(true) ile buton açık/kapalı durumuna sahip olur.
    // toggled(bool) sinyali durum değişiminde tetiklenir.
    // QSS :checked pseudo-selector'u ile farklı stiller uygulanabilir.
    QLabel* toggleBaslik = new QLabel("Bildirim Ayarları:");
    toggleBaslik->setStyleSheet("font-size: 14px; font-weight: bold; color: #34495e;");

    QPushButton* bildirimToggle = new QPushButton("Bildirimler: KAPALI");
    bildirimToggle->setCheckable(true);
    bildirimToggle->setChecked(false);
    bildirimToggle->setStyleSheet(
        "QPushButton {"
        "  background-color: #95a5a6; color: white;"
        "  font-size: 13px; padding: 8px; "
        "  border-radius: 5px; border: none;"
        "}"
        "QPushButton:checked {"
        "  background-color: #27ae60;"
        "}"
    );

    QLabel* toggleDurum = new QLabel("Bildirimler devrede değil.");
    toggleDurum->setStyleSheet("font-size: 12px; color: #7f8c8d;");

    QObject::connect(bildirimToggle, &QPushButton::toggled,
                     toggleDurum, [bildirimToggle, toggleDurum](bool acik) {
        if (acik) {
            bildirimToggle->setText("Bildirimler: AÇIK");
            toggleDurum->setText("E-posta ve SMS bildirimleri aktif.");
            toggleDurum->setStyleSheet("font-size: 12px; color: #27ae60;");
        } else {
            bildirimToggle->setText("Bildirimler: KAPALI");
            toggleDurum->setText("Bildirimler devrede değil.");
            toggleDurum->setStyleSheet("font-size: 12px; color: #7f8c8d;");
        }
    });

    anaDuzen->addWidget(toggleBaslik);
    anaDuzen->addWidget(bildirimToggle);
    anaDuzen->addWidget(toggleDurum);

    // =============================================
    // 7) QT STYLE SHEETS (QSS) İLE STİL ÖRNEKLERİ
    // =============================================
    // QSS, CSS benzeri bir söz dizimiyle widget görünümünü değiştirir.
    // :hover (fare üzerinde), :pressed (basılmış), :checked (işaretli)
    // gibi pseudo-selector'lar desteklenir.
    QLabel* qssBaslik = new QLabel("Qt Style Sheets (QSS) Örnekleri:");
    qssBaslik->setStyleSheet("font-size: 13px; color: #7f8c8d; margin-top: 5px;");
    anaDuzen->addWidget(qssBaslik);

    QHBoxLayout* qssSatir = new QHBoxLayout();

    // Yuvarlak buton
    QPushButton* yuvarlakButon = new QPushButton("Yuvarlak");
    yuvarlakButon->setFixedSize(80, 80);
    yuvarlakButon->setStyleSheet(
        "QPushButton {"
        "  font-size: 13px;"
        "  background-color: #e74c3c; color: white;"
        "  border-radius: 40px; border: none;"
        "}"
        "QPushButton:hover {"
        "  background-color: #c0392b;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #a93226;"
        "}"
    );

    // Gradyan buton
    QPushButton* gradyanButon = new QPushButton("Gradyan");
    gradyanButon->setStyleSheet(
        "QPushButton {"
        "  font-size: 13px; padding: 10px 20px;"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "    stop:0 #6c5ce7, stop:1 #a29bfe);"
        "  color: white; border: none; border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "    stop:0 #5b4cdb, stop:1 #8e85f5);"
        "}"
    );

    // Çerçeveli buton (outline style)
    QPushButton* cerceveButon = new QPushButton("Çerçeveli");
    cerceveButon->setStyleSheet(
        "QPushButton {"
        "  font-size: 13px; padding: 10px 20px;"
        "  background-color: transparent;"
        "  color: #2ecc71; border: 2px solid #2ecc71;"
        "  border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #2ecc71; color: white;"
        "}"
    );

    qssSatir->addWidget(yuvarlakButon);
    qssSatir->addWidget(gradyanButon);
    qssSatir->addWidget(cerceveButon);
    anaDuzen->addLayout(qssSatir);

    // =============================================
    // 8) DURUM ETİKETİ VE BAĞLANTI
    // =============================================
    QLabel* durumEtiket = new QLabel("Bir butona tıklayın...");
    durumEtiket->setAlignment(Qt::AlignCenter);
    durumEtiket->setStyleSheet(
        "font-size: 13px; padding: 8px; "
        "background-color: #f0f0f0; border-radius: 5px;"
    );
    anaDuzen->addWidget(durumEtiket);

    // Her butona tıklama bilgisi bağla
    QObject::connect(kaydetButon, &QPushButton::clicked,
                     durumEtiket, [durumEtiket]() {
        durumEtiket->setText("Kaydet butonuna tıklandı!");
        durumEtiket->setStyleSheet(
            "font-size: 13px; padding: 8px; "
            "background-color: #d5f5e3; border-radius: 5px; color: #27ae60;"
        );
    });

    QObject::connect(bilgiButon, &QPushButton::clicked,
                     durumEtiket, [durumEtiket]() {
        durumEtiket->setText("Bilgi butonuna tıklandı!");
        durumEtiket->setStyleSheet(
            "font-size: 13px; padding: 8px; "
            "background-color: #d6eaf8; border-radius: 5px; color: #2980b9;"
        );
    });

    QObject::connect(uyariButon, &QPushButton::clicked,
                     durumEtiket, [durumEtiket]() {
        durumEtiket->setText("Uyarı butonuna tıklandı!");
        durumEtiket->setStyleSheet(
            "font-size: 13px; padding: 8px; "
            "background-color: #fdebd0; border-radius: 5px; color: #e67e22;"
        );
    });

    QObject::connect(yuvarlakButon, &QPushButton::clicked,
                     durumEtiket, [durumEtiket]() {
        durumEtiket->setText("Yuvarlak stil butonuna tıklandı!");
    });

    QObject::connect(gradyanButon, &QPushButton::clicked,
                     durumEtiket, [durumEtiket]() {
        durumEtiket->setText("Gradyan stil butonuna tıklandı!");
    });

    QObject::connect(cerceveButon, &QPushButton::clicked,
                     durumEtiket, [durumEtiket]() {
        durumEtiket->setText("Çerçeveli stil butonuna tıklandı!");
    });

    // =============================================
    // 9) KAPAT BUTONU
    // =============================================
    anaDuzen->addStretch();

    QPushButton* kapatButon = new QPushButton("Kapat");
    kapatButon->setStyleSheet(
        "font-size: 13px; padding: 8px; "
        "background-color: #95a5a6; color: white; border-radius: 5px;"
    );
    QObject::connect(kapatButon, &QPushButton::clicked,
                     &uygulama, &QApplication::quit);
    anaDuzen->addWidget(kapatButon);

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "QPushButton ve QLabel Örnekleri"
 * Boyut: En az 500x550 piksel
 *
 * İçerik (yukarıdan aşağıya):
 *   - Başlık etiketi: "QPushButton ve QLabel Örnekleri" (büyük, kalın)
 *
 *   - HTML destekli etiket (çerçeveli panel):
 *       "OBYS Bilgi Paneli" (başlık)
 *       Öğrenci sayısı: 142 (yeşil, kalın)
 *       Aktif dersler: 18 (mavi, kalın)
 *       Ortalama başarı: %78.5 (turuncu, italik)
 *
 *   - QPixmap görseli: 120x60 mavi dikdörtgen (kesikli çerçeve içinde)
 *     Üzerine gelindiğinde ipucu metni görünür
 *
 *   - İkonlu butonlar (yan yana):
 *       [Kaydet] (yeşil, kaydet ikonu)
 *       [Bilgi] (mavi, bilgi ikonu)
 *       [Uyarı] (turuncu, uyarı ikonu)
 *
 *   - "Bildirim Ayarları:" başlığı
 *   - Toggle buton: "Bildirimler: KAPALI" (gri, tıklanınca yeşil "AÇIK")
 *   - Bildirim durumu metni
 *
 *   - "QSS Örnekleri:" başlığı
 *   - Stil butonları (yan yana):
 *       [Yuvarlak] (kırmızı, yuvarlak kenarlar)
 *       [Gradyan] (mor gradyan)
 *       [Çerçeveli] (yeşil çerçeve, şeffaf arka plan)
 *       Hover ve pressed efektleri mevcut (QSS pseudo-selector)
 *
 *   - Durum etiketi: butona tıklandığında ilgili mesaj gösterilir
 *   - [Kapat] butonu (gri)
 *
 * NOT: Bu bir GUI uygulamasıdır, konsol çıktısı üretmez.
 */

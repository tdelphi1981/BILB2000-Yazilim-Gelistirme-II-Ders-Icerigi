/**
 * Giriş Widget'ları - Veri Alma Araçları
 *
 * Bu program, kullanıcıdan veri almak için kullanılan temel Qt giriş
 * widget'larını kapsamlı olarak gösterir: QLineEdit (metin, şifre,
 * salt okunur modları), QComboBox (açılır liste), QSpinBox (tamsayı),
 * QDoubleSpinBox (ondalıklı sayı), QCheckBox (çoklu seçim), QRadioButton
 * (tekli seçim) ve QButtonGroup (buton gruplama). Her widget'in değişim
 * sinyalleri ortak bir durum etiketine bağlanmıştır.
 *
 * Öğrenilecek kavramlar:
 * - QLineEdit: metin girişi, setEchoMode() ile şifre modu, setReadOnly()
 * - QComboBox: açılır liste seçimi, currentTextChanged sinyali
 * - QSpinBox: tamsayı giriş, setRange(), setPrefix(), setSuffix()
 * - QDoubleSpinBox: ondalıklı sayı girişi, setDecimals()
 * - QCheckBox: çoklu seçim, checkStateChanged sinyali
 * - QRadioButton: tekli seçim, toggled sinyali
 * - QButtonGroup: radyo butonlarını mantıksal gruplama, idToggled sinyali
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 4 - Temel Widget'lar
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 02_giris_widgetlari 02_giris_widgetlari.cpp
 *
 * Çalıştırma: ./02_giris_widgetlari
 */

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QWidget pencere;
    pencere.setWindowTitle("Giriş Widget'ları");
    pencere.setMinimumSize(500, 620);

    QVBoxLayout* anaDuzen = new QVBoxLayout(&pencere);
    anaDuzen->setSpacing(8);
    anaDuzen->setContentsMargins(15, 15, 15, 15);

    // Durum etiketi (en üstte, tüm widget'lardan bilgi alır)
    QLabel* durumEtiket = new QLabel("Widget'ları kullanarak bilgi girin.");
    durumEtiket->setAlignment(Qt::AlignCenter);
    durumEtiket->setWordWrap(true);
    durumEtiket->setStyleSheet(
        "font-size: 14px; padding: 10px; "
        "background-color: #eaf2f8; border: 1px solid #aed6f1; "
        "border-radius: 5px;"
    );
    anaDuzen->addWidget(durumEtiket);

    // =============================================
    // 1) QLINEEDIT - METİN GİRİŞİ (3 mod)
    // =============================================
    QGroupBox* metinGrup = new QGroupBox("QLineEdit - Metin Girişi");
    QFormLayout* metinDuzen = new QFormLayout(metinGrup);

    // a) Normal metin girişi
    QLineEdit* isimGiris = new QLineEdit();
    isimGiris->setPlaceholderText("Örneğin: Ayşe Yılmaz");
    isimGiris->setMaxLength(50);
    isimGiris->setStyleSheet(
        "padding: 5px; border: 2px solid #bdc3c7; border-radius: 4px;"
    );

    QObject::connect(isimGiris, &QLineEdit::textChanged,
                     durumEtiket, [durumEtiket](const QString& metin) {
        durumEtiket->setText(QString("Girilen isim: \"%1\"").arg(metin));
    });

    metinDuzen->addRow("Ad Soyad:", isimGiris);

    // b) Şifre modu (EchoMode::Password)
    // setEchoMode() ile girilen karakterler gizlenir.
    // Password: yıldız/nokta gösterir, PasswordEchoOnEdit: yazarken gösterir
    QLineEdit* sifreGiris = new QLineEdit();
    sifreGiris->setEchoMode(QLineEdit::Password);
    sifreGiris->setPlaceholderText("Şifrenizi girin...");
    sifreGiris->setStyleSheet(
        "padding: 5px; border: 2px solid #bdc3c7; border-radius: 4px;"
    );

    QObject::connect(sifreGiris, &QLineEdit::textChanged,
                     durumEtiket, [durumEtiket](const QString& metin) {
        // Gerçek şifre gösterilmez, sadece uzunluk bilgisi
        durumEtiket->setText(
            QString("Şifre uzunluğu: %1 karakter").arg(metin.length())
        );
    });

    metinDuzen->addRow("Şifre:", sifreGiris);

    // c) Salt okunur mod (setReadOnly)
    // setReadOnly(true) ile kullanıcı içerik değiştiremez
    // ama metin seçilebilir ve kopyalanabilir.
    QLineEdit* saltOkunurGiris = new QLineEdit("Bu alan değiştirilemez.");
    saltOkunurGiris->setReadOnly(true);
    saltOkunurGiris->setStyleSheet(
        "padding: 5px; border: 2px solid #bdc3c7; border-radius: 4px; "
        "background-color: #f0f0f0; color: #7f8c8d;"
    );

    metinDuzen->addRow("Salt okunur:", saltOkunurGiris);

    anaDuzen->addWidget(metinGrup);

    // =============================================
    // 2) QCOMBOBOX - AÇILIR LİSTE
    // =============================================
    QGroupBox* comboGrup = new QGroupBox("QComboBox - Açılır Liste");
    QFormLayout* comboDuzen = new QFormLayout(comboGrup);

    QComboBox* bolumSecim = new QComboBox();
    bolumSecim->addItem("Bilgisayar Bilimleri");
    bolumSecim->addItem("Yazılım Mühendisliği");
    bolumSecim->addItem("Matematik");
    bolumSecim->addItem("Fizik");
    bolumSecim->addItem("Kimya");
    bolumSecim->setStyleSheet("padding: 5px;");

    // currentTextChanged sinyali seçim değiştiğinde tetiklenir
    QObject::connect(bolumSecim, &QComboBox::currentTextChanged,
                     durumEtiket, [durumEtiket](const QString& secilen) {
        durumEtiket->setText(QString("Seçilen bölüm: %1").arg(secilen));
    });

    comboDuzen->addRow("Bölüm:", bolumSecim);
    anaDuzen->addWidget(comboGrup);

    // =============================================
    // 3) QSPINBOX VE QDOUBLESPINBOX - SAYISAL DEĞER
    // =============================================
    QGroupBox* sayiGrup = new QGroupBox("QSpinBox / QDoubleSpinBox - Sayısal Değer");
    QFormLayout* sayiDuzen = new QFormLayout(sayiGrup);

    // a) QSpinBox: Tamsayı giriş
    QSpinBox* donemSecim = new QSpinBox();
    donemSecim->setRange(1, 8);           // minimum ve maksimum değer
    donemSecim->setValue(1);              // başlangıç değeri
    donemSecim->setPrefix("Dönem: ");    // değerden önce gösterilen metin
    donemSecim->setSuffix(". yarıyıl");  // değerden sonra gösterilen metin
    donemSecim->setStyleSheet("padding: 5px;");

    QObject::connect(donemSecim, &QSpinBox::valueChanged,
                     durumEtiket, [durumEtiket](int deger) {
        durumEtiket->setText(QString("Seçilen dönem: %1. yarıyıl").arg(deger));
    });

    sayiDuzen->addRow("Dönem:", donemSecim);

    // b) QDoubleSpinBox: Ondalıklı sayı girişi
    // Örneğin not ortalaması 0.00 - 4.00 arası
    QDoubleSpinBox* ortalamaGiris = new QDoubleSpinBox();
    ortalamaGiris->setRange(0.00, 4.00);    // GNO aralığı
    ortalamaGiris->setValue(2.50);           // başlangıç değeri
    ortalamaGiris->setDecimals(2);           // ondalık basamak sayısı
    ortalamaGiris->setSingleStep(0.05);      // ok tuşlarıyla artış miktarı
    ortalamaGiris->setPrefix("GNO: ");
    ortalamaGiris->setStyleSheet("padding: 5px;");

    QObject::connect(ortalamaGiris, &QDoubleSpinBox::valueChanged,
                     durumEtiket, [durumEtiket](double deger) {
        QString durum;
        if (deger >= 3.50) durum = "Yüksek Onur";
        else if (deger >= 3.00) durum = "Onur";
        else if (deger >= 2.00) durum = "Başarılı";
        else durum = "Şartlı";

        durumEtiket->setText(
            QString("GNO: %1 -> %2").arg(deger, 0, 'f', 2).arg(durum)
        );
    });

    sayiDuzen->addRow("Not Ort.:", ortalamaGiris);
    anaDuzen->addWidget(sayiGrup);

    // =============================================
    // 4) QCHECKBOX - ÇOKLU SEÇİM
    // =============================================
    // QCheckBox bağımsız olarak işaretlenebilir (birden fazla seçim)
    QGroupBox* checkGrup = new QGroupBox("QCheckBox - Çoklu Seçim");
    QVBoxLayout* checkDuzen = new QVBoxLayout(checkGrup);

    QCheckBox* bursluKontrol = new QCheckBox("Burslu öğrenci");
    QCheckBox* yatiliKontrol = new QCheckBox("Yatılı barınma");
    QCheckBox* stajKontrol = new QCheckBox("Staj yapacak");

    // Ortak lambda: herhangi bir checkbox değiştiğinde tüm seçimler raporlanır
    auto checkDegisti = [durumEtiket, bursluKontrol, yatiliKontrol, stajKontrol]() {
        QStringList secimler;
        if (bursluKontrol->isChecked()) secimler << "Burslu";
        if (yatiliKontrol->isChecked()) secimler << "Yatılı";
        if (stajKontrol->isChecked()) secimler << "Stajyer";

        if (secimler.isEmpty()) {
            durumEtiket->setText("Ek seçenek işaretlenmedi.");
        } else {
            durumEtiket->setText("Seçimler: " + secimler.join(", "));
        }
    };

    QObject::connect(bursluKontrol, &QCheckBox::checkStateChanged,
                     durumEtiket, checkDegisti);
    QObject::connect(yatiliKontrol, &QCheckBox::checkStateChanged,
                     durumEtiket, checkDegisti);
    QObject::connect(stajKontrol, &QCheckBox::checkStateChanged,
                     durumEtiket, checkDegisti);

    checkDuzen->addWidget(bursluKontrol);
    checkDuzen->addWidget(yatiliKontrol);
    checkDuzen->addWidget(stajKontrol);
    anaDuzen->addWidget(checkGrup);

    // =============================================
    // 5) QRADIOBUTTON + QBUTTONGROUP - TEKLİ SEÇİM
    // =============================================
    // QRadioButton'lar aynı parent içinde otomatik olarak tekli seçim
    // sağlar. QButtonGroup ile mantıksal gruplama ve ID atama yapılır.
    // Bu, seçimi programatik olarak sorgulamamızı kolaylaştırır.
    QGroupBox* radioGrup = new QGroupBox("QRadioButton + QButtonGroup - Tekli Seçim");
    QHBoxLayout* radioDuzen = new QHBoxLayout(radioGrup);

    QRadioButton* lisansRadio = new QRadioButton("Lisans");
    QRadioButton* yuksekLisansRadio = new QRadioButton("Yüksek Lisans");
    QRadioButton* doktoraRadio = new QRadioButton("Doktora");
    lisansRadio->setChecked(true);  // Varsayılan seçim

    // QButtonGroup ile gruplama ve ID atama
    // Bu sayede hangi butonun seçili olduğunu ID ile sorgulayabiliriz.
    QButtonGroup* ogrenimGrup = new QButtonGroup(&pencere);
    ogrenimGrup->addButton(lisansRadio, 1);       // ID = 1
    ogrenimGrup->addButton(yuksekLisansRadio, 2);  // ID = 2
    ogrenimGrup->addButton(doktoraRadio, 3);       // ID = 3

    // idToggled sinyali: seçilen butonun ID'si ve durumu ile tetiklenir.
    // Bu, birden fazla radyo butonunu ayrı ayrı bağlamaktan daha pratiktir.
    QObject::connect(ogrenimGrup, &QButtonGroup::idToggled,
                     durumEtiket, [durumEtiket](int id, bool isaretli) {
        if (!isaretli) return;  // Sadece yeni seçilen butonla ilgilen

        QString seviye;
        switch (id) {
            case 1: seviye = "Lisans (4 yıl)"; break;
            case 2: seviye = "Yüksek Lisans (2 yıl)"; break;
            case 3: seviye = "Doktora (4+ yıl)"; break;
        }
        durumEtiket->setText(QString("Öğrenim türü [ID=%1]: %2").arg(id).arg(seviye));
    });

    radioDuzen->addWidget(lisansRadio);
    radioDuzen->addWidget(yuksekLisansRadio);
    radioDuzen->addWidget(doktoraRadio);
    anaDuzen->addWidget(radioGrup);

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "Giriş Widget'ları"
 * Boyut: En az 500x620 piksel
 *
 * İçerik (yukarıdan aşağıya):
 *   - Durum etiketi (mavi çerçeveli): "Widget'ları kullanarak bilgi girin."
 *     Her widget değişiminde bu etiket güncellenir.
 *
 *   - "QLineEdit - Metin Girişi" grubu:
 *     Ad Soyad: [............] (placeholder: "Örneğin: Ayşe Yılmaz")
 *     Şifre:    [****]        (karakterler gizli, Password modu)
 *     Salt okunur: [Bu alan değiştirilemez.] (gri arka plan, düzenlenemez)
 *
 *   - "QComboBox - Açılır Liste" grubu:
 *     Bölüm: [Bilgisayar Bilimleri v] (5 seçenek)
 *
 *   - "QSpinBox / QDoubleSpinBox - Sayısal Değer" grubu:
 *     Dönem: [Dönem: 1. yarıyıl] (1-8 arası, ok tuşları ile)
 *     Not Ort.: [GNO: 2.50] (0.00-4.00 arası, 0.05 adım, 2 ondalık)
 *
 *   - "QCheckBox - Çoklu Seçim" grubu:
 *     [ ] Burslu öğrenci
 *     [ ] Yatılı barınma
 *     [ ] Staj yapacak
 *     (birden fazla seçilebilir)
 *
 *   - "QRadioButton + QButtonGroup - Tekli Seçim" grubu:
 *     (o) Lisans  ( ) Yüksek Lisans  ( ) Doktora
 *     (sadece biri seçilebilir, QButtonGroup ile ID bazlı sorgulama)
 *
 * Davranış:
 *   - Her widget değişiminde durum etiketi güncellenir.
 *   - Şifre alanında karakter gizlenir, sadece uzunluk raporlanır.
 *   - Salt okunur alan metin seçimine izin verir ama düzenleme yapamaz.
 *   - GNO değerine göre "Yüksek Onur/Onur/Başarılı/Şartlı" gösterilir.
 *   - QButtonGroup ile radyo butonları ID bazlı rapor verir.
 */

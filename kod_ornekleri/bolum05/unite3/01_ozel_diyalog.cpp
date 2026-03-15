/**
 * Özel Diyalog Penceresi - OBYS Öğrenci Düzenleme Diyalogu
 *
 * Bu program, QDialog sınıfından türetilen özel bir diyalog penceresi
 * oluşturur. Öğrenci bilgilerini düzenlemek için ad, numara ve bölüm
 * alanları içeren form, QDialogButtonBox ile OK/Cancel butonları ve
 * accept()/reject() mekanizması gösterilmektedir.
 *
 * Öğrenilecek kavramlar:
 * - QDialog sınıfından türetme
 * - Q_OBJECT makrosu ve moc sistemi
 * - QDialogButtonBox ile standart butonlar
 * - accept() ve reject() slotları
 * - exec() ile modal çalıştırma ve dönüş değeri
 * - Diyalogdan veri alma (getter metodları)
 *
 * Bölüm: 05 - Qt Pencere ve Düzen Yönetimi
 * Ünite: 3 - Diyalog Pencereleri
 *
 * Derleme (CMake önerilir, CMakeLists.txt dosyasına bakınız):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 *
 * Doğrudan derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 01_ozel_diyalog 01_ozel_diyalog.cpp
 */

#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QToolBar>
#include <QMessageBox>
#include <QDebug>

// =============================================
// ÖZEL DİYALOG SINIFI
// =============================================
class OgrenciDuzenlemeDiyalogu : public QDialog {
    // Q_OBJECT makrosu olmadan da basit lambda slot kullanılabilir
    // Ancak gerçek projelerde Q_OBJECT gerekir (sinyal/slot tanımı için)

public:
    explicit OgrenciDuzenlemeDiyalogu(QWidget* parent = nullptr)
        : QDialog(parent) {

        setWindowTitle("Öğrenci Bilgisi Düzenle");
        setMinimumWidth(400);

        QVBoxLayout* anaDuzen = new QVBoxLayout(this);
        anaDuzen->setSpacing(12);

        // -- Başlık --
        QLabel* baslik = new QLabel("Öğrenci Bilgilerini Düzenle");
        baslik->setStyleSheet(
            "font-size: 16px; font-weight: bold; color: #2c3e50; "
            "padding: 8px; background-color: #eaf2f8; border-radius: 4px;"
        );
        baslik->setAlignment(Qt::AlignCenter);
        anaDuzen->addWidget(baslik);

        // -- Form Alanları --
        QFormLayout* formDuzen = new QFormLayout();
        formDuzen->setSpacing(10);

        adGiris_ = new QLineEdit();
        adGiris_->setPlaceholderText("Öğrenci adı ve soyadı");
        formDuzen->addRow("Ad Soyad:", adGiris_);

        numaraGiris_ = new QSpinBox();
        numaraGiris_->setRange(1000, 9999);
        numaraGiris_->setValue(1000);
        formDuzen->addRow("Öğrenci No:", numaraGiris_);

        bolumSecim_ = new QComboBox();
        bolumSecim_->addItems({
            "-- Seçiniz --",
            "Bilgisayar Bilimleri",
            "Yazılım Mühendisliği",
            "Elektrik-Elektronik Müh.",
            "Matematik",
            "Fizik"
        });
        formDuzen->addRow("Bölüm:", bolumSecim_);

        anaDuzen->addLayout(formDuzen);

        // -- Buton Kutusu --
        QDialogButtonBox* butonKutusu = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            this
        );

        // Buton metinlerini Türkçeleştir
        butonKutusu->button(QDialogButtonBox::Ok)->setText("Kaydet");
        butonKutusu->button(QDialogButtonBox::Cancel)->setText("İptal");

        anaDuzen->addWidget(butonKutusu);

        // -- Buton bağlantıları --
        // OK -> doğrulama yap, başarılıysa accept()
        connect(butonKutusu, &QDialogButtonBox::accepted, this, [this]() {
            // Doğrulama
            if (adGiris_->text().trimmed().isEmpty()) {
                QMessageBox::warning(this, "Uyarı",
                                     "Öğrenci adı boş bırakılamaz!");
                adGiris_->setFocus();
                return;
            }
            if (bolumSecim_->currentIndex() == 0) {
                QMessageBox::warning(this, "Uyarı",
                                     "Lütfen bir bölüm seçiniz!");
                bolumSecim_->setFocus();
                return;
            }
            // Doğrulama başarılı - diyalogu kapat (QDialog::Accepted döner)
            accept();
        });

        // Cancel -> reject()
        connect(butonKutusu, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    // -- Getter metodları: Diyalog kapatıldıktan sonra verilere erişim --
    QString ogrenciAdi() const { return adGiris_->text().trimmed(); }
    int ogrenciNumarasi() const { return numaraGiris_->value(); }
    QString bolum() const { return bolumSecim_->currentText(); }

    // -- Setter metodları: Düzenleme için mevcut veriyi yükle --
    void setOgrenciAdi(const QString& ad) { adGiris_->setText(ad); }
    void setOgrenciNumarasi(int no) { numaraGiris_->setValue(no); }
    void setBolum(const QString& bolum) {
        int idx = bolumSecim_->findText(bolum);
        if (idx >= 0) bolumSecim_->setCurrentIndex(idx);
    }

private:
    QLineEdit* adGiris_;
    QSpinBox* numaraGiris_;
    QComboBox* bolumSecim_;
};

// =============================================
// ANA PROGRAM
// =============================================
int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QMainWindow anaPencere;
    anaPencere.setWindowTitle("OBYS - Özel Diyalog Örneği");
    anaPencere.resize(600, 400);

    // Log alanı
    QTextEdit* log = new QTextEdit();
    log->setReadOnly(true);
    log->append("[OK] Uygulama başlatıldı\n");
    anaPencere.setCentralWidget(log);

    // Araç çubuğuna buton ekle
    QToolBar* aracCubugu = anaPencere.addToolBar("Araçlar");

    QPushButton* yeniBtn = new QPushButton("Yeni Öğrenci Ekle");
    aracCubugu->addWidget(yeniBtn);

    QPushButton* duzenleBtn = new QPushButton("Öğrenci Düzenle");
    aracCubugu->addWidget(duzenleBtn);

    // -- Yeni öğrenci ekleme --
    QObject::connect(yeniBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        OgrenciDuzenlemeDiyalogu diyalog(&anaPencere);

        // exec() modal olarak çalıştırır ve sonucu döner
        if (diyalog.exec() == QDialog::Accepted) {
            log->append(QString("[OK] Yeni öğrenci eklendi: %1 (No: %2) - %3")
                        .arg(diyalog.ogrenciAdi())
                        .arg(diyalog.ogrenciNumarasi())
                        .arg(diyalog.bolum()));
            qDebug() << "[OK] Öğrenci eklendi:" << diyalog.ogrenciAdi();
        } else {
            log->append("[UYARI] Ekleme iptal edildi");
            qDebug() << "[UYARI] Ekleme iptal edildi";
        }
    });

    // -- Mevcut öğrenci düzenleme --
    QObject::connect(duzenleBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        OgrenciDuzenlemeDiyalogu diyalog(&anaPencere);

        // Mevcut verileri yükle
        diyalog.setOgrenciAdi("Ayşe Yılmaz");
        diyalog.setOgrenciNumarasi(2045);
        diyalog.setBolum("Bilgisayar Bilimleri");

        if (diyalog.exec() == QDialog::Accepted) {
            log->append(QString("[OK] Öğrenci güncellendi: %1 (No: %2) - %3")
                        .arg(diyalog.ogrenciAdi())
                        .arg(diyalog.ogrenciNumarasi())
                        .arg(diyalog.bolum()));
            qDebug() << "[OK] Öğrenci güncellendi:" << diyalog.ogrenciAdi();
        } else {
            log->append("[UYARI] Düzenleme iptal edildi");
        }
    });

    anaPencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi - Ana]
 * Başlık: "OBYS - Özel Diyalog Örneği"
 * Boyut: 600x400 piksel
 * Araç çubuğu: [Yeni Öğrenci Ekle] [Öğrenci Düzenle]
 * Merkez: Log alanı (salt okunur)
 *
 * [GUI Penceresi - Diyalog] ("Yeni Öğrenci Ekle" tıklandığında)
 * Başlık: "Öğrenci Bilgisi Düzenle"
 * İçerik:
 *   "Öğrenci Bilgilerini Düzenle" başlığı
 *   Ad Soyad:     [__________________]
 *   Öğrenci No:   [1000 ^v]
 *   Bölüm:        [-- Seçiniz --]
 *   [Kaydet] [İptal]
 *
 * Davranış:
 *   - "Yeni Öğrenci Ekle": Boş formla diyalog açılır
 *   - "Öğrenci Düzenle": Mevcut verilerle (Ayşe Yılmaz) diyalog açılır
 *   - Boş isim veya bölüm seçilmemişse uyarı mesajı
 *   - "Kaydet" -> log'a ekleme mesajı yazılır
 *   - "İptal" -> log'a iptal mesajı yazılır
 *   - Diyalog modal: ana pencere ile etkileşim engellenir
 *
 * Log çıktısı (başarılı ekleme):
 *   [OK] Yeni öğrenci eklendi: Ali Demir (No: 2050) - Yazılım Mühendisliği
 */

/**
 * Modal ve Modeless Diyaloglar - exec() vs show() Karşılaştırması
 *
 * Bu program, modal ve modeless (kalıcı olmayan) diyalog pencerelerinin
 * farkını gösterir. Modal diyalog (exec()) ana pencereyi engeller,
 * modeless diyalog (show()) ise ana pencere ile eş zamanlı çalışır.
 * Modeless diyaloglarda parent sahipliği ve bellek yönetimi de
 * gösterilmektedir.
 *
 * Öğrenilecek kavramlar:
 * - exec() ile modal diyalog (engelleyici)
 * - show() ile modeless diyalog (engellemez)
 * - Parent sahipliğinin önemi (bellek yönetimi)
 * - setAttribute(Qt::WA_DeleteOnClose)
 * - Modal ve modeless arasındaki davranış farkı
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
 *       -o 03_modal_modeless 03_modal_modeless.cpp
 */

#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QDebug>

// =============================================
// YARDIMCI: Basit bir diyalog oluştur
// =============================================
QDialog* diyalogOlustur(QWidget* parent, const QString& baslik,
                        const QString& aciklama) {
    QDialog* diyalog = new QDialog(parent);
    diyalog->setWindowTitle(baslik);
    diyalog->setMinimumSize(350, 200);

    QVBoxLayout* duzen = new QVBoxLayout(diyalog);

    QLabel* etiket = new QLabel(aciklama);
    etiket->setWordWrap(true);
    etiket->setStyleSheet(
        "font-size: 13px; padding: 10px; "
        "background-color: #eaf2f8; border-radius: 4px;"
    );
    duzen->addWidget(etiket);

    QLineEdit* giris = new QLineEdit();
    giris->setPlaceholderText("Bir şeyler yazın...");
    duzen->addWidget(giris);

    QLabel* notEtiket = new QLabel(
        "[NOT] Ana pencere ile etkileşimi deneyin!"
    );
    notEtiket->setStyleSheet("color: #7f8c8d; font-style: italic;");
    duzen->addWidget(notEtiket);

    QDialogButtonBox* butonlar = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    );
    QObject::connect(butonlar, &QDialogButtonBox::accepted,
                     diyalog, &QDialog::accept);
    QObject::connect(butonlar, &QDialogButtonBox::rejected,
                     diyalog, &QDialog::reject);
    duzen->addWidget(butonlar);

    return diyalog;
}

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) ANA PENCERE
    // =============================================
    QMainWindow anaPencere;
    anaPencere.setWindowTitle("OBYS - Modal vs Modeless Diyalog");
    anaPencere.resize(600, 450);

    QWidget* merkeziWidget = new QWidget();
    QVBoxLayout* anaDuzen = new QVBoxLayout(merkeziWidget);
    anaDuzen->setSpacing(15);
    anaDuzen->setContentsMargins(20, 20, 20, 20);

    // Açıklama
    QLabel* aciklama = new QLabel(
        "Modal diyalog (exec()) ana pencereyi engeller.\n"
        "Modeless diyalog (show()) ana pencere ile birlikte çalışır.\n"
        "Her iki butonu da deneyerek farkı gözlemleyin."
    );
    aciklama->setStyleSheet(
        "padding: 12px; background-color: #fdebd0; "
        "border-radius: 6px; font-size: 13px;"
    );
    aciklama->setWordWrap(true);
    anaDuzen->addWidget(aciklama);

    // =============================================
    // 2) BUTONLAR
    // =============================================
    QHBoxLayout* butonDuzen = new QHBoxLayout();

    QPushButton* modalBtn = new QPushButton("Modal Diyalog (exec)");
    modalBtn->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; "
        "padding: 12px 20px; font-size: 14px; border-radius: 6px; }"
        "QPushButton:hover { background-color: #c0392b; }"
    );

    QPushButton* modelessBtn = new QPushButton("Modeless Diyalog (show)");
    modelessBtn->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; "
        "padding: 12px 20px; font-size: 14px; border-radius: 6px; }"
        "QPushButton:hover { background-color: #219a52; }"
    );

    butonDuzen->addWidget(modalBtn);
    butonDuzen->addWidget(modelessBtn);
    anaDuzen->addLayout(butonDuzen);

    // =============================================
    // 3) LOG ALANI
    // =============================================
    QTextEdit* log = new QTextEdit();
    log->setReadOnly(true);
    log->setStyleSheet(
        "font-family: 'Consolas', monospace; font-size: 12px; "
        "background-color: #2c3e50; color: #ecf0f1; padding: 8px;"
    );
    log->append("[OK] Uygulama başlatıldı");
    log->append("Ana pencereye tıklayarak etkileşimi test edin\n");
    anaDuzen->addWidget(log);

    anaPencere.setCentralWidget(merkeziWidget);

    // =============================================
    // 4) SİNYAL/SLOT BAĞLANTILARI
    // =============================================

    // -- Modal diyalog: exec() ile --
    QObject::connect(modalBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        log->append("[UYARI] Modal diyalog açılıyor - ana pencere kilitlenecek!");

        QDialog* diyalog = diyalogOlustur(
            &anaPencere,
            "Modal Diyalog",
            "Bu modal bir diyalogdur.\n\n"
            "exec() ile açıldığı için ana pencere ile "
            "etkileşim ENGELLENMIŞTIR. Bu diyalogu "
            "kapatmadan ana pencereyi kullanamazsınız."
        );

        // exec() -> engelleyici (blocking) çağrı
        // Diyalog kapanana kadar bu satırın altı çalışmaz
        int sonuc = diyalog->exec();

        if (sonuc == QDialog::Accepted) {
            log->append("[OK] Modal diyalog: OK ile kapatıldı");
        } else {
            log->append("[UYARI] Modal diyalog: İptal ile kapatıldı");
        }

        // exec() ile açılan diyalog stack'te oluşturulabilir
        // veya manuel delete edilmelidir
        delete diyalog;
    });

    // -- Modeless diyalog: show() ile --
    QObject::connect(modelessBtn, &QPushButton::clicked, &anaPencere,
                     [log, &anaPencere]() {
        log->append("[OK] Modeless diyalog açılıyor - ana pencere aktif kalacak!");

        QDialog* diyalog = diyalogOlustur(
            &anaPencere,  // parent belirle -> pencere kapatılınca diyalog da kapanır
            "Modeless Diyalog",
            "Bu modeless bir diyalogdur.\n\n"
            "show() ile açıldığı için ana pencere ile "
            "etkileşim DEVAM EDER. Hem bu diyalogu hem de "
            "ana pencereyi aynı anda kullanabilirsiniz."
        );

        // Kapatılınca otomatik silinsin (bellek sızıntısını önle)
        diyalog->setAttribute(Qt::WA_DeleteOnClose);

        // Kapanma sinyalini yakala
        QObject::connect(diyalog, &QDialog::finished, log,
                         [log](int sonuc) {
            if (sonuc == QDialog::Accepted) {
                log->append("[OK] Modeless diyalog: OK ile kapatıldı");
            } else {
                log->append("[UYARI] Modeless diyalog: İptal ile kapatıldı");
            }
        });

        // show() -> engellemez (non-blocking)
        // Kod hemen devam eder, diyalog arka planda açık kalır
        diyalog->show();

        log->append("[OK] show() çağrısı tamamlandı - kod devam ediyor!");
    });

    anaPencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi - Ana]
 * Başlık: "OBYS - Modal vs Modeless Diyalog"
 * Boyut: 600x450 piksel
 *
 * İçerik:
 *   - Açıklama metni (turuncu arka plan)
 *   - [Modal Diyalog (exec)] (kırmızı)  [Modeless Diyalog (show)] (yeşil)
 *   - Log alanı (koyu arka plan)
 *
 * Modal Diyalog Davranışı:
 *   - exec() çağrıldığında ana pencere kilitlenir
 *   - Diyalog kapatılmadan ana pencere kullanılamaz
 *   - OK/Cancel sonucu exec() dönüş değerinden alınır
 *   - Log mesajı diyalog kapatıldıktan sonra eklenir
 *
 * Modeless Diyalog Davranışı:
 *   - show() çağrıldığında ana pencere aktif kalır
 *   - Her iki pencere aynı anda kullanılabilir
 *   - "show() çağrısı tamamlandı" mesajı hemen görünür
 *   - Kapatma sonucu finished() sinyali ile alınır
 *   - WA_DeleteOnClose ile otomatik bellek temizleme
 */

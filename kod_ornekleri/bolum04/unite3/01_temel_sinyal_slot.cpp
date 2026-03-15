/**
 * Temel Sinyal/Slot Mekanizması - QPushButton ve QLineEdit
 *
 * Bu program, Qt'nin sinyal/slot mekanizmasının temel kullanımını
 * gösterir. QPushButton::clicked sinyali ile bir mesaj güncellenir,
 * QLineEdit::textChanged sinyali ile yazılan metin anlık olarak
 * bir QLabel'a yansıtılır. Böylece sinyal/slot bağlantısının
 * en temel iki kullanımını tek örnekte görebilirsin.
 *
 * Öğrenilecek kavramlar:
 * - QPushButton::clicked sinyali ve lambda bağlantısı
 * - QLineEdit::textChanged sinyali
 * - QLabel::setText slotu ile doğrudan bağlantı
 * - Yeni connect syntax'ı (& operatörü ile, derleme zamanı kontrollü)
 * - QVBoxLayout ile widget yerleştirme
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 3 - Sinyaller ve Slotlar
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 01_temel_sinyal_slot 01_temel_sinyal_slot.cpp
 *
 * Çalıştırma: ./01_temel_sinyal_slot
 */

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) ANA PENCERE VE DÜZEN
    // =============================================
    QWidget pencere;
    pencere.setWindowTitle("Sinyal/Slot Temelleri");
    pencere.setMinimumSize(400, 280);

    QVBoxLayout* duzen = new QVBoxLayout(&pencere);
    duzen->setSpacing(10);
    duzen->setContentsMargins(15, 15, 15, 15);

    // =============================================
    // 2) BUTON TIKLAMA -> LAMBDA İLE MESAJ
    // =============================================
    // Durum etiketi: buton tıklamalarını gösterecek
    QLabel* durumEtiket = new QLabel("Henüz butona basılmadı.");
    durumEtiket->setAlignment(Qt::AlignCenter);
    durumEtiket->setStyleSheet(
        "font-size: 16px; padding: 12px; "
        "background-color: #ecf0f1; border-radius: 5px;"
    );

    // Tıklama sayacını heap'te ayırıyoruz ki lambda içinde kullanabilelim
    int* tiklamaSayaci = new int(0);

    QPushButton* tiklaButon = new QPushButton("Tıkla!");
    tiklaButon->setStyleSheet(
        "background-color: #3498db; color: white; "
        "font-size: 14px; padding: 8px; border-radius: 5px;"
    );

    // Yeni connect syntax'ı:
    //   connect(gönderen, &Sınıf::sinyal, bağlam, lambda)
    // durumEtiket bağlam nesnesi olarak verilir. Eğer durumEtiket
    // silinirse bağlantı otomatik kopar (güvenli bellek yönetimi).
    QObject::connect(tiklaButon, &QPushButton::clicked,
                     durumEtiket, [durumEtiket, tiklamaSayaci]() {
        (*tiklamaSayaci)++;
        durumEtiket->setText(
            QString("Butona %1 kez tıklandı!").arg(*tiklamaSayaci)
        );
        // Arka plan rengini her tıklamada değiştir
        if (*tiklamaSayaci % 2 == 0) {
            durumEtiket->setStyleSheet(
                "font-size: 16px; padding: 12px; "
                "background-color: #d5f5e3; border-radius: 5px;"
            );
        } else {
            durumEtiket->setStyleSheet(
                "font-size: 16px; padding: 12px; "
                "background-color: #d6eaf8; border-radius: 5px;"
            );
        }
    });

    // =============================================
    // 3) QLINEEDIT TEXTCHANGED -> QLABEL SETTEXT
    // =============================================
    // Bir ayırıcı etiket
    QLabel* ayiriciEtiket = new QLabel("Aşağıya bir şey yaz:");
    ayiriciEtiket->setStyleSheet("font-size: 13px; color: #7f8c8d;");

    // Metin giriş alanı
    QLineEdit* girisAlani = new QLineEdit();
    girisAlani->setPlaceholderText("Buraya yaz, aşağıda anlık görünecek...");
    girisAlani->setStyleSheet(
        "font-size: 14px; padding: 8px; border: 2px solid #bdc3c7; "
        "border-radius: 5px;"
    );

    // Anlık önizleme etiketi
    QLabel* onizlemeEtiket = new QLabel("...");
    onizlemeEtiket->setAlignment(Qt::AlignCenter);
    onizlemeEtiket->setStyleSheet(
        "font-size: 18px; font-weight: bold; padding: 12px; "
        "background-color: #fdebd0; border-radius: 5px;"
    );

    // textChanged sinyali her karakter girişinde tetiklenir.
    // Burada sinyal parametresi (const QString&) doğrudan
    // QLabel::setText slotunun parametresiyle eşleşir.
    // Bu nedenle lambda yerine doğrudan slot bağlantısı yapabiliyoruz.
    QObject::connect(girisAlani, &QLineEdit::textChanged,
                     onizlemeEtiket, &QLabel::setText);

    // =============================================
    // 4) KAPAT BUTONU -> QUIT
    // =============================================
    QPushButton* kapatButon = new QPushButton("Kapat");
    kapatButon->setStyleSheet(
        "background-color: #e74c3c; color: white; "
        "font-size: 14px; padding: 8px; border-radius: 5px;"
    );

    // clicked sinyalini doğrudan QApplication::quit slotuna bağla
    QObject::connect(kapatButon, &QPushButton::clicked,
                     &uygulama, &QApplication::quit);

    // =============================================
    // 5) WIDGET'LARI DÜZENE EKLE
    // =============================================
    duzen->addWidget(durumEtiket);
    duzen->addWidget(tiklaButon);
    duzen->addSpacing(10);
    duzen->addWidget(ayiriciEtiket);
    duzen->addWidget(girisAlani);
    duzen->addWidget(onizlemeEtiket);
    duzen->addStretch();
    duzen->addWidget(kapatButon);

    // Bellek temizliği: uygulama kapanırken sayacı sil
    QObject::connect(&uygulama, &QApplication::aboutToQuit, [tiklamaSayaci]() {
        delete tiklamaSayaci;
    });

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "Sinyal/Slot Temelleri"
 * Boyut: En az 400x280 piksel
 *
 * İçerik (yukarıdan aşağıya):
 *   - Gri arka planlı durum etiketi: "Henüz butona basılmadı."
 *   - Mavi "Tıkla!" butonu
 *   - "Aşağıya bir şey yaz:" bilgi etiketi
 *   - Metin giriş alanı (placeholder ile)
 *   - Turuncu arka planlı önizleme etiketi: "..."
 *   - Kırmızı "Kapat" butonu
 *
 * Davranış:
 *   - "Tıkla!" butonuna her tıklamada durum etiketi güncellenir:
 *     "Butona 1 kez tıklandı!", "Butona 2 kez tıklandı!" vb.
 *     Arka plan rengi mavi <-> yeşil arasında değişir.
 *   - Metin giriş alanına yazı yazıldığında, her karakter değişiminde
 *     önizleme etiketi anlık olarak güncellenir (textChanged -> setText).
 *   - "Kapat" butonuna tıklandığında uygulama sonlanır.
 */

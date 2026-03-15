/**
 * Lambda ile Connect - Farklı Yakalama (Capture) Yöntemleri
 *
 * Bu program, Qt sinyal/slot bağlantısında lambda ifadelerinin
 * farklı yakalama modlarını gösterir: değer ile [=], referans
 * ile [&], belirli değişkenler ve context nesnesi kullanımı.
 *
 * Öğrenilecek kavramlar:
 * - Lambda ile sinyal/slot bağlantısı
 * - [=] değer yakalama (value capture)
 * - [&] referans yakalama (reference capture)
 * - [this] ile sınıf üyesi erişimi
 * - Context nesnesi ile yaşam süresi yönetimi
 * - connect() fonksiyonunun lambda aşırı yüklemesi
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 3 - Sinyaller ve Slotlar
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Widgets) \
 *       -o 02_lambda_connect 02_lambda_connect.cpp
 */

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    QWidget pencere;
    pencere.setWindowTitle("Lambda Connect Örnekleri");
    pencere.setMinimumSize(400, 350);

    QVBoxLayout* duzen = new QVBoxLayout(&pencere);
    duzen->setSpacing(10);

    // Ortak durum etiketi
    QLabel* durumEtiket = new QLabel("Lambda connect örneklerini deneyin.");
    durumEtiket->setAlignment(Qt::AlignCenter);
    durumEtiket->setWordWrap(true);
    durumEtiket->setStyleSheet(
        "font-size: 14px; padding: 10px; "
        "background-color: #f8f9fa; border: 1px solid #dee2e6; "
        "border-radius: 5px;"
    );
    duzen->addWidget(durumEtiket);

    // =============================================
    // 1) DEĞER YAKALAMA [=]
    // =============================================
    QString mesaj = "Değer ile yakalandım!";
    int sayac = 42;

    QPushButton* buton1 = new QPushButton("1) [=] Değer Yakalama");
    buton1->setStyleSheet("padding: 8px; font-size: 13px;");

    // [=] : Tüm yerel değişkenlerin KOPYASINI yakalar.
    // mesaj ve sayac'ın o anki değerleri lambda içine kopyalanır.
    QObject::connect(buton1, &QPushButton::clicked,
                     durumEtiket, [=]() {
        // mesaj ve sayac burada lambda oluşturulduğu andaki değerlerdir
        durumEtiket->setText(
            QString("[=] mesaj: %1, sayac: %2").arg(mesaj).arg(sayac)
        );
    });
    duzen->addWidget(buton1);

    // Orijinal değişkenleri değiştir -> lambda bunu GÖRMEZ
    mesaj = "Bu değişiklik lambda'ya yansımaz";
    sayac = 999;

    // =============================================
    // 2) REFERANS YAKALAMA [&] (dikkatli kullan!)
    // =============================================
    // [UYARI] Referans yakalama, değişkenin yaşam süresi boyunca
    // geçerli olmalıdır. Yerel değişkenleri referansla yakalarken
    // çok dikkatli olun!

    // Heap'te oluşturarak yaşam süresi sorununu önlüyoruz
    int* tikSayisi = new int(0);

    QPushButton* buton2 = new QPushButton("2) [&] Referans (heap ile)");
    buton2->setStyleSheet("padding: 8px; font-size: 13px;");

    // tikSayisi işaretçisini değer ile yakalıyoruz,
    // ama gösterdiği değeri referans gibi kullanıyoruz
    QObject::connect(buton2, &QPushButton::clicked,
                     durumEtiket, [durumEtiket, tikSayisi]() {
        (*tikSayisi)++;
        durumEtiket->setText(
            QString("[heap] Tıklama sayısı: %1").arg(*tikSayisi)
        );
    });
    duzen->addWidget(buton2);

    // =============================================
    // 3) BELİRLİ DEĞİŞKENLERİ YAKALAMA
    // =============================================
    QString ogrenciAdi = "Ayşe Yılmaz";
    int ogrenciNo = 1003;

    QPushButton* buton3 = new QPushButton("3) [ogrenciAdi, ogrenciNo] Seçmeli");
    buton3->setStyleSheet("padding: 8px; font-size: 13px;");

    // Sadece belirli değişkenleri yakala (en iyi pratik)
    QObject::connect(buton3, &QPushButton::clicked,
                     durumEtiket, [durumEtiket, ogrenciAdi, ogrenciNo]() {
        durumEtiket->setText(
            QString("Öğrenci: %1 (No: %2)").arg(ogrenciAdi).arg(ogrenciNo)
        );
    });
    duzen->addWidget(buton3);

    // =============================================
    // 4) CONTEXT NESNESİ İLE YAŞAM SÜRESİ
    // =============================================
    // connect(sender, signal, context, lambda)
    // context nesnesi silindiğinde bağlantı otomatik kopar.
    // Bu, bellek sızıntısı ve dangling pointer sorunlarını önler.

    // Bir "geçici panel" olarak context nesnesi oluşturalım
    QObject* contextNesnesi = new QObject(&pencere);
    contextNesnesi->setObjectName("GeciciContext");

    QPushButton* buton4 = new QPushButton("4) Context Nesne ile Bağlantı");
    buton4->setStyleSheet("padding: 8px; font-size: 13px;");

    // contextNesnesi silinirse bu bağlantı otomatik kopar
    QObject::connect(buton4, &QPushButton::clicked,
                     contextNesnesi, [durumEtiket]() {
        durumEtiket->setText("Context nesnesi üzerinden çağrıldı.");
    });
    duzen->addWidget(buton4);

    // Context nesnesini sildiğimizi gösteren buton
    QPushButton* contextSilButon = new QPushButton("Context Nesnesini Sil");
    contextSilButon->setStyleSheet(
        "padding: 8px; font-size: 13px; color: #e74c3c;"
    );

    QObject::connect(contextSilButon, &QPushButton::clicked,
                     durumEtiket, [durumEtiket, contextNesnesi, contextSilButon]() mutable {
        if (contextNesnesi) {
            delete contextNesnesi;
            contextNesnesi = nullptr;
            durumEtiket->setText(
                "Context nesnesi silindi. Buton 4 artık çalışmayacak."
            );
            contextSilButon->setEnabled(false);
        }
    });
    duzen->addWidget(contextSilButon);

    // =============================================
    // 5) PARAMETRELI SINYAL + LAMBDA
    // =============================================
    // QLineEdit gibi widget'lar parametreli sinyal yayar.
    // Burada basit bir örnek olarak butonun toggled sinyalini kullanalım.

    QPushButton* buton5 = new QPushButton("5) Toggled Sinyal (tıkla)");
    buton5->setCheckable(true);  // Toggle buton yap
    buton5->setStyleSheet("padding: 8px; font-size: 13px;");

    // toggled(bool) sinyali, butonun durumunu parametre olarak iletir
    QObject::connect(buton5, &QPushButton::toggled,
                     durumEtiket, [durumEtiket](bool isaretli) {
        if (isaretli) {
            durumEtiket->setText("Buton İŞARETLENDİ (checked = true)");
            durumEtiket->setStyleSheet(
                "font-size: 14px; padding: 10px; "
                "background-color: #d5f5e3; border: 1px solid #27ae60; "
                "border-radius: 5px;"
            );
        } else {
            durumEtiket->setText("Buton İŞARETLENMEDİ (checked = false)");
            durumEtiket->setStyleSheet(
                "font-size: 14px; padding: 10px; "
                "background-color: #fadbd8; border: 1px solid #e74c3c; "
                "border-radius: 5px;"
            );
        }
    });
    duzen->addWidget(buton5);

    // Temizlik
    QObject::connect(&uygulama, &QApplication::aboutToQuit, [tikSayisi]() {
        delete tikSayisi;
    });

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "Lambda Connect Örnekleri"
 * Boyut: En az 400x350 piksel
 *
 * İçerik (dikey sırada):
 *   - Durum etiketi (gri çerçeveli)
 *   - Buton 1: "[=] Değer Yakalama"
 *   - Buton 2: "[&] Referans (heap ile)"
 *   - Buton 3: "[ogrenciAdi, ogrenciNo] Seçmeli"
 *   - Buton 4: "Context Nesne ile Bağlantı"
 *   - "Context Nesnesini Sil" butonu
 *   - Buton 5: "Toggled Sinyal" (toggle buton)
 *
 * Davranış:
 *   - Buton 1: Etiket "[=] mesaj: Değer ile yakalandım!, sayac: 42" gösterir
 *     (değiştirilmiş değerleri değil, yakalama anındaki değerleri)
 *   - Buton 2: Her tıklamada sayaç artar: "Tıklama sayısı: 1", "2", ...
 *   - Buton 3: "Öğrenci: Ayşe Yılmaz (No: 1003)" gösterir
 *   - Buton 4: "Context nesnesi üzerinden çağrıldı." gösterir
 *     (context silindikten sonra tepki vermez)
 *   - "Context Nesnesini Sil" butonu context'i siler ve devre dışı kalır
 *   - Buton 5: Toggle durumuna göre yeşil/kırmızı arka plan değişir
 */

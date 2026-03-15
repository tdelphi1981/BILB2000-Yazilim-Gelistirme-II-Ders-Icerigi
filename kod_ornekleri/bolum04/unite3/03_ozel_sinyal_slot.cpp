/**
 * Özel Sinyal ve Slot Tanımlama - Öğrenci Sınıfı
 *
 * Bu program, Q_OBJECT makrosu ile kendi sinyal ve slotlarımızı
 * nasıl tanımladığımızı gösterir. Ogrenci sınıfı notDegisti(int)
 * ve kayitTamamlandi() sinyallerini, notuGuncelle(int) slotunu
 * içerir. emit anahtar kelimesi ile sinyal yayma işlemi
 * örneklenmiştir.
 *
 * Öğrenilecek kavramlar:
 * - Q_OBJECT makrosu ve Meta-Object System
 * - signals: bloğu ile özel sinyal tanımlama
 * - public slots: bloğu ile özel slot tanımlama
 * - emit ile sinyal yayma
 * - Birden fazla sinyal/slot bağlantısı
 * - Lambda ve sınıf slotlarına bağlantı
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 3 - Sinyaller ve Slotlar
 *
 * Derleme:
 *   moc 03_ozel_sinyal_slot.cpp -o 03_ozel_sinyal_slot.moc
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Core) \
 *       -o 03_ozel_sinyal_slot 03_ozel_sinyal_slot.cpp
 *
 * Çalıştırma: ./03_ozel_sinyal_slot
 */

#include <QCoreApplication>
#include <QObject>
#include <QDebug>

// =============================================
// ÖĞRENCİ SINIFI - ÖZEL SİNYAL VE SLOT ÖRNEĞİ
// =============================================
// Q_OBJECT makrosu, sınıfın sinyal/slot mekanizmasını
// kullanabilmesi için gereklidir. Bu makro olmadan
// signals ve slots blokları çalışmaz.
class Ogrenci : public QObject {
    Q_OBJECT

public:
    explicit Ogrenci(const QString& isim, int numara,
                     QObject* parent = nullptr)
        : QObject(parent)
        , m_isim(isim)
        , m_numara(numara)
        , m_not(0)
        , m_kayitTamam(false)
    {
        setObjectName(isim);
    }

    // Getter fonksiyonları
    QString isim() const { return m_isim; }
    int numara() const { return m_numara; }
    int not_() const { return m_not; }  // 'not' C++ anahtar kelime, _ ekledik
    bool kayitliMi() const { return m_kayitTamam; }

public slots:
    // notuGuncelle: Öğrencinin notunu güncelleyen slot.
    // Değer değiştiyse notDegisti sinyali yayılır.
    void notuGuncelle(int yeniNot) {
        // Geçerlilik kontrolü
        if (yeniNot < 0 || yeniNot > 100) {
            qDebug() << "  [HATA]" << m_isim
                     << "için geçersiz not:" << yeniNot
                     << "(0-100 arası olmalı)";
            return;
        }

        int eskiNot = m_not;
        m_not = yeniNot;

        qDebug() << "  [SLOT] notuGuncelle() ->"
                 << m_isim << ": eski=" << eskiNot
                 << ", yeni=" << yeniNot;

        // Not değiştiyse sinyal yay
        if (eskiNot != yeniNot) {
            emit notDegisti(yeniNot);
        }
    }

    // kaydiTamamla: Öğrencinin kaydını tamamlayan slot.
    // Başarılı olursa kayitTamamlandi sinyali yayılır.
    void kaydiTamamla() {
        if (m_isim.isEmpty()) {
            qDebug() << "  [HATA] Kayıt tamamlanamadı: isim boş!";
            return;
        }

        m_kayitTamam = true;
        qDebug() << "  [SLOT] kaydiTamamla() ->"
                 << m_isim << "(No:" << m_numara << ") kaydı tamamlandı.";

        // Kayıt tamamlandı sinyalini yay
        emit kayitTamamlandi();
    }

signals:
    // notDegisti: Öğrenci notu değiştiğinde yayılır.
    // Parametre olarak yeni notu taşır.
    void notDegisti(int yeniNot);

    // kayitTamamlandi: Öğrenci kaydı başarıyla tamamlandığında yayılır.
    void kayitTamamlandi();

private:
    QString m_isim;
    int m_numara;
    int m_not;
    bool m_kayitTamam;
};

// =============================================
// NOT İZLEYİCİ - ÖĞRENCİ SİNYALLERİNİ DİNLER
// =============================================
class NotIzleyici : public QObject {
    Q_OBJECT

public:
    explicit NotIzleyici(QObject* parent = nullptr)
        : QObject(parent) {
        setObjectName("NotIzleyici");
    }

public slots:
    void notDegisiminiRaporla(int yeniNot) {
        QString durum;
        if (yeniNot >= 90) durum = "Mükemmel (AA)";
        else if (yeniNot >= 80) durum = "Çok İyi (BA)";
        else if (yeniNot >= 70) durum = "İyi (BB)";
        else if (yeniNot >= 60) durum = "Orta (CB)";
        else if (yeniNot >= 50) durum = "Geçti (CC)";
        else durum = "Kaldı (FF)";

        qDebug() << "  [İZLEYİCİ] Not değişikliği algılandı:"
                 << yeniNot << "->" << durum;
    }

    void kayitOnaylandi() {
        qDebug() << "  [İZLEYİCİ] Kayıt onaylandı, sisteme işlendi.";
    }
};

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    qDebug() << "=== Özel Sinyal/Slot Örneği: Öğrenci Sınıfı ===\n";

    // =============================================
    // 1) NESNELERİ OLUŞTUR
    // =============================================
    Ogrenci ogrenci1("Ayşe Yılmaz", 1003);
    Ogrenci ogrenci2("Mehmet Demir", 1007);
    NotIzleyici izleyici;

    // =============================================
    // 2) SİNYAL/SLOT BAĞLANTILARI
    // =============================================
    qDebug() << "--- Bağlantıları kuruyor ---";

    // Ogrenci1'in notDegisti sinyalini izleyiciye bağla
    QObject::connect(&ogrenci1, &Ogrenci::notDegisti,
                     &izleyici, &NotIzleyici::notDegisiminiRaporla);

    // Ogrenci1'in kayitTamamlandi sinyalini izleyiciye bağla
    QObject::connect(&ogrenci1, &Ogrenci::kayitTamamlandi,
                     &izleyici, &NotIzleyici::kayitOnaylandi);

    // Ogrenci2'nin notDegisti sinyalini lambda'ya bağla
    QObject::connect(&ogrenci2, &Ogrenci::notDegisti,
                     [&ogrenci2](int yeniNot) {
        qDebug() << "  [LAMBDA]" << ogrenci2.isim()
                 << "için not değişti:" << yeniNot;
    });

    // Ogrenci2'nin kayitTamamlandi sinyalini lambda'ya bağla
    QObject::connect(&ogrenci2, &Ogrenci::kayitTamamlandi,
                     [&ogrenci2]() {
        qDebug() << "  [LAMBDA]" << ogrenci2.isim()
                 << "kaydı tamamlandı!";
    });

    qDebug() << "Bağlantılar kuruldu.\n";

    // =============================================
    // 3) NOT GÜNCELLEME TESTİ
    // =============================================
    qDebug() << "--- Not Güncelleme Testi ---";

    qDebug() << "\nAyşe'nin notu güncelleniyor (85):";
    ogrenci1.notuGuncelle(85);

    qDebug() << "\nMehmet'in notu güncelleniyor (45):";
    ogrenci2.notuGuncelle(45);

    qDebug() << "\nAyşe'nin notu tekrar güncelleniyor (92):";
    ogrenci1.notuGuncelle(92);

    // Geçersiz not denemesi
    qDebug() << "\nGeçersiz not denemesi (150):";
    ogrenci1.notuGuncelle(150);

    // Aynı not tekrar verildiğinde sinyal yayılmamalı
    qDebug() << "\nAynı not tekrar (92, sinyal yayılmamalı):";
    ogrenci1.notuGuncelle(92);

    // =============================================
    // 4) KAYIT TAMAMLAMA TESTİ
    // =============================================
    qDebug() << "\n--- Kayıt Tamamlama Testi ---";

    qDebug() << "\nAyşe'nin kaydı tamamlanıyor:";
    ogrenci1.kaydiTamamla();

    qDebug() << "\nMehmet'in kaydı tamamlanıyor:";
    ogrenci2.kaydiTamamla();

    // =============================================
    // 5) DISCONNECT TESTİ
    // =============================================
    qDebug() << "\n--- Disconnect Testi ---";
    QObject::disconnect(&ogrenci1, &Ogrenci::notDegisti,
                        &izleyici, &NotIzleyici::notDegisiminiRaporla);
    qDebug() << "Ogrenci1-İzleyici not bağlantısı koparıldı.";

    qDebug() << "\nAyşe'nin notu güncelleniyor (75, izleyici tepki vermemeli):";
    ogrenci1.notuGuncelle(75);

    qDebug() << "\n[OK] Tüm testler tamamlandı.";

    return 0;
}

#include "03_ozel_sinyal_slot.moc"

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * === Özel Sinyal/Slot Örneği: Öğrenci Sınıfı ===
 *
 * --- Bağlantıları kuruyor ---
 * Bağlantılar kuruldu.
 *
 * --- Not Güncelleme Testi ---
 *
 * Ayşe'nin notu güncelleniyor (85):
 *   [SLOT] notuGuncelle() -> "Ayşe Yılmaz" : eski= 0 , yeni= 85
 *   [İZLEYİCİ] Not değişikliği algılandı: 85 -> "Çok İyi (BA)"
 *
 * Mehmet'in notu güncelleniyor (45):
 *   [SLOT] notuGuncelle() -> "Mehmet Demir" : eski= 0 , yeni= 45
 *   [LAMBDA] "Mehmet Demir" için not değişti: 45
 *
 * Ayşe'nin notu tekrar güncelleniyor (92):
 *   [SLOT] notuGuncelle() -> "Ayşe Yılmaz" : eski= 85 , yeni= 92
 *   [İZLEYİCİ] Not değişikliği algılandı: 92 -> "Mükemmel (AA)"
 *
 * Geçersiz not denemesi (150):
 *   [HATA] "Ayşe Yılmaz" için geçersiz not: 150 (0-100 arası olmalı)
 *
 * Aynı not tekrar (92, sinyal yayılmamalı):
 *   [SLOT] notuGuncelle() -> "Ayşe Yılmaz" : eski= 92 , yeni= 92
 *
 * --- Kayıt Tamamlama Testi ---
 *
 * Ayşe'nin kaydı tamamlanıyor:
 *   [SLOT] kaydiTamamla() -> "Ayşe Yılmaz" (No: 1003 ) kaydı tamamlandı.
 *   [İZLEYİCİ] Kayıt onaylandı, sisteme işlendi.
 *
 * Mehmet'in kaydı tamamlanıyor:
 *   [SLOT] kaydiTamamla() -> "Mehmet Demir" (No: 1007 ) kaydı tamamlandı.
 *   [LAMBDA] "Mehmet Demir" kaydı tamamlandı!
 *
 * --- Disconnect Testi ---
 * Ogrenci1-İzleyici not bağlantısı koparıldı.
 *
 * Ayşe'nin notu güncelleniyor (75, izleyici tepki vermemeli):
 *   [SLOT] notuGuncelle() -> "Ayşe Yılmaz" : eski= 92 , yeni= 75
 *
 * [OK] Tüm testler tamamlandı.
 */

/**
 * Parametreli Sinyal - Sinyal Zincirleme ve Çoklu Slot Bağlantısı
 *
 * Bu program, parametreli özel sinyallerin kullanımını gösterir.
 * QString ve int parametreli sinyaller, bir sinyalin birden fazla
 * slota bağlanması ve sinyal zincirleme (bir sinyal başka bir sinyali
 * tetikler) örneklenmiştir. OBYS öğrenci kayıt senaryosu üzerinden
 * pratik bir uygulama sunulur.
 *
 * Öğrenilecek kavramlar:
 * - Birden fazla parametreli sinyal tanımlama (QString, int)
 * - Bir sinyali birden fazla slota bağlama (fan-out)
 * - Sinyal zincirleme: sinyal -> sinyal (chained signals)
 * - Parametreli sinyali lambda ve sınıf slotuna bağlama
 * - Sinyal parametrelerinin tip uyumluluğu
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 3 - Sinyaller ve Slotlar
 *
 * Derleme:
 *   moc 04_parametreli_sinyal.cpp -o 04_parametreli_sinyal.moc
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Core) \
 *       -o 04_parametreli_sinyal 04_parametreli_sinyal.cpp
 *
 * Çalıştırma: ./04_parametreli_sinyal
 */

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QList>

// =============================================
// OGRENCI FORMU - PARAMETRELİ SİNYAL KAYNAĞI
// =============================================
class OgrenciForm : public QObject {
    Q_OBJECT

public:
    explicit OgrenciForm(QObject* parent = nullptr)
        : QObject(parent) {
        setObjectName("OgrenciForm");
    }

    // Form doldurma simülasyonu: doğrulama yapar ve sinyal yayar
    void kaydet(const QString& isim, int numara, const QString& bolum) {
        qDebug() << "  [FORM] Kayıt işlemi başlatıldı:"
                 << isim << "(" << numara << ")";

        // Doğrulama
        if (isim.isEmpty()) {
            qDebug() << "  [HATA] İsim boş olamaz!";
            emit hataOlustu("Öğrenci ismi boş olamaz.");
            return;
        }

        if (numara < 1000 || numara > 9999) {
            qDebug() << "  [HATA] Geçersiz numara:" << numara;
            emit hataOlustu(
                QString("Geçersiz öğrenci numarası: %1 (1000-9999 arası olmalı)")
                    .arg(numara)
            );
            return;
        }

        // Başarılı kayıt -> sinyaller yay
        // ogrenciKaydedildi 3 FARKLI SLOTA bağlıdır (fan-out örneği)
        emit ogrenciKaydedildi(isim, numara);

        // kayitTamamlandi, ZINCIRLEME sinyal olarak tetiklenecek
        emit kayitTamamlandi(isim, numara, bolum);

        qDebug() << "  [FORM] Kayıt başarılı.";
    }

signals:
    // Basit parametreli sinyal: isim ve numara
    void ogrenciKaydedildi(const QString& isim, int numara);

    // Daha detaylı sinyal: isim, numara ve bölüm
    void kayitTamamlandi(const QString& isim, int numara, const QString& bolum);

    // Hata sinyali: tek QString parametre
    void hataOlustu(const QString& hataMesaji);

    // Zincirleme hedef sinyal: sadece isim
    // Bu sinyal doğrudan başka bir sinyalden tetiklenecek
    void logKaydiOlusturuldu(const QString& isim);
};

// =============================================
// VERİTABANI YÖNETİCİSİ - KAYITLARI SAKLAR
// =============================================
class VeritabaniYoneticisi : public QObject {
    Q_OBJECT

public:
    explicit VeritabaniYoneticisi(QObject* parent = nullptr)
        : QObject(parent) {
        setObjectName("VeritabaniYoneticisi");
    }

    int kayitSayisi() const { return m_kayitlar.size(); }

public slots:
    void ogrenciEkle(const QString& isim, int numara) {
        QString kayit = QString("%1 (%2)").arg(isim).arg(numara);
        m_kayitlar.append(kayit);
        qDebug() << "  [VERİTABANI] Öğrenci eklendi:" << kayit;
        qDebug() << "  [VERİTABANI] Toplam kayıt:" << m_kayitlar.size();
    }

    void kayitlariListele() const {
        qDebug() << "  [VERİTABANI] --- Kayıtlı Öğrenciler ---";
        if (m_kayitlar.isEmpty()) {
            qDebug() << "  [VERİTABANI] (kayıt yok)";
            return;
        }
        for (int i = 0; i < m_kayitlar.size(); ++i) {
            qDebug() << "  " << (i + 1) << "." << m_kayitlar[i];
        }
    }

private:
    QList<QString> m_kayitlar;
};

// =============================================
// LOG YÖNETİCİSİ - OLAYLARI LOGLAR
// =============================================
class LogYoneticisi : public QObject {
    Q_OBJECT

public:
    explicit LogYoneticisi(QObject* parent = nullptr)
        : QObject(parent) {
        setObjectName("LogYoneticisi");
    }

public slots:
    // Zincirleme sinyal hedefi: sadece isim alır
    void logYaz(const QString& isim) {
        qDebug() << "  [LOG] Yeni kayit logu:" << isim
                 << "(zaman: şimdi)";
    }
};

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    qDebug() << "=== Parametreli Sinyal Örneği ===\n";

    // Nesneleri oluştur
    OgrenciForm form;
    VeritabaniYoneticisi veritabani;
    LogYoneticisi logYoneticisi;

    // =============================================
    // 1) BİR SİNYALİ BİRDEN FAZLA SLOTA BAĞLAMA (Fan-out)
    // =============================================
    qDebug() << "--- Bağlantıları kuruyor ---";

    // ogrenciKaydedildi sinyali -> 3 farklı hedef:

    // Hedef 1: VeritabaniYoneticisi::ogrenciEkle (sınıf slotu)
    QObject::connect(&form, &OgrenciForm::ogrenciKaydedildi,
                     &veritabani, &VeritabaniYoneticisi::ogrenciEkle);

    // Hedef 2: Lambda (konsola bilgi yazar)
    QObject::connect(&form, &OgrenciForm::ogrenciKaydedildi,
                     [](const QString& isim, int numara) {
        qDebug() << "  [LAMBDA-1] Yeni öğrenci kaydedildi:"
                 << isim << "- No:" << numara;
    });

    // Hedef 3: Başka bir lambda (istatistik günceller)
    int* toplamKayit = new int(0);
    QObject::connect(&form, &OgrenciForm::ogrenciKaydedildi,
                     [toplamKayit](const QString& isim, int /*numara*/) {
        (*toplamKayit)++;
        qDebug() << "  [LAMBDA-2] İstatistik güncellendi:"
                 << isim << "| Toplam işlem:" << *toplamKayit;
    });

    qDebug() << "[OK] ogrenciKaydedildi -> 3 hedefe bağlandı (fan-out)\n";

    // =============================================
    // 2) SİNYAL ZİNCİRLEME (Signal -> Signal)
    // =============================================
    // Bir sinyal doğrudan başka bir sinyale bağlanabilir.
    // Bu durumda aradaki sinyal "relay" (aktarıcı) görevi görür.
    //
    // Burada: ogrenciKaydedildi(QString, int) sinyalini,
    // logKaydiOlusturuldu(QString) sinyaline zincirliyoruz.
    // Parametre sayısı farklı olduğu için lambda ile uyarlama yapıyoruz.
    QObject::connect(&form, &OgrenciForm::ogrenciKaydedildi,
                     &form, [&form](const QString& isim, int /*numara*/) {
        // Zincirleme: başka bir sinyal yay
        emit form.logKaydiOlusturuldu(isim);
    });

    // Zincirin devamı: logKaydiOlusturuldu -> LogYoneticisi::logYaz
    QObject::connect(&form, &OgrenciForm::logKaydiOlusturuldu,
                     &logYoneticisi, &LogYoneticisi::logYaz);

    qDebug() << "[OK] Sinyal zinciri kuruldu:";
    qDebug() << "     ogrenciKaydedildi -> logKaydiOlusturuldu -> logYaz\n";

    // =============================================
    // 3) KAYITTAMAMLANDI SİNYALİ (3 PARAMETRE)
    // =============================================
    QObject::connect(&form, &OgrenciForm::kayitTamamlandi,
                     [](const QString& isim, int numara, const QString& bolum) {
        qDebug() << "  [DETAY] Kayıt detayı:" << isim
                 << "| No:" << numara << "| Bölüm:" << bolum;
    });

    // =============================================
    // 4) HATA SİNYALİ BAĞLANTISI
    // =============================================
    QObject::connect(&form, &OgrenciForm::hataOlustu,
                     [](const QString& mesaj) {
        qDebug() << "  [HATA YAKALAYICI]" << mesaj;
    });

    qDebug() << "Tüm bağlantılar kuruldu.\n";

    // =============================================
    // 5) BAŞARILI KAYITLAR
    // =============================================
    qDebug() << "--- Başarılı Kayıtlar ---";

    qDebug() << "\nKayit 1:";
    form.kaydet("Ayşe Yılmaz", 1003, "Bilgisayar Bilimleri");

    qDebug() << "\nKayit 2:";
    form.kaydet("Mehmet Demir", 1007, "Yazılım Mühendisliği");

    qDebug() << "\nKayit 3:";
    form.kaydet("Zeynep Kara", 2001, "Matematik");

    // =============================================
    // 6) HATALI KAYITLAR
    // =============================================
    qDebug() << "\n--- Hatalı Kayıtlar ---";

    qDebug() << "\nHatalı Kayıt 1 (boş isim):";
    form.kaydet("", 1005, "Fizik");

    qDebug() << "\nHatalı Kayıt 2 (geçersiz numara):";
    form.kaydet("Ali Öztürk", 50, "Kimya");

    // =============================================
    // 7) VERİTABANI DURUMU
    // =============================================
    qDebug() << "\n--- Veritabanı Durumu ---";
    veritabani.kayitlariListele();
    qDebug() << "\nToplam başarılı kayıt:" << veritabani.kayitSayisi();
    qDebug() << "Toplam işlem (istatistik):" << *toplamKayit;

    // Temizlik
    delete toplamKayit;

    qDebug() << "\n[OK] Tüm işlemler tamamlandı.";

    return 0;
}

#include "04_parametreli_sinyal.moc"

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * === Parametreli Sinyal Örneği ===
 *
 * --- Bağlantıları kuruyor ---
 * [OK] ogrenciKaydedildi -> 3 hedefe bağlandı (fan-out)
 *
 * [OK] Sinyal zinciri kuruldu:
 *      ogrenciKaydedildi -> logKaydiOlusturuldu -> logYaz
 *
 * Tüm bağlantılar kuruldu.
 *
 * --- Başarılı Kayıtlar ---
 *
 * Kayıt 1:
 *   [FORM] Kayıt işlemi başlatıldı: "Ayşe Yılmaz" ( 1003 )
 *   [VERİTABANI] Öğrenci eklendi: "Ayşe Yılmaz (1003)"
 *   [VERİTABANI] Toplam kayıt: 1
 *   [LAMBDA-1] Yeni öğrenci kaydedildi: "Ayşe Yılmaz" - No: 1003
 *   [LAMBDA-2] İstatistik güncellendi: "Ayşe Yılmaz" | Toplam işlem: 1
 *   [LOG] Yeni kayıt logu: "Ayşe Yılmaz" (zaman: şimdi)
 *   [DETAY] Kayıt detayı: "Ayşe Yılmaz" | No: 1003 | Bölüm: "Bilgisayar Bilimleri"
 *   [FORM] Kayıt başarılı.
 *
 * Kayıt 2:
 *   [FORM] Kayıt işlemi başlatıldı: "Mehmet Demir" ( 1007 )
 *   [VERİTABANI] Öğrenci eklendi: "Mehmet Demir (1007)"
 *   [VERİTABANI] Toplam kayıt: 2
 *   [LAMBDA-1] Yeni öğrenci kaydedildi: "Mehmet Demir" - No: 1007
 *   [LAMBDA-2] İstatistik güncellendi: "Mehmet Demir" | Toplam işlem: 2
 *   [LOG] Yeni kayıt logu: "Mehmet Demir" (zaman: şimdi)
 *   [DETAY] Kayıt detayı: "Mehmet Demir" | No: 1007 | Bölüm: "Yazılım Mühendisliği"
 *   [FORM] Kayıt başarılı.
 *
 * Kayıt 3:
 *   [FORM] Kayıt işlemi başlatıldı: "Zeynep Kara" ( 2001 )
 *   [VERİTABANI] Öğrenci eklendi: "Zeynep Kara (2001)"
 *   [VERİTABANI] Toplam kayıt: 3
 *   [LAMBDA-1] Yeni öğrenci kaydedildi: "Zeynep Kara" - No: 2001
 *   [LAMBDA-2] İstatistik güncellendi: "Zeynep Kara" | Toplam işlem: 3
 *   [LOG] Yeni kayıt logu: "Zeynep Kara" (zaman: şimdi)
 *   [DETAY] Kayıt detayı: "Zeynep Kara" | No: 2001 | Bölüm: "Matematik"
 *   [FORM] Kayıt başarılı.
 *
 * --- Hatalı Kayıtlar ---
 *
 * Hatalı Kayıt 1 (boş isim):
 *   [FORM] Kayıt işlemi başlatıldı: "" ( 1005 )
 *   [HATA] İsim boş olamaz!
 *   [HATA YAKALAYICI] "Öğrenci ismi boş olamaz."
 *
 * Hatalı Kayıt 2 (geçersiz numara):
 *   [FORM] Kayıt işlemi başlatıldı: "Ali Öztürk" ( 50 )
 *   [HATA] Geçersiz numara: 50
 *   [HATA YAKALAYICI] "Geçersiz öğrenci numarası: 50 (1000-9999 arası olmalı)"
 *
 * --- Veritabanı Durumu ---
 *   [VERİTABANI] --- Kayıtlı Öğrenciler ---
 *   1. "Ayşe Yılmaz (1003)"
 *   2. "Mehmet Demir (1007)"
 *   3. "Zeynep Kara (2001)"
 *
 * Toplam başarılı kayıt: 3
 * Toplam işlem (istatistik): 3
 *
 * [OK] Tüm işlemler tamamlandı.
 */

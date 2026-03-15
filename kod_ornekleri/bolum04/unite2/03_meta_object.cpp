/**
 * Meta-Object Sistemi - Q_PROPERTY ve Dinamik Özellikler
 *
 * Bu program, Qt'nin Meta-Object sisteminin temel özelliklerini
 * gösterir: Q_PROPERTY ile bildirilen özellikler, property()/
 * setProperty() ile dinamik erişim, className() ve metaObject()
 * ile çalışma zamanı tür bilgisi sorgulama.
 *
 * Öğrenilecek kavramlar:
 * - Q_OBJECT ve Q_PROPERTY makroları
 * - Meta-object ile çalışma zamanı tür bilgisi
 * - property() ve setProperty() ile dinamik erişim
 * - className() ve metaObject() kullanımı
 * - Dinamik özellik (dynamic property) ekleme
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 2 - Qt'nin Temel Yapısı
 *
 * Derleme (moc gerektiği için CMake veya qmake önerilir):
 *   Bu dosya Q_OBJECT makrosu içerdiği için moc işlenmesi gerekir.
 *   En kolay yol: dosyanın sonundaki #include "03_meta_object.moc"
 *   satırını ekleyip aşağıdaki gibi derlemektir:
 *
 *   moc 03_meta_object.cpp -o 03_meta_object.moc
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Core) \
 *       -o 03_meta_object 03_meta_object.cpp
 */

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

// -- OBYS bağlamında: Öğrenci sınıfı --
class Ogrenci : public QObject {
    Q_OBJECT

    // Q_PROPERTY: Meta-object sistemi üzerinden erişilebilir özellik
    // READ  -> okuma fonksiyonu
    // WRITE -> yazma fonksiyonu
    // NOTIFY -> değer değiştiğinde yayılan sinyal
    Q_PROPERTY(QString isim READ isim WRITE setIsim NOTIFY isimDegisti)
    Q_PROPERTY(int numara READ numara WRITE setNumara NOTIFY numaraDegisti)
    Q_PROPERTY(double ortalama READ ortalama WRITE setOrtalama NOTIFY ortalamaDegisti)

public:
    explicit Ogrenci(QObject* parent = nullptr)
        : QObject(parent), m_isim(""), m_numara(0), m_ortalama(0.0) {}

    // -- Okuma (getter) fonksiyonları --
    QString isim() const { return m_isim; }
    int numara() const { return m_numara; }
    double ortalama() const { return m_ortalama; }

    // -- Yazma (setter) fonksiyonları --
    void setIsim(const QString& isim) {
        if (m_isim != isim) {
            m_isim = isim;
            emit isimDegisti(m_isim);
        }
    }

    void setNumara(int numara) {
        if (m_numara != numara) {
            m_numara = numara;
            emit numaraDegisti(m_numara);
        }
    }

    void setOrtalama(double ortalama) {
        if (qFuzzyCompare(m_ortalama, ortalama) == false) {
            m_ortalama = ortalama;
            emit ortalamaDegisti(m_ortalama);
        }
    }

signals:
    void isimDegisti(const QString& yeniIsim);
    void numaraDegisti(int yeniNumara);
    void ortalamaDegisti(double yeniOrtalama);

private:
    QString m_isim;
    int m_numara;
    double m_ortalama;
};

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    qDebug() << "=== Meta-Object Sistemi Örneği ===\n";

    // Öğrenci nesnesi oluştur
    Ogrenci ogr;
    ogr.setObjectName("ogrenci_1");

    // =============================================
    // 1) SINIF BİLGİSİ (className)
    // =============================================
    qDebug() << "--- Sınıf Bilgisi ---";
    qDebug() << "Sınıf adı:" << ogr.metaObject()->className();
    qDebug() << "Üst sınıf:" << ogr.metaObject()->superClass()->className();
    qDebug() << "Object name:" << ogr.objectName();

    // =============================================
    // 2) Q_PROPERTY İLE DEĞER ATAMA (setter)
    // =============================================
    qDebug() << "\n--- Doğrudan Setter ile Değer Atama ---";
    ogr.setIsim("Ayse Yilmaz");
    ogr.setNumara(1003);
    ogr.setOrtalama(85.5);

    qDebug() << "İsim:" << ogr.isim();
    qDebug() << "Numara:" << ogr.numara();
    qDebug() << "Ortalama:" << ogr.ortalama();

    // =============================================
    // 3) DİNAMİK ERİŞİM (property / setProperty)
    // =============================================
    qDebug() << "\n--- property() ile Dinamik Okuma ---";

    // property() string anahtar ile değer döndürür (QVariant)
    QVariant isimVar = ogr.property("isim");
    QVariant numaraVar = ogr.property("numara");
    QVariant ortalamaVar = ogr.property("ortalama");

    qDebug() << "isim (QVariant):" << isimVar.toString();
    qDebug() << "numara (QVariant):" << numaraVar.toInt();
    qDebug() << "ortalama (QVariant):" << ortalamaVar.toDouble();

    qDebug() << "\n--- setProperty() ile Dinamik Yazma ---";

    // setProperty() string anahtar ile değer atar
    ogr.setProperty("isim", "Mehmet Demir");
    ogr.setProperty("numara", 1007);
    ogr.setProperty("ortalama", 92.1);

    qDebug() << "Güncellenmiş isim:" << ogr.property("isim").toString();
    qDebug() << "Güncellenmiş numara:" << ogr.property("numara").toInt();
    qDebug() << "Güncellenmiş ortalama:" << ogr.property("ortalama").toDouble();

    // =============================================
    // 4) META-OBJECT ÖZELLİK LİSTESİ
    // =============================================
    qDebug() << "\n--- Meta-Object Özellik Listesi ---";

    const QMetaObject* meta = ogr.metaObject();
    qDebug() << "Toplam özellik sayısı:" << meta->propertyCount();

    // propertyOffset(): Bu sınıfa ait özelliklerin başladığı indeks.
    // Önceki indeksler üst sınıftan (QObject) miras alınan özelliklerdir.
    int baslangic = meta->propertyOffset();
    qDebug() << "Bu sınıfa ait özellikler (indeks" << baslangic << "den itibaren):";

    for (int i = baslangic; i < meta->propertyCount(); ++i) {
        QMetaProperty ozellik = meta->property(i);
        qDebug() << "  ->" << ozellik.name()
                 << "| Tip:" << ozellik.typeName()
                 << "| Okunabilir:" << ozellik.isReadable()
                 << "| Yazılabilir:" << ozellik.isWritable();
    }

    // =============================================
    // 5) DİNAMİK ÖZELLİK EKLEME
    // =============================================
    qDebug() << "\n--- Dinamik Özellik (Q_PROPERTY dışında) ---";

    // Q_PROPERTY ile tanımlanmamış bir özellik eklenebilir
    ogr.setProperty("bolum", "Bilgisayar Bilimleri");
    ogr.setProperty("donem", 6);

    qDebug() << "Dinamik özellik - bölüm:" << ogr.property("bolum").toString();
    qDebug() << "Dinamik özellik - dönem:" << ogr.property("donem").toInt();

    // Dinamik özellikler dynamicPropertyNames() ile listelenebilir
    qDebug() << "Dinamik özellik isimleri:" << ogr.dynamicPropertyNames();

    // =============================================
    // 6) inherits() İLE TÜR KONTROLÜ
    // =============================================
    qDebug() << "\n--- inherits() ile Tür Kontrolü ---";
    qDebug() << "Öğrenci, QObject mi?" << ogr.inherits("QObject");
    qDebug() << "Öğrenci, Öğrenci mi?" << ogr.inherits("Ogrenci");
    qDebug() << "Öğrenci, QWidget mi?" << ogr.inherits("QWidget");

    return 0;
}

// moc tarafından üretilen kodu dahil et.
// Bu satır, Q_OBJECT makrosu kullanan sınıflar için gereklidir.
#include "03_meta_object.moc"

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * === Meta-Object Sistemi Örneği ===
 *
 * --- Sınıf Bilgisi ---
 * Sınıf adı: Ogrenci
 * Üst sınıf: QObject
 * Object name: "ogrenci_1"
 *
 * --- Doğrudan Setter ile Değer Atama ---
 * İsim: "Ayse Yilmaz"
 * Numara: 1003
 * Ortalama: 85.5
 *
 * --- property() ile Dinamik Okuma ---
 * isim (QVariant): "Ayse Yilmaz"
 * numara (QVariant): 1003
 * ortalama (QVariant): 85.5
 *
 * --- setProperty() ile Dinamik Yazma ---
 * Güncellenmiş isim: "Mehmet Demir"
 * Güncellenmiş numara: 1007
 * Güncellenmiş ortalama: 92.1
 *
 * --- Meta-Object Özellik Listesi ---
 * Toplam özellik sayısı: 4
 * Bu sınıfa ait özellikler (indeks 1 den itibaren):
 *   -> isim | Tip: QString | Okunabilir: true | Yazılabilir: true
 *   -> numara | Tip: int | Okunabilir: true | Yazılabilir: true
 *   -> ortalama | Tip: double | Okunabilir: true | Yazılabilir: true
 *
 * --- Dinamik Özellik (Q_PROPERTY dışında) ---
 * Dinamik özellik - bölüm: "Bilgisayar Bilimleri"
 * Dinamik özellik - dönem: 6
 * Dinamik özellik isimleri: QList("bolum", "donem")
 *
 * --- inherits() ile Tür Kontrolü ---
 * Öğrenci, QObject mi? true
 * Öğrenci, Öğrenci mi? true
 * Öğrenci, QWidget mi? false
 */

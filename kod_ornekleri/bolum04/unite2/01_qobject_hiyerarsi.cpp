/**
 * QObject Hiyerarşisi - Nesne Ağacı ve Çocuk Yönetimi
 *
 * Bu program, Qt'nin nesne hiyerarşisini (object tree) gösterir.
 * QObject nesneleri parent-child ilişkisi ile bir ağaç yapısı
 * oluşturur. Bu yapı bellek yönetimi ve nesne keşfini kolaylaştırır.
 *
 * Öğrenilecek kavramlar:
 * - QObject parent-child ilişkisi
 * - setObjectName() ile nesneleri adlandırma
 * - children() ile çocuk nesnelere erişim
 * - findChild() ve findChildren() ile nesne arama
 * - qDebug() ile hata ayıklama çıktısı
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 2 - Qt'nin Temel Yapısı
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Core) \
 *       -o 01_qobject_hiyerarsi 01_qobject_hiyerarsi.cpp
 */

#include <QCoreApplication>
#include <QObject>
#include <QDebug>

// -- Nesne ağacını özyinelemeli olarak yazdıran yardımcı fonksiyon --
void hiyerarsiYazdir(const QObject* nesne, int girinti = 0) {
    // Girinti oluştur
    QString bosluk(girinti * 2, ' ');

    // Nesne bilgilerini yazdır
    qDebug().noquote() << bosluk
        << QString("[%1] objectName: \"%2\", çocuk sayısı: %3")
               .arg(nesne->metaObject()->className())
               .arg(nesne->objectName())
               .arg(nesne->children().size());

    // Çocuk nesneleri özyinelemeli olarak yazdır
    for (const QObject* cocuk : nesne->children()) {
        hiyerarsiYazdir(cocuk, girinti + 1);
    }
}

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    // =============================================
    // 1) NESNE AĞACI OLUŞTURMA
    // =============================================
    qDebug() << "=== QObject Hiyerarşisi Örneği ===\n";

    // Kök nesne (parent = nullptr)
    QObject* okul = new QObject();
    okul->setObjectName("Universite");

    // Birinci seviye çocuklar
    QObject* fakulte1 = new QObject(okul);  // parent = okul
    fakulte1->setObjectName("FenFakultesi");

    QObject* fakulte2 = new QObject(okul);  // parent = okul
    fakulte2->setObjectName("MuhendislikFakultesi");

    // İkinci seviye çocuklar (bölümler)
    QObject* bolum1 = new QObject(fakulte1);  // parent = fakulte1
    bolum1->setObjectName("BilgisayarBilimleri");

    QObject* bolum2 = new QObject(fakulte1);
    bolum2->setObjectName("Matematik");

    QObject* bolum3 = new QObject(fakulte2);
    bolum3->setObjectName("YazilimMuhendisligi");

    // Üçüncü seviye çocuklar (öğrenciler)
    QObject* ogr1 = new QObject(bolum1);
    ogr1->setObjectName("AyseYilmaz");

    QObject* ogr2 = new QObject(bolum1);
    ogr2->setObjectName("MehmetDemir");

    QObject* ogr3 = new QObject(bolum3);
    ogr3->setObjectName("ZeynepKara");

    // =============================================
    // 2) HİYERARŞİYİ YAZDIRMA
    // =============================================
    qDebug() << "--- Tam Hiyerarşi ---";
    hiyerarsiYazdir(okul);

    // =============================================
    // 3) ÇOCUK NESNELERE ERİŞİM
    // =============================================
    qDebug() << "\n--- children() ile Doğrudan Çocuklar ---";
    const QObjectList& cocuklar = okul->children();
    qDebug() << "Üniversite'nin çocuk sayısı:" << cocuklar.size();
    for (const QObject* cocuk : cocuklar) {
        qDebug() << "  ->" << cocuk->objectName();
    }

    // =============================================
    // 4) NESNE ARAMA (findChild / findChildren)
    // =============================================
    qDebug() << "\n--- findChild() ile Tek Nesne Arama ---";

    // Kök nesneden itibaren özyinelemeli arama yapar
    QObject* bulunan = okul->findChild<QObject*>("AyseYilmaz");
    if (bulunan) {
        qDebug() << "Bulundu:" << bulunan->objectName();
        qDebug() << "Parent:" << bulunan->parent()->objectName();
    }

    qDebug() << "\n--- findChildren() ile Çoklu Arama ---";

    // Tüm QObject tipindeki çocukları bul (özyinelemeli)
    QList<QObject*> tumNesneler = okul->findChildren<QObject*>();
    qDebug() << "Toplam nesne sayısı (kök hariç):" << tumNesneler.size();
    for (const QObject* n : tumNesneler) {
        qDebug() << "  ->" << n->objectName()
                 << "(parent:" << n->parent()->objectName() << ")";
    }

    // =============================================
    // 5) PARENT DEĞİŞTİRME
    // =============================================
    qDebug() << "\n--- Parent Değiştirme ---";
    qDebug() << "Öncesi - MehmetDemir'in parent'ı:" << ogr2->parent()->objectName();

    // Mehmet'i Yazılım Mühendisliği bölümüne transfer et
    ogr2->setParent(bolum3);

    qDebug() << "Sonrası - MehmetDemir'in parent'ı:" << ogr2->parent()->objectName();

    qDebug() << "\n--- Güncellenmiş Hiyerarşi ---";
    hiyerarsiYazdir(okul);

    // =============================================
    // 6) TEMİZLİK
    // =============================================
    // Kök nesneyi sildiğimizde tüm çocuklar otomatik silinir.
    // Bu Qt'nin bellek yönetim mekanizmasının temelidir.
    delete okul;

    qDebug() << "\n[OK] Tüm nesneler temizlendi.";

    return 0;
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * === QObject Hiyerarşisi Örneği ===
 *
 * --- Tam Hiyerarşi ---
 * [QObject] objectName: "Universite", çocuk sayısı: 2
 *   [QObject] objectName: "FenFakultesi", çocuk sayısı: 2
 *     [QObject] objectName: "BilgisayarBilimleri", çocuk sayısı: 2
 *       [QObject] objectName: "AyseYilmaz", çocuk sayısı: 0
 *       [QObject] objectName: "MehmetDemir", çocuk sayısı: 0
 *     [QObject] objectName: "Matematik", çocuk sayısı: 0
 *   [QObject] objectName: "MuhendislikFakultesi", çocuk sayısı: 1
 *     [QObject] objectName: "YazilimMuhendisligi", çocuk sayısı: 1
 *       [QObject] objectName: "ZeynepKara", çocuk sayısı: 0

 *
 * --- children() ile Doğrudan Çocuklar ---
 * Üniversite'nin çocuk sayısı: 2
 *   -> "FenFakultesi"
 *   -> "MuhendislikFakultesi"
 *
 * --- findChild() ile Tek Nesne Arama ---
 * Bulundu: "AyseYilmaz"
 * Parent: "BilgisayarBilimleri"
 *
 * --- findChildren() ile Çoklu Arama ---
 * Toplam nesne sayısı (kök hariç): 8
 *   -> "FenFakultesi" (parent: "Universite")
 *   -> "BilgisayarBilimleri" (parent: "FenFakultesi")
 *   -> "AyseYilmaz" (parent: "BilgisayarBilimleri")
 *   -> "MehmetDemir" (parent: "BilgisayarBilimleri")
 *   -> "Matematik" (parent: "FenFakultesi")
 *   -> "MuhendislikFakultesi" (parent: "Universite")
 *   -> "YazilimMuhendisligi" (parent: "MuhendislikFakultesi")
 *   -> "ZeynepKara" (parent: "YazilimMuhendisligi")
 *
 * --- Parent Değiştirme ---
 * Öncesi - MehmetDemir'in parent'ı: "BilgisayarBilimleri"
 * Sonrası - MehmetDemir'in parent'ı: "YazilimMuhendisligi"
 *
 * --- Güncellenmiş Hiyerarşi ---
 * [QObject] objectName: "Universite", çocuk sayısı: 2
 *   [QObject] objectName: "FenFakultesi", çocuk sayısı: 2
 *     [QObject] objectName: "BilgisayarBilimleri", çocuk sayısı: 1
 *       [QObject] objectName: "AyseYilmaz", çocuk sayısı: 0
 *     [QObject] objectName: "Matematik", çocuk sayısı: 0
 *   [QObject] objectName: "MuhendislikFakultesi", çocuk sayısı: 1
 *     [QObject] objectName: "YazilimMuhendisligi", çocuk sayısı: 2
 *       [QObject] objectName: "ZeynepKara", çocuk sayısı: 0
 *       [QObject] objectName: "MehmetDemir", çocuk sayısı: 0
 *
 * [OK] Tüm nesneler temizlendi.
 */

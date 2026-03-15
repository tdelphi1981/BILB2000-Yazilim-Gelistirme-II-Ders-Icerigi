/**
 * Qt Bellek Yönetimi - Parent-Child Otomatik Silme
 *
 * Bu program, Qt'nin parent-child mekanizması ile otomatik bellek
 * yönetimini gösterir. Bir parent nesne silindiğinde tüm çocuk
 * nesneleri de otomatik olarak silinir. destroyed() sinyali ile
 * silme işlemlerini gözlemliyoruz.
 *
 * Öğrenilecek kavramlar:
 * - Parent-child otomatik bellek yönetimi
 * - destroyed() sinyali ile silme izleme
 * - QObject::deleteLater() kullanımı
 * - Bellek sızıntısı önleme stratejileri
 *
 * Bölüm: 04 - Qt Framework'üne Giriş
 * Ünite: 2 - Qt'nin Temel Yapısı
 *
 * Derleme:
 *   g++ -std=c++20 -fPIC $(pkg-config --cflags --libs Qt6Core) \
 *       -o 02_bellek_yonetimi 02_bellek_yonetimi.cpp
 */

#include <QCoreApplication>
#include <QObject>
#include <QDebug>

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);

    qDebug() << "=== Qt Bellek Yönetimi Örneği ===\n";

    // =============================================
    // 1) PARENT-CHILD OTOMATİK SİLME
    // =============================================
    qDebug() << "--- Parent-Child Otomatik Silme ---";

    // Parent nesnesi oluştur
    QObject* sinif = new QObject();
    sinif->setObjectName("BilgisayarBilimleri_3A");

    // 3 çocuk nesne oluştur (parent = sinif)
    QObject* ogrenci1 = new QObject(sinif);
    ogrenci1->setObjectName("Ayse");

    QObject* ogrenci2 = new QObject(sinif);
    ogrenci2->setObjectName("Mehmet");

    QObject* ogrenci3 = new QObject(sinif);
    ogrenci3->setObjectName("Zeynep");

    // Her nesnenin destroyed() sinyaline lambda bağla.
    // destroyed() sinyali, nesne silinmeden hemen önce yayılır.
    QObject::connect(sinif, &QObject::destroyed, [](QObject* nesne) {
        qDebug() << "  [SILINDI] Parent:" << nesne->objectName();
    });

    QObject::connect(ogrenci1, &QObject::destroyed, [](QObject* nesne) {
        qDebug() << "  [SILINDI] Çocuk:" << nesne->objectName();
    });

    QObject::connect(ogrenci2, &QObject::destroyed, [](QObject* nesne) {
        qDebug() << "  [SILINDI] Çocuk:" << nesne->objectName();
    });

    QObject::connect(ogrenci3, &QObject::destroyed, [](QObject* nesne) {
        qDebug() << "  [SILINDI] Çocuk:" << nesne->objectName();
    });

    qDebug() << "Silmeden önce çocuk sayısı:" << sinif->children().size();
    qDebug() << "Parent siliniyor...\n";

    // Parent'ı sil -> tüm çocuklar otomatik silinir!
    delete sinif;
    // Bu noktadan sonra ogrenci1, ogrenci2, ogrenci3 işaretçileri geçersizdir.

    qDebug() << "\n[OK] Parent ve tüm çocuklar silindi.\n";

    // =============================================
    // 2) TEK ÇOCUK SİLME
    // =============================================
    qDebug() << "--- Tek Çocuk Silme ---";

    QObject* bolum = new QObject();
    bolum->setObjectName("YazilimMuh");

    QObject* ogr_a = new QObject(bolum);
    ogr_a->setObjectName("Ali");

    QObject* ogr_b = new QObject(bolum);
    ogr_b->setObjectName("Fatma");

    QObject::connect(ogr_a, &QObject::destroyed, [](QObject* nesne) {
        qDebug() << "  [SILINDI]" << nesne->objectName();
    });

    QObject::connect(ogr_b, &QObject::destroyed, [](QObject* nesne) {
        qDebug() << "  [SILINDI]" << nesne->objectName();
    });

    QObject::connect(bolum, &QObject::destroyed, [](QObject* nesne) {
        qDebug() << "  [SILINDI]" << nesne->objectName();
    });

    qDebug() << "Çocuk sayısı (silmeden önce):" << bolum->children().size();

    // Sadece bir çocuğu sil
    delete ogr_a;

    qDebug() << "Çocuk sayısı (Ali silindikten sonra):" << bolum->children().size();

    // Geri kalan parent ve çocukları temizle
    qDebug() << "Parent siliniyor...";
    delete bolum;

    qDebug() << "\n[OK] İşlem tamamlandı.";

    // =============================================
    // 3) PARENT DEĞİŞTİRME VE BELLEK
    // =============================================
    qDebug() << "\n--- Parent Değiştirme ---";

    QObject* grup_a = new QObject();
    grup_a->setObjectName("GrupA");

    QObject* grup_b = new QObject();
    grup_b->setObjectName("GrupB");

    QObject* ogrenci = new QObject(grup_a);
    ogrenci->setObjectName("Can");

    qDebug() << "Can'ın parent'ı:" << ogrenci->parent()->objectName();
    qDebug() << "GrupA çocuk sayısı:" << grup_a->children().size();
    qDebug() << "GrupB çocuk sayısı:" << grup_b->children().size();

    // Parent'ı değiştir -> eski parent'tan otomatik çıkar
    ogrenci->setParent(grup_b);

    qDebug() << "\nParent değiştirildi (GrupA -> GrupB):";
    qDebug() << "Can'ın parent'ı:" << ogrenci->parent()->objectName();
    qDebug() << "GrupA çocuk sayısı:" << grup_a->children().size();
    qDebug() << "GrupB çocuk sayısı:" << grup_b->children().size();

    // Temizlik: iki kök nesneyi sil
    delete grup_a;  // çocuğu yok (Can başka gruba geçti)
    delete grup_b;  // Can da otomatik silinir

    qDebug() << "\n[OK] Tüm bellek temizlendi.";

    return 0;
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * === Qt Bellek Yönetimi Örneği ===
 *
 * --- Parent-Child Otomatik Silme ---
 * Silmeden önce çocuk sayısı: 3
 * Parent siliniyor...
 *
 *   [SILINDI] Çocuk: "Ayse"
 *   [SILINDI] Çocuk: "Mehmet"
 *   [SILINDI] Çocuk: "Zeynep"
 *   [SILINDI] Parent: "BilgisayarBilimleri_3A"
 *
 * [OK] Parent ve tüm çocuklar silindi.
 *
 * --- Tek Çocuk Silme ---
 * Çocuk sayısı (silmeden önce): 2
 *   [SILINDI] "Ali"
 * Çocuk sayısı (Ali silindikten sonra): 1
 * Parent siliniyor...
 *   [SILINDI] "Fatma"
 *   [SILINDI] "YazilimMuh"
 *
 * [OK] İşlem tamamlandı.
 *
 * --- Parent Değiştirme ---
 * Can'ın parent'ı: "GrupA"
 * GrupA çocuk sayısı: 1
 * GrupB çocuk sayısı: 0
 *
 * Parent değiştirildi (GrupA -> GrupB):
 * Can'ın parent'ı: "GrupB"
 * GrupA çocuk sayısı: 0
 * GrupB çocuk sayısı: 1
 *
 * [OK] Tüm bellek temizlendi.
 */

/**
 * Qt Designer ve UI Dosyası - .ui Dosyası Kullanımı
 *
 * Bu program, Qt Designer ile oluşturulmuş bir .ui dosyasının
 * C++ kodunda nasıl kullanıldığını gösterir. uic aracı .ui dosyasını
 * bir C++ başlık dosyasına dönüştürür ve Ui::KayitFormu sınıfı
 * üzerinden widget'lara erişim sağlanır (tek kalıtım yaklaşımı).
 *
 * Öğrenilecek kavramlar:
 * - Qt Designer .ui dosya formatı (XML)
 * - uic ile otomatik kod üretimi
 * - Ui::Form kalıbı (single inheritance approach)
 * - setupUi() ile arayüz kurulumu
 * - CMake AUTOUIC özelliği
 *
 * Bölüm: 05 - Qt Pencere ve Düzen Yönetimi
 * Ünite: 4 - Qt Designer ve UI Dosyaları
 *
 * Derleme (CMake ile - AUTOUIC etkin olmalı):
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
 *   cmake --build .
 *
 * [NOT] Bu dosya tek başına derlenemez, CMakeLists.txt gerektirir.
 * CMake, AUTOUIC sayesinde .ui dosyasını otomatik olarak işler.
 */

#include <QApplication>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>

// uic tarafından otomatik oluşturulan başlık dosyası
// CMake AUTOUIC bu dosyayı 01_designer_ornek.ui'den üretir
#include "ui_01_designer_ornek.h"

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);

    // =============================================
    // 1) WIDGET VE UI KURULUMU
    // =============================================
    QWidget pencere;

    // Ui sınıfı: .ui dosyasındaki tüm widget'lara erişim sağlar
    Ui::KayitFormu ui;
    ui.setupUi(&pencere);  // Widget'ları pencereye yerleştir

    // =============================================
    // 2) SİNYAL/SLOT BAĞLANTILARI
    // =============================================
    // ui.kaydetButon, ui.isimGiris gibi isimlerle widget'lara eriş
    QObject::connect(ui.kaydetButon, &QPushButton::clicked,
                     &pencere, [&ui, &pencere]() {
        QString isim = ui.isimGiris->text().trimmed();

        if (isim.isEmpty()) {
            ui.durumEtiket->setText("[HATA] İsim boş bırakılamaz!");
            ui.durumEtiket->setStyleSheet("color: #e74c3c; padding: 5px;");
            return;
        }

        ui.durumEtiket->setText(QString("[OK] %1 kaydedildi!").arg(isim));
        ui.durumEtiket->setStyleSheet("color: #27ae60; padding: 5px;");

        QMessageBox::information(&pencere, "Başarılı",
                                 QString("%1 başarıyla kaydedildi.").arg(isim));
        qDebug() << "[OK] Öğrenci kaydedildi:" << isim;
    });

    pencere.show();

    return uygulama.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * [GUI Penceresi]
 * Başlık: "OBYS - Kayıt Formu (Designer)"
 * Boyut: 400x250 piksel
 *
 * İçerik (.ui dosyasından oluşturulur):
 *   "Öğrenci Kayıt Formu" başlığı (mavi arka plan)
 *   Ad Soyad:
 *   [__________________]  (placeholder: Öğrenci adını giriniz)
 *   [durum etiketi]
 *   [Kaydet]              (yeşil buton)
 *
 * Davranış:
 *   - Boş isimle "Kaydet" -> kırmızı hata mesajı
 *   - Geçerli isimle "Kaydet" -> yeşil başarı mesajı + QMessageBox
 *
 * Derleme notu:
 *   Bu dosya CMake AUTOUIC ile derlenmelidir.
 *   CMake, .ui dosyasını otomatik olarak ui_01_designer_ornek.h'ye dönüştürür.
 */

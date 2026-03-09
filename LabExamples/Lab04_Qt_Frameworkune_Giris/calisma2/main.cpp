/**
 * Lab 04 - Çalışma 2 Başlangıç Kodu
 * Parent-Child ve QObject
 *
 * Bu dosya Lab 04 föyü ile birlikte kullanılır.
 * "GOREV" işaretli bölgeleri tamamlayın.
 *
 * Öğrenilecek kavramlar:
 * - QObject parent-child hiyerarşisi
 * - setObjectName() ile nesne adlandırma
 * - children() ile alt nesne listesi
 * - findChild<T>() ve findChildren<T>() ile arama
 * - metaObject()->className() ile tür bilgisi
 *
 * Lab: 04 - Qt Framework'üne Giriş
 * Çalışma: 2 - Parent-Child ve QObject (25 dk)
 *
 * Derleme: mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştırma: ./app
 */

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Ana pencere (parent)
    auto* pencere = new QWidget;
    pencere->setWindowTitle("QObject Hiyerarsisi");
    pencere->resize(400, 300);

    // Child widget'lar
    auto* baslik = new QLabel("OBYS Ana Menu", pencere);
    baslik->setObjectName("baslikLabel");
    baslik->setGeometry(120, 20, 200, 30);

    auto* buton1 = new QPushButton("Ogrenci Listesi", pencere);
    buton1->setObjectName("listeButon");
    buton1->setGeometry(120, 70, 160, 35);

    auto* buton2 = new QPushButton("Not Girisi", pencere);
    buton2->setObjectName("notButon");
    buton2->setGeometry(120, 120, 160, 35);

    // Hiyerarsiyi incele
    qDebug() << "Cocuk sayisi:" << pencere->children().size();
    for (auto* child : pencere->children()) {
        qDebug() << " -" << child->objectName()
                 << "(" << child->metaObject()->className() << ")";
    }

    // findChild ile belirli bir nesneyi bul
    auto* bulunan = pencere->findChild<QPushButton*>("notButon");
    if (bulunan) {
        qDebug() << "Bulunan buton:" << bulunan->text();
    }

    pencere->show();

    // === GOREV 2.1 - QObject Hiyerarsisi Kesfi (10 dk) ===
    // TODO: a) Pencereye 2 adet daha QPushButton ekleyin:
    //          "Devamsizlik" ve "Cikis" butonlari
    //          auto* buton3 = new QPushButton("Devamsizlik", pencere);
    //          buton3->setObjectName("devamsizlikButon");
    //          buton3->setGeometry(120, 170, 160, 35);
    //
    // TODO: b) Her butona setObjectName() ile anlamli bir isim verin
    //
    // TODO: c) children() ile tum child nesneleri listeleyin ve
    //          her birinin sinif adini (metaObject()->className())
    //          yazdir
    //
    // TODO: d) Toplam child sayisini qDebug() ile yazdir
    // ============================================

    // === GOREV 2.2 - findChild ve findChildren (15 dk) ===
    // TODO: a) findChild<QLabel*>() ile baslik etiketini bulun ve
    //          metnini yazdir:
    //          auto* etiket = pencere->findChild<QLabel*>("baslikLabel");
    //          if (etiket) qDebug() << "Etiket:" << etiket->text();
    //
    // TODO: b) findChildren<QPushButton*>() ile tum butonlari bulun:
    //          auto butonlar = pencere->findChildren<QPushButton*>();
    //          qDebug() << "Buton sayisi:" << butonlar.size();
    //          for (auto* b : butonlar) {
    //              qDebug() << " Buton:" << b->text();
    //          }
    //
    // TODO: c) Ismi "cikisButon" olan butonu findChild ile bulun
    //          ve metnini "Sistemi Kapat" olarak degistirin:
    //          auto* cikis = pencere->findChild<QPushButton*>("cikisButon");
    //          if (cikis) cikis->setText("Sistemi Kapat");
    // ============================================

    return app.exec();
}

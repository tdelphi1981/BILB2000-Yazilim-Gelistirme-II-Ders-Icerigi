/**
 * main.cpp - OBYS Responsive UI girişi
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 4 - Qt ile Çok Kanallı Programlama
 *
 * Derleme:
 *   mkdir build && cd build
 *   cmake ..
 *   make
 *   ./obys_responsive
 */
#include <QApplication>
#include <QRandomGenerator>
#include "rapor_penceresi.h"

QList<Ogrenci> testVerisi(int n) {
    QList<Ogrenci> v;
    v.reserve(n);
    QStringList bolumler = {"Bilgisayar", "Makine", "Elektrik"};
    QStringList harfler = {"AA", "BA", "BB", "CB", "CC", "DC", "DD"};
    for (int i = 0; i < n; ++i) {
        Ogrenci o;
        o.numara = 100000 + i;
        o.ad = QString("Öğrenci %1").arg(i);
        o.bolum = bolumler[i % bolumler.size()];
        int ds = 20 + QRandomGenerator::global()->bounded(15);
        for (int d = 0; d < ds; ++d) {
            DersNotu n;
            n.dersKodu = QString("D%1").arg(d);
            n.kredi = 3;
            n.harf = harfler[
                QRandomGenerator::global()->bounded(harfler.size())];
            o.notlar.append(n);
        }
        v.append(o);
    }
    return v;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    RaporPenceresi pencere(testVerisi(10000));
    pencere.show();
    return app.exec();
}

/*
 * BEKLENEN DAVRANIŞ:
 * ------------------
 * 1. Pencere açılır. Tablo boş, "Rapor Hesapla" butonu aktif.
 * 2. Butona tıklanır; progress dialog 200 ms sonra açılır.
 * 3. Progress bar kesintisiz dolar (paralel çalışma).
 * 4. 4-5 saniye sonra (8-çekirdekli makine) tamamlanır.
 * 5. Tablo 10.000 satırla dolar.
 * 6. Hesaplama sırasında UI donmaz -- pencere hareket ettirilebilir.
 * 7. İptal tuşu basılırsa işlem durur.
 */

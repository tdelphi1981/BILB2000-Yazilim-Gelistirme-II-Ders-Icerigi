/**
 * JSON Komut/Cevap Protokolü — Stand-alone Test
 *
 * Bu uygulamada:
 *  - QJsonDocument, QJsonObject ile JSON kodlama/çözme
 *  - {"komut":"...","id":N,"veri":{...}} format taslağı
 *  - {"id":N,"durum":"ok|hata","veri":{...}} cevap formatı
 *  - Round-trip (kodla -> çöz) testi
 *  - Hatalı JSON ve eksik alan davranışı
 *
 * OBYS bağlamı: OBYS sunucusu ile istemci arasında konuşulan dil
 * JSON olacaktır. Bu modül, JSON'u byte dizisine çevirip ağ üzerinden
 * göndermeye hazır hale getirir; karşı tarafta da ters yönde çözer.
 *
 * Bölüm 12 Ünite 4 Konu 2
 *
 * Derleme:  mkdir build && cd build && cmake .. && cmake --build .
 * Çalıştır: ./json_komut_cevap
 */

#include "Protokol.h"

#include <QCoreApplication>
#include <QJsonObject>

#include <iostream>

int main(int argc, char* argv[]) {
    QCoreApplication uygulama(argc, argv);
    Q_UNUSED(uygulama);

    std::cout << "=== JSON Komut/Cevap Protokol Testi ===\n\n";

    // 1) Bir login komutu hazırla
    Komut girisKomutu;
    girisKomutu.komut = QStringLiteral("login");
    girisKomutu.id    = 1;
    girisKomutu.veri  = QJsonObject{
        {"kullanici", "ali.yilmaz"},
        {"sifre",     "gizli123"},
    };

    QByteArray kodlanmis = komutuKodla(girisKomutu);
    std::cout << "[1] Kodlanmış komut:\n  "
              << kodlanmis.toStdString() << "\n\n";

    // 2) Round-trip: kodla -> çöz, alanları doğrula
    auto cozulen = komutuCoz(kodlanmis);
    if (!cozulen) {
        std::cerr << "Çözme başarısız!\n";
        return 1;
    }
    std::cout << "[2] Çözülen komut alanları:\n";
    std::cout << "  komut    = " << cozulen->komut.toStdString() << "\n";
    std::cout << "  id       = " << cozulen->id << "\n";
    std::cout << "  veri.kullanici = "
              << cozulen->veri["kullanici"].toString().toStdString() << "\n\n";

    // 3) Bir başarılı cevap oluştur
    QJsonObject vY{
        {"oturum_no", "abc-123-xyz"},
        {"yetki",     "ogretmen"},
    };
    Cevap basariliCevap = okCevabi(1, vY);
    QByteArray cevapBayt = cevabiKodla(basariliCevap);
    std::cout << "[3] Kodlanmış başarılı cevap:\n  "
              << cevapBayt.toStdString() << "\n\n";

    // 4) Hatalı cevap
    Cevap hataC = hataCevabi(2, QStringLiteral("Şifre hatalı"));
    std::cout << "[4] Kodlanmış hatalı cevap:\n  "
              << cevabiKodla(hataC).toStdString() << "\n\n";

    // 5) Bozuk JSON çözme denemesi
    QByteArray bozuk = "{\"komut\":\"ping\", id:bozuk}";
    if (!komutuCoz(bozuk)) {
        std::cout << "[5] Bozuk JSON beklenildiği gibi çözülemedi (OK).\n\n";
    }

    // 6) Eksik alan denemesi
    QByteArray eksik = R"({"komut":"ping"})";
    if (!komutuCoz(eksik)) {
        std::cout << "[6] Eksik 'id' alanı reddedildi (OK).\n\n";
    }

    // 7) Türkçe karakter testi
    Komut trKomut;
    trKomut.komut = QStringLiteral("ogrenci_ekle");
    trKomut.id    = 42;
    trKomut.veri  = QJsonObject{
        {"ad",     QStringLiteral("Ayşe Çağlar")},
        {"bolum",  QStringLiteral("Bilgisayar Mühendisliği")},
    };
    QByteArray trKodlanmis = komutuKodla(trKomut);
    std::cout << "[7] Türkçe karakter testi:\n  "
              << trKodlanmis.toStdString() << "\n";
    auto trCozulen = komutuCoz(trKodlanmis);
    if (trCozulen) {
        std::cout << "  ad      = "
                  << trCozulen->veri["ad"].toString().toStdString() << "\n";
        std::cout << "  bölüm   = "
                  << trCozulen->veri["bolum"].toString().toStdString() << "\n";
    }

    std::cout << "\n=== TEST TAMAMLANDI ===\n";
    return 0;
}

/*
BEKLENEN CIKTI:
---------------
=== JSON Komut/Cevap Protokol Testi ===

[1] Kodlanmış komut:
  {"id":1,"komut":"login","veri":{"kullanici":"ali.yilmaz","sifre":"gizli123"}}

[2] Çözülen komut alanları:
  komut    = login
  id       = 1
  veri.kullanici = ali.yilmaz

[3] Kodlanmış başarılı cevap:
  {"durum":"ok","id":1,"veri":{"oturum_no":"abc-123-xyz","yetki":"ogretmen"}}

[4] Kodlanmış hatalı cevap:
  {"durum":"hata","hata":"Şifre hatalı","id":2}

[5] Bozuk JSON beklenildiği gibi çözülemedi (OK).

[6] Eksik 'id' alanı reddedildi (OK).

[7] Türkçe karakter testi:
  {"id":42,"komut":"ogrenci_ekle","veri":{"ad":"Ayşe Çağlar","bolum":"Bilgisayar Mühendisliği"}}
  ad      = Ayşe Çağlar
  bölüm   = Bilgisayar Mühendisliği

=== TEST TAMAMLANDI ===
*/

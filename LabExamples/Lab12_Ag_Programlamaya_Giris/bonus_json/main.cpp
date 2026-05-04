/**
 * Lab 12 - Bonus: JSON Komut Çözücü
 *
 * Hedef: gelen string mesaji JSON olarak parse et,
 *        {"komut": "...", "veri": "..."} formatini tani,
 *        komuta gore cevap olustur.
 *
 * Desteklenen komutlar:
 *   - "echo"  : veri'yi aynen geri dondur.
 *   - "ping"  : "pong" dondur.
 *   - "tarih" : sistem tarihini ISO formatinda dondur.
 *   - aksi    : {"hata":"bilinmeyen komut"} dondur.
 *
 * Bu kod ag tarafini icermez; sadece komut isleyici fonksiyonu
 * yazip 4 ornek mesajla test eder. Calisma 3'teki ChatSunucu icine
 * tasinmasi kolaydir.
 */

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QDateTime>
#include <QByteArray>
#include <QString>
#include <QDebug>

// ============================================================
// GOREV: Komut dispatch fonksiyonunu doldur.
//
// Adimlar:
//   1) QJsonDocument::fromJson(istek, &hata) ile parse et.
//      Parse hatasi varsa {"hata":"gecersiz JSON"} cevabi olustur.
//
//   2) doc.object() ile root nesnesine eris.
//      "komut" alani string degilse hata cevabi.
//
//   3) switch yerine if/else if zinciri (QString karsilastirmasi):
//      - "echo"  : cevap["veri"] = obj.value("veri").toString();
//      - "ping"  : cevap["veri"] = "pong";
//      - "tarih" : cevap["veri"] = QDateTime::currentDateTime()
//                                    .toString(Qt::ISODate);
//      - default : cevap["hata"] = "bilinmeyen komut";
//
//   4) QJsonObject'i QJsonDocument'e sar, toJson(QJsonDocument::Compact)
//      ile QByteArray dondur.
// ============================================================
QByteArray komutIsle(const QByteArray& istek)
{
    QJsonObject cevap;
    // GOREV: Bu govdeyi doldurun.

    return QJsonDocument(cevap).toJson(QJsonDocument::Compact);
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // 4 test mesajiyla denenir:
    QByteArray testler[] = {
        R"({"komut":"echo","veri":"merhaba OBYS"})",
        R"({"komut":"ping"})",
        R"({"komut":"tarih"})",
        R"({"komut":"silahlanmazu"})"
    };

    for (const QByteArray& istek : testler) {
        QByteArray cevap = komutIsle(istek);
        qInfo().noquote() << ">>>" << istek;
        qInfo().noquote() << "<<<" << cevap;
        qInfo() << "----";
    }

    return 0;
}

/**
 * QCryptographicHash ile Salt + SHA-256 Parola Özeti
 *
 * Bu Qt programı, kullanıcının komut satırından aldığı parolayı
 * rastgele bir salt ile birleştirip SHA-256 özetine dönüştürür ve
 * salt + hash çiftini ekrana basar. Salt'ın amacı, aynı parolanın
 * iki farklı kullanıcı için bile farklı özetler üretmesini sağlayarak
 * gökkuşağı tablosu (rainbow table) saldırılarını anlamlı kılmamaktır.
 *
 * OBYS bağlamında: Yeni öğrenci kaydı sırasında parolası asla düz
 * metin olarak değil, salt + SHA-256 ile saklanır.
 *
 * Öğrenilecek kavramlar:
 * - QCryptographicHash::hash(...)
 * - QRandomGenerator ile salt üretme
 * - QByteArray::toHex() ile gösterim
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 4 - Güvenlik ve Tasarım
 *
 * Derleme:
 *   mkdir build && cd build
 *   cmake .. && cmake --build .
 * Çalıştırma: ./qcryptographichash_ornek sifre123
 */

#include <QByteArray>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QString>
#include <QTextStream>

QByteArray rastgele_salt(int boyut = 16) {
    QByteArray salt;
    salt.resize(boyut);
    auto* rng = QRandomGenerator::system();
    for (int i = 0; i < boyut; ++i) {
        salt[i] = static_cast<char>(rng->bounded(256));
    }
    return salt;
}

QByteArray sifre_ozeti(const QByteArray& parola, const QByteArray& salt) {
    QByteArray birlesik;
    birlesik.append(salt);
    birlesik.append(parola);
    return QCryptographicHash::hash(birlesik, QCryptographicHash::Sha256);
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    QTextStream cout(stdout);

    if (app.arguments().size() < 2) {
        cout << "Kullanım: " << app.arguments().at(0) << " <parola>\n";
        return 1;
    }

    const QString parola = app.arguments().at(1);
    const QByteArray parola_baytlari = parola.toUtf8();

    const QByteArray salt = rastgele_salt();
    const QByteArray ozet = sifre_ozeti(parola_baytlari, salt);

    cout << "Parola      : " << parola << '\n';
    cout << "Salt (hex)  : " << salt.toHex() << '\n';
    cout << "SHA-256 (hex): " << ozet.toHex() << '\n';
    cout << "\nVeritabanına kaydedilecek alan:\n  "
         << salt.toHex() << ":" << ozet.toHex() << '\n';

    return 0;
}

/*
BEKLENEN ÇIKTI (örnek - salt rastgele):
---------------------------------------
Parola      : sifre123
Salt (hex)  : 9f3b2c0a4e1f6...
SHA-256 (hex): 7a2c1f4e9b8d...
*/

/**
 * OBYS İstemci - Konsol Tabanlı
 *
 * Bu istemci, Bölüm 13 mini projesinin sunucusuna QTcpSocket ile
 * bağlanır, önce LOGIN komutu gönderip oturum jetonu alır, sonra
 * OGRENCI_LISTE komutu ile öğrenci listesini ister ve gelen yanıtı
 * ekrana yazar.
 *
 * OBYS bağlamında: Bu, çok daha geniş bir öğrenci uygulamasının
 * minimal "bağlan ve sorgula" çekirdeğidir.
 *
 * Bölüm: 13 - OBYS Client/Server Mimarisi
 * Ünite: 4 - Güvenlik ve Tasarım
 *
 * Derleme:
 *   mkdir build && cd build
 *   cmake .. && cmake --build .
 * Çalıştırma: ./client/obys_istemci 127.0.0.1 5560 ogr001 sifre123
 */

#include <QByteArray>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QString>
#include <QTcpSocket>
#include <QTextStream>
#include <QTimer>

namespace {

QString sha256Hex(const QString& metin) {
    const QByteArray h = QCryptographicHash::hash(
        metin.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromLatin1(h.toHex());
}

QString jsonAlanOku(const QString& metin, const QString& anahtar) {
    const QString desen = "\"" + anahtar + "\":\"";
    const int konum = metin.indexOf(desen);
    if (konum < 0) return {};
    const int basla = konum + desen.size();
    const int son = metin.indexOf('"', basla);
    if (son < 0) return {};
    return metin.mid(basla, son - basla);
}

bool gonderVeBekle(QTcpSocket& sok, const QString& istek, QString& yanit) {
    const QByteArray paket = (istek + "\n").toUtf8();
    sok.write(paket);
    sok.flush();

    if (!sok.waitForReadyRead(3000)) return false;
    while (!sok.canReadLine()) {
        if (!sok.waitForReadyRead(1000)) return false;
    }
    yanit = QString::fromUtf8(sok.readLine()).trimmed();
    return true;
}

} // namespace

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    QTextStream cout(stdout);

    const QString host    = (argc > 1) ? argv[1] : "127.0.0.1";
    const quint16 port    = (argc > 2) ? QString(argv[2]).toUShort() : 5560;
    const QString kul     = (argc > 3) ? argv[3] : "ogr001";
    const QString parola  = (argc > 4) ? argv[4] : "sifre123";

    QTcpSocket sok;
    cout << "[İstemci] " << host << ":" << port << " bağlanılıyor...\n";
    sok.connectToHost(host, port);
    if (!sok.waitForConnected(3000)) {
        cout << "[İstemci] Bağlantı kurulamadı: "
             << sok.errorString() << '\n';
        return 1;
    }
    cout << "[İstemci] Bağlandı.\n";

    // 1) LOGIN
    const QString hash = sha256Hex(parola);
    const QString login =
        QString(R"({"komut":"LOGIN","kullanici":"%1","parola_hash":"%2"})")
        .arg(kul, hash);

    QString yanit;
    if (!gonderVeBekle(sok, login, yanit)) {
        cout << "[İstemci] LOGIN gönderilemedi.\n";
        return 1;
    }
    cout << "[İstemci] LOGIN cevabı: " << yanit << '\n';

    const QString token = jsonAlanOku(yanit, "token");
    if (token.isEmpty()) {
        cout << "[İstemci] Kimlik doğrulanamadı.\n";
        return 1;
    }

    // 2) OGRENCI_LISTE
    const QString liste =
        QString(R"({"komut":"OGRENCI_LISTE","token":"%1"})").arg(token);
    if (!gonderVeBekle(sok, liste, yanit)) {
        cout << "[İstemci] OGRENCI_LISTE gönderilemedi.\n";
        return 1;
    }
    cout << "[İstemci] LİSTE cevabı:\n  " << yanit << '\n';

    sok.disconnectFromHost();
    if (sok.state() != QAbstractSocket::UnconnectedState) {
        sok.waitForDisconnected(1000);
    }
    return 0;
}

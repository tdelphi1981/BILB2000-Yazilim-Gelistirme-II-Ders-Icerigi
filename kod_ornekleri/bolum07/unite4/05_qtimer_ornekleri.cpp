/**
 * QTimer ile Zamanlayıcı Örnekleri
 *
 * Bu program QTimer ile periyodik zamanlayıcı (otomatik kaydetme),
 * tek seferlik zamanlayıcı (durum mesajı temizleme) ve
 * durum çubuğu saati oluşturmayı gösterir.
 *
 * Öğrenilecek kavramlar:
 * - QTimer::start() ile periyodik zamanlayıcı
 * - QTimer::singleShot() ile tek seferlik gecikme
 * - Durum çubuğunda canlı saat
 * - Otomatik kaydetme mekanizması
 *
 * Bölüm: 07 - Qt İleri Bileşenler
 * Ünite: 4 - Olay İşleme ve Zamanlayıcılar
 *
 * Derleme: CMake ile (Qt6 gerekli)
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class TimerOrnegi : public QMainWindow {
    Q_OBJECT
public:
    explicit TimerOrnegi(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - Timer Örneği");
        resize(600, 400);

        // Merkezi widget
        auto* merkez = new QWidget(this);
        auto* duzen = new QVBoxLayout(merkez);

        auto* baslik = new QLabel("QTimer Örnekleri", merkez);
        baslik->setAlignment(Qt::AlignCenter);
        baslik->setStyleSheet("font-size: 18px; font-weight: bold;");
        duzen->addWidget(baslik);

        // Sayac
        m_sayacLabel = new QLabel("Sayaç: 0", merkez);
        m_sayacLabel->setAlignment(Qt::AlignCenter);
        m_sayacLabel->setStyleSheet("font-size: 24px; color: #008080;");
        duzen->addWidget(m_sayacLabel);

        // Butonlar
        auto* baslatButon = new QPushButton("Sayacı Başlat (1 sn)", merkez);
        auto* durdurButon = new QPushButton("Sayacı Durdur", merkez);
        auto* tekButon = new QPushButton("3 sn Sonra Mesaj (singleShot)", merkez);

        duzen->addWidget(baslatButon);
        duzen->addWidget(durdurButon);
        duzen->addWidget(tekButon);
        duzen->addStretch();

        setCentralWidget(merkez);

        // Periyodik timer
        m_sayacTimer = new QTimer(this);
        connect(m_sayacTimer, &QTimer::timeout, this, [this]() {
            m_sayac++;
            m_sayacLabel->setText(
                QString("Sayaç: %1").arg(m_sayac));
        });

        connect(baslatButon, &QPushButton::clicked, this, [this]() {
            m_sayacTimer->start(1000);
            statusBar()->showMessage("Sayaç başlatıldı");
        });

        connect(durdurButon, &QPushButton::clicked, this, [this]() {
            m_sayacTimer->stop();
            statusBar()->showMessage("Sayaç durduruldu");
        });

        connect(tekButon, &QPushButton::clicked, this, [this]() {
            statusBar()->showMessage("3 saniye bekleniyor...");
            QTimer::singleShot(3000, this, [this]() {
                statusBar()->showMessage(
                    "Tek seferlik mesaj gösterildi!", 2000);
            });
        });

        // Durum cubugu saati
        saatKur();
    }

private:
    QTimer* m_sayacTimer = nullptr;
    QLabel* m_sayacLabel = nullptr;
    QLabel* m_saatLabel = nullptr;
    int m_sayac = 0;

    void saatKur() {
        m_saatLabel = new QLabel(this);
        statusBar()->addPermanentWidget(m_saatLabel);

        auto* saatTimer = new QTimer(this);
        connect(saatTimer, &QTimer::timeout, this, [this]() {
            m_saatLabel->setText(
                QTime::currentTime().toString("HH:mm:ss"));
        });
        saatTimer->start(1000);
        m_saatLabel->setText(
            QTime::currentTime().toString("HH:mm:ss"));
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    TimerOrnegi pencere;
    pencere.show();
    return app.exec();
}

#include "05_qtimer_ornekleri.moc"

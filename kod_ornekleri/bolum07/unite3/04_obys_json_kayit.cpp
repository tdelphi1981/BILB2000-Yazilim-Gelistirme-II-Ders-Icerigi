/**
 * OBYS JSON Kayıt/Yükleme Sistemi
 *
 * Bu program QJsonDocument ile öğrenci verilerini JSON formatında
 * dosyaya kaydetme ve dosyadan yükleme işlemlerini gösterir.
 * Son açılan dosya listesi de QSettings ile saklanır.
 *
 * Öğrenilecek kavramlar:
 * - QJsonDocument, QJsonObject, QJsonArray kullanımı
 * - JSON dosyaya yazma ve okuma
 * - QFileDialog ile dosya kaydetme/açma
 * - Son açılan dosyalar menüsü
 *
 * Bölüm: 07 - Qt İleri Bileşenler
 * Ünite: 3 - Ayarlar ve Veri Kalıcılığı
 *
 * Derleme: CMake ile (Qt6 gerekli)
 */

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class JsonOBYS : public QMainWindow {
    Q_OBJECT
public:
    explicit JsonOBYS(QWidget* parent = nullptr)
        : QMainWindow(parent) {
        setWindowTitle("OBYS - JSON Kayıt Örneği");
        resize(800, 500);

        tabloOlustur();
        menuOlustur();
        statusBar()->showMessage("Hazır");
    }

private:
    QStandardItemModel* m_model = nullptr;
    QString m_mevcutDosya;
    QMenu* m_sonDosyaMenu = nullptr;

    void tabloOlustur() {
        m_model = new QStandardItemModel(0, 4, this);
        m_model->setHorizontalHeaderLabels(
            {"Numara", "Ad Soyad", "Bölüm", "GPA"});

        // Ornek veriler ekle
        ogrenciEkle("2024001", "Ali Yılmaz", "Bilg. Bil.", "3.45");
        ogrenciEkle("2024002", "Ayşe Kaya", "Yaz. Müh.", "3.82");

        auto* tablo = new QTableView(this);
        tablo->setModel(m_model);
        tablo->setAlternatingRowColors(true);
        tablo->horizontalHeader()->setStretchLastSection(true);
        setCentralWidget(tablo);
    }

    void ogrenciEkle(const QString& no, const QString& ad,
                     const QString& bolum, const QString& gpa) {
        int satir = m_model->rowCount();
        m_model->insertRow(satir);
        m_model->setItem(satir, 0, new QStandardItem(no));
        m_model->setItem(satir, 1, new QStandardItem(ad));
        m_model->setItem(satir, 2, new QStandardItem(bolum));
        m_model->setItem(satir, 3, new QStandardItem(gpa));
    }

    void menuOlustur() {
        auto* dosyaMenu = menuBar()->addMenu("Dosya");

        auto* kaydetAct = dosyaMenu->addAction("Kaydet...");
        kaydetAct->setShortcut(QKeySequence::Save);
        connect(kaydetAct, &QAction::triggered, this, &JsonOBYS::dosyaKaydet);

        auto* acAct = dosyaMenu->addAction("Aç...");
        acAct->setShortcut(QKeySequence::Open);
        connect(acAct, &QAction::triggered, this, &JsonOBYS::dosyaAc);

        dosyaMenu->addSeparator();
        m_sonDosyaMenu = dosyaMenu->addMenu("Son Açılanlar");
        sonDosyaMenusuGuncelle();

        dosyaMenu->addSeparator();
        auto* cikisAct = dosyaMenu->addAction("Çıkış");
        connect(cikisAct, &QAction::triggered, this, &QWidget::close);
    }

    void dosyaKaydet() {
        QString yol = QFileDialog::getSaveFileName(this,
            "Öğrenci Verilerini Kaydet", "",
            "JSON Dosyaları (*.json)");
        if (yol.isEmpty()) return;

        QJsonObject kok;
        kok["versiyon"] = 1;
        kok["tarih"] = QDateTime::currentDateTime().toString(Qt::ISODate);

        QJsonArray dizi;
        for (int i = 0; i < m_model->rowCount(); ++i) {
            QJsonObject ogr;
            ogr["numara"] = m_model->item(i, 0)->text();
            ogr["ad"]     = m_model->item(i, 1)->text();
            ogr["bolum"]  = m_model->item(i, 2)->text();
            ogr["gpa"]    = m_model->item(i, 3)->text().toDouble();
            dizi.append(ogr);
        }
        kok["ogrenciler"] = dizi;

        QFile dosya(yol);
        if (dosya.open(QIODevice::WriteOnly)) {
            dosya.write(QJsonDocument(kok).toJson(QJsonDocument::Indented));
            m_mevcutDosya = yol;
            sonDosyaEkle(yol);
            statusBar()->showMessage("Kaydedildi: " + yol, 3000);
        }
    }

    void dosyaAc() {
        QString yol = QFileDialog::getOpenFileName(this,
            "Öğrenci Verilerini Aç", "",
            "JSON Dosyaları (*.json)");
        if (yol.isEmpty()) return;
        dosyaYukle(yol);
    }

    void dosyaYukle(const QString& yol) {
        QFile dosya(yol);
        if (!dosya.open(QIODevice::ReadOnly)) return;

        QJsonDocument doc = QJsonDocument::fromJson(dosya.readAll());
        QJsonArray dizi = doc.object()["ogrenciler"].toArray();

        m_model->removeRows(0, m_model->rowCount());
        for (const auto& val : dizi) {
            QJsonObject obj = val.toObject();
            ogrenciEkle(
                obj["numara"].toString(),
                obj["ad"].toString(),
                obj["bolum"].toString(),
                QString::number(obj["gpa"].toDouble(), 'f', 2));
        }

        m_mevcutDosya = yol;
        sonDosyaEkle(yol);
        statusBar()->showMessage(
            QString("%1 öğrenci yüklendi").arg(dizi.size()), 3000);
    }

    void sonDosyaEkle(const QString& yol) {
        QSettings ayarlar;
        QStringList dosyalar = ayarlar.value("sonDosyalar").toStringList();
        dosyalar.removeAll(yol);
        dosyalar.prepend(yol);
        while (dosyalar.size() > 5)
            dosyalar.removeLast();
        ayarlar.setValue("sonDosyalar", dosyalar);
        sonDosyaMenusuGuncelle();
    }

    void sonDosyaMenusuGuncelle() {
        m_sonDosyaMenu->clear();
        QSettings ayarlar;
        for (const auto& yol : ayarlar.value("sonDosyalar").toStringList()) {
            if (!QFileInfo::exists(yol)) continue;
            auto* act = m_sonDosyaMenu->addAction(QFileInfo(yol).fileName());
            connect(act, &QAction::triggered, this,
                    [this, yol]() { dosyaYukle(yol); });
        }
        m_sonDosyaMenu->setEnabled(!m_sonDosyaMenu->isEmpty());
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("KTU");
    app.setApplicationName("OBYS_JSON");

    JsonOBYS pencere;
    pencere.show();
    return app.exec();
}

#include "04_obys_json_kayit.moc"

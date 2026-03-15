/**
 * OBYS Ana Pencere - Sınıf Tanımı (Başlık Dosyası)
 *
 * Bu dosya, OBYS uygulamasının ana pencere sınıfını tanımlar.
 * QMainWindow'dan türetilen ObysAnaPencere sınıfı, menü çubuğu,
 * araç çubuğu, durum çubuğu, dock widget ve merkezi widget
 * bileşenlerini içerir. Bölüm 5'in tüm konularını bir araya
 * getiren kapsamlı bir örnektir.
 *
 * Öğrenilecek kavramlar:
 * - QMainWindow alt sınıfı tasarımı
 * - Q_OBJECT makrosu ve Meta-Object System
 * - Private slot ve yardımcı metot ayrımı
 * - Header/Source dosya ayrımı
 *
 * Bölüm: 05 - Qt Pencere ve Düzen Yönetimi
 * Ünite: 4 - Qt Designer ve UI Dosyaları
 *
 * Derleme: CMakeLists.txt dosyasına bakınız.
 */

#ifndef OBYS_ANA_PENCERE_H
#define OBYS_ANA_PENCERE_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>

class ObysAnaPencere : public QMainWindow {
    Q_OBJECT

public:
    explicit ObysAnaPencere(QWidget* parent = nullptr);
    ~ObysAnaPencere() = default;

private slots:
    // -- OBYS İşlem Slotları --
    void yeniOgrenci();
    void ogrenciListele();
    void dersEkle();
    void dersListele();
    void hakkinda();

private:
    // -- Arayüz Oluşturma Yardımcıları --
    void menuOlustur();
    void aracCubuguOlustur();
    void durumCubuguOlustur();
    void dockOlustur();
    void merkeziWidgetOlustur();

    // -- Menü ve Eylemler --
    QMenu* dosyaMenu_;
    QMenu* ogrenciMenu_;
    QMenu* dersMenu_;
    QMenu* yardimMenu_;

    QAction* yeniEylem_;
    QAction* acEylem_;
    QAction* kaydetEylem_;
    QAction* cikisEylem_;
    QAction* ogrenciEkleEylem_;
    QAction* ogrenciListeEylem_;
    QAction* dersEkleEylem_;
    QAction* dersListeEylem_;
    QAction* hakkindaEylem_;

    // -- Araç Çubuğu --
    QToolBar* anaAracCubugu_;

    // -- Dock Widget --
    QDockWidget* gezinmeDock_;
    QListWidget* gezinmeListe_;

    // -- Merkezi Widget Bileşenleri --
    QLineEdit* isimGiris_;
    QSpinBox* numaraGiris_;
    QComboBox* bolumSecim_;
    QPushButton* kaydetButon_;
    QPushButton* temizleButon_;
    QLabel* durumEtiket_;

    // -- Durum Çubuğu --
    QLabel* kayitSayisiEtiket_;
    int kayitSayaci_ = 0;
};

#endif // OBYS_ANA_PENCERE_H

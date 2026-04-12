/**
 * main_window.h -- OBYS v1 ana pencere basligi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * Basit bir QMainWindow turevi. Dosya menusunden Yeni/Ac/Kaydet/
 * Farkli Kaydet/Cikis islemleri yapilabilir. "Not Ekle" butonu
 * NotGirisDialog'u acar. Baslik cubugunda aktif dosya adi ve
 * kirlilik durumu (*) gosterilir. Kapanisa yakin kullaniciya
 * kaydetme sorulur.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Widgets ile birlikte.
 */

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "obys_servis.h"
#include "persistence_manager.h"

#include <QMainWindow>
#include <QString>

class QAction;
class QCloseEvent;
class QListWidget;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

protected:
    void closeEvent(QCloseEvent* olay) override;

private slots:
    void dosyaYeni();
    void dosyaAc();
    bool dosyaKaydet();
    bool dosyaFarkliKaydet();
    void notEkleDialogAc();

private:
    void menuleriKur();
    void merkeziWidgetKur();
    void basligiGuncelle();
    void notListesiniGuncelle();
    bool kaydetmeSorusu();  // kapanmadan once

    void dirtyYap(bool d = true);

    OBYSServis         m_servis;
    PersistenceManager m_persist;

    QString m_aktifDosya;
    bool    m_dirty{false};

    // Merkezi alan
    QListWidget* m_notListesi{nullptr};
    QPushButton* m_notEkleButonu{nullptr};

    // Menu aksiyonlari
    QAction* m_yeniAction{nullptr};
    QAction* m_acAction{nullptr};
    QAction* m_kaydetAction{nullptr};
    QAction* m_farkliKaydetAction{nullptr};
    QAction* m_cikisAction{nullptr};
};

#endif // MAIN_WINDOW_H

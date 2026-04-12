/**
 * main_window.h -- OBYS v2 ana pencere basligi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 *
 * Unite 1'deki MainWindow'un uzerine iki yeni aksiyon eklenmis
 * halidir:
 *   [Transkript Yazdir...] -> Secilen ogrencinin transkriptini
 *                             onizleme/yazdir/PDF secenekleri ile
 *                             QPrinter'a yonlendirir.
 *   [Toplu Rapor...]       -> Secilen ders icin cok sayfali
 *                             DersRaporu belgesini yazdirir.
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
class QTextDocument;

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

    // Unite 2 -- Yeni aksiyonlar
    void transkriptYazdir();
    void topluRaporYazdir();

private:
    void menuleriKur();
    void merkeziWidgetKur();
    void basligiGuncelle();
    void notListesiniGuncelle();
    bool kaydetmeSorusu();
    void dirtyYap(bool d = true);

    // Belgeyi Yazdir/PDF/Onizleme akisina sokan yardimci
    void belgeyiYazdirmaAkisina(QTextDocument* belge,
                                const QString& varsayilanPdfAd);

    OBYSServis         m_servis;
    PersistenceManager m_persist;

    QString m_aktifDosya;
    bool    m_dirty{false};

    QListWidget* m_notListesi{nullptr};
    QPushButton* m_notEkleButonu{nullptr};

    QAction* m_yeniAction{nullptr};
    QAction* m_acAction{nullptr};
    QAction* m_kaydetAction{nullptr};
    QAction* m_farkliKaydetAction{nullptr};
    QAction* m_cikisAction{nullptr};

    // Unite 2 -- Yeni aksiyonlar
    QAction* m_transkriptAction{nullptr};
    QAction* m_topluRaporAction{nullptr};
};

#endif // MAIN_WINDOW_H

/**
 * main_window.h -- OBYS v4 ana pencere basligi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 4 - OBYS GUI Mini Proje Tamamlama
 *
 * Unite 3'teki MainWindow uzerine mini proje tamamlama kapsaminda
 * sunlar eklenmistir:
 *   - "Dosya > Disa Aktar > JSON" aksiyonu (JsonExporter kullanir)
 *   - "Dosya > Son Acilanlar" alt menusu (QSettings tabanli)
 *   - "Gorunum > Aydinlik Tema / Karanlik Tema" aksiyonlari
 *   - "Yardim > Hakkinda..." diyalogu
 *   - Tum menu ogeleri icin klavye kisayollari
 *   - closeEvent'te pencere durumu ve geometrisinin QSettings'e
 *     kaydedilmesi; yapilandirici icinde geri yuklenmesi
 */

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "obys_servis.h"
#include "persistence_manager.h"

#include <QMainWindow>
#include <QString>
#include <QStringList>

class QAction;
class QCloseEvent;
class QMenu;
class QTabWidget;
class QTableView;
class QPushButton;
class QTextDocument;

class NotlarModel;
class IstatistikModel;
class GrafikPaneli;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

protected:
    void closeEvent(QCloseEvent* olay) override;

private slots:
    // Dosya menusu
    void dosyaYeni();
    void dosyaAc();
    bool dosyaKaydet();
    bool dosyaFarkliKaydet();
    void disaAktarJson();

    // Notlar
    void notEkleDialogAc();

    // Raporlar
    void transkriptYazdir();
    void topluRaporYazdir();

    // Gorunum
    void temayiUygula(const QString& temaAd);

    // Yardim
    void hakkindaGoster();

private:
    void menuleriKur();
    void merkeziWidgetKur();
    QWidget* notlarSekmesiKur();
    QWidget* trendSekmesiKur();
    void basligiGuncelle();
    bool kaydetmeSorusu();
    void dirtyYap(bool d = true);

    void belgeyiYazdirmaAkisina(QTextDocument* belge,
                                const QString& varsayilanPdfAd);

    // QSettings / pencere durumu
    void ayarlariYukle();
    void ayarlariKaydet();

    // Son acilanlar (QSettings'e dayali)
    void sonAcilanlariYukle();
    void sonAcilanlariGuncelle(const QString& yol);
    void sonAcilanlarMenusunuYenile();

    void dosyaYoluAc(const QString& yol);

    OBYSServis         m_servis;
    PersistenceManager m_persist;

    QString m_aktifDosya;
    bool    m_dirty{false};

    QTabWidget*      m_sekmeler{nullptr};
    QTableView*      m_notTablosu{nullptr};
    NotlarModel*     m_notlarModel{nullptr};
    IstatistikModel* m_istatistikModel{nullptr};
    GrafikPaneli*    m_grafikPanel{nullptr};
    QPushButton*     m_notEkleButonu{nullptr};

    // Dosya menusu aksiyonlari
    QAction* m_yeniAction{nullptr};
    QAction* m_acAction{nullptr};
    QAction* m_kaydetAction{nullptr};
    QAction* m_farkliKaydetAction{nullptr};
    QAction* m_disaAktarJsonAction{nullptr};
    QAction* m_cikisAction{nullptr};

    // Raporlar
    QAction* m_transkriptAction{nullptr};
    QAction* m_topluRaporAction{nullptr};

    // Gorunum / tema
    QAction* m_aydinlikTemaAction{nullptr};
    QAction* m_karanlikTemaAction{nullptr};

    // Yardim
    QAction* m_hakkindaAction{nullptr};

    // Son acilanlar alt menusu
    QMenu*        m_sonAcilanlarMenu{nullptr};
    QStringList   m_sonAcilanlar;
    static constexpr int kMaxSonAcilan = 5;
};

#endif // MAIN_WINDOW_H

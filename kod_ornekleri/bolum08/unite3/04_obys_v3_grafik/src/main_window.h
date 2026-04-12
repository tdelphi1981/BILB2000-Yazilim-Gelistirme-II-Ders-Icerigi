/**
 * main_window.h -- OBYS v3 ana pencere basligi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 3 - Grafik ve Istatistik
 *
 * Unite 2'deki MainWindow'un uzerine QTabWidget ile uc sekme eklenmis
 * halidir:
 *   [Notlar]     -> NotlarModel gosteren QTableView (Unite 2 ile
 *                    ayni semantik)
 *   [Grafikler]  -> IstatistikModel baglanmis GrafikPaneli (bar chart)
 *   [Trend]      -> Sahte donem verisiyle uretilmis QLineSeries
 *                    tabanli trend grafigi
 *
 * Not eklendiginde NotlarModel::yenile() cagrilir; IstatistikModel
 * bu modeli dinledigi icin bar chart otomatik olarak guncellenir.
 */

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "obys_servis.h"
#include "persistence_manager.h"

#include <QMainWindow>
#include <QString>

class QAction;
class QCloseEvent;
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
    void dosyaYeni();
    void dosyaAc();
    bool dosyaKaydet();
    bool dosyaFarkliKaydet();
    void notEkleDialogAc();
    void transkriptYazdir();
    void topluRaporYazdir();

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

    QAction* m_yeniAction{nullptr};
    QAction* m_acAction{nullptr};
    QAction* m_kaydetAction{nullptr};
    QAction* m_farkliKaydetAction{nullptr};
    QAction* m_cikisAction{nullptr};
    QAction* m_transkriptAction{nullptr};
    QAction* m_topluRaporAction{nullptr};
};

#endif // MAIN_WINDOW_H

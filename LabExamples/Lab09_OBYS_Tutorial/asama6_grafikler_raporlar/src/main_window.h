#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "obys_servis.h"
#include "persistence_manager.h"

#include <QMainWindow>
#include <QStringList>

class QAction;
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

protected:
    void closeEvent(QCloseEvent* olay) override;

private slots:
    void dosyaYeni();
    void dosyaAc();
    bool dosyaKaydet();
    bool dosyaFarkliKaydet();

    void ogrenciEkleDialogAc();
    void dersEkleDialogAc();
    void notEkleDialogAc();

    void disaAktarJson();
    void ayarlariGoster();
    void transkriptYazdir();

private:
    void menuleriKur();
    void merkeziWidgetKur();
    QWidget* notlarSekmesiKur();

    void basligiGuncelle();
    bool kaydetmeSorusu();
    void dirtyYap(bool d = true);

    void ayarlariYukle();
    void ayarlariKaydet();

    // Son acilanlar
    void sonAcilanlariYukle();
    void sonAcilanlariGuncelle(const QString& yol);
    void sonAcilanlarMenusunuYenile();
    void dosyaYoluAc(const QString& yol);

    OBYSServis         m_servis;
    PersistenceManager m_persist;

    QString     m_aktifDosya;
    bool        m_dirty{false};

    QTabWidget*  m_sekmeler{nullptr};
    QTableView*  m_notTablosu{nullptr};
    NotlarModel* m_notlarModel{nullptr};
    QPushButton* m_notEkleButonu{nullptr};

    IstatistikModel* m_istatistikModel{nullptr};
    GrafikPaneli*    m_grafikPanel{nullptr};

    // Menuler
    QMenu*      m_sonAcilanlarMenu{nullptr};
    QStringList m_sonAcilanlar;
    static constexpr int kMaxSonAcilan = 5;
};

#endif // MAIN_WINDOW_H

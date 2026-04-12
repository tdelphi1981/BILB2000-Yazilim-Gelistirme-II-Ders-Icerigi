/**
 * not_giris_dialog.h -- NotGirisDialog sinif basligi (proje surumu)
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * 01 numarali ornekten bu projeye kopyalanmistir. Obys_domain.h
 * tarafindan saglanan NotDTO ve NotTuru tiplerini kullanir.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Widgets ile birlikte.
 */

#ifndef PROJ_NOT_GIRIS_DIALOG_H
#define PROJ_NOT_GIRIS_DIALOG_H

#include "obys_domain.h"

#include <QDialog>
#include <QList>
#include <QPair>
#include <QString>

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QDialogButtonBox;

class NotGirisDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotGirisDialog(QWidget* parent = nullptr);
    ~NotGirisDialog() override = default;

    NotDTO notDTO() const;

    void ogrencileriYukle(const QList<QPair<int, QString>>& ogrenciler);
    void dersleriYukle(const QList<QPair<int, QString>>& dersler);

private slots:
    void notDegistiginde(double yeniDeger);
    void notTuruDegistiginde(int indeks);

private:
    static QString sayisalDanHarfe(double puan);

    QComboBox*        m_ogrenciSecici{nullptr};
    QComboBox*        m_dersSecici{nullptr};
    QDoubleSpinBox*   m_notDegeri{nullptr};
    QComboBox*        m_notTuruSecici{nullptr};
    QLabel*           m_harfNotuOnizleme{nullptr};
    QDialogButtonBox* m_butonlar{nullptr};
};

#endif // PROJ_NOT_GIRIS_DIALOG_H

#ifndef NOT_GIRIS_DIALOG_H
#define NOT_GIRIS_DIALOG_H

#include "obys_domain.h"
#include <QDialog>
#include <QList>
#include <QPair>

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QDialogButtonBox;

class NotGirisDialog : public QDialog {
    Q_OBJECT
public:
    explicit NotGirisDialog(QWidget* parent = nullptr);

    NotDTO notDTO() const;

    void ogrencileriYukle(
        const QList<QPair<int, QString>>& liste);
    void dersleriYukle(
        const QList<QPair<int, QString>>& liste);

private slots:
    void notDegistiginde(double yeniDeger);
    void notTuruDegistiginde(int indeks);

private:
    static QString sayisalDanHarfe(double puan);

    QComboBox*        m_ogrenciSecici{nullptr};
    QComboBox*        m_dersSecici{nullptr};
    QDoubleSpinBox*   m_notDegeri{nullptr};
    QComboBox*        m_notTuruSecici{nullptr};
    QLabel*           m_harfOnizleme{nullptr};
    QDialogButtonBox* m_butonlar{nullptr};
};

#endif // NOT_GIRIS_DIALOG_H

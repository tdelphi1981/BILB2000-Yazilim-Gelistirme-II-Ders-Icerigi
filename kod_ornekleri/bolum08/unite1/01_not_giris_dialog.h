/**
 * 01_not_giris_dialog.h -- NotGirisDialog sinif basligi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * Bu dosya OBYS'ye ders notu girisi icin ozel bir QDialog
 * turetme ornegini gosterir. Kullanici ogrenci, ders ve not
 * bilgilerini formdan girer; dialog bir NotDTO uretir.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Widgets ile birlikte.
 */

#ifndef NOT_GIRIS_DIALOG_H
#define NOT_GIRIS_DIALOG_H

#include <QDialog>
#include <QString>
#include <variant>

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QDialogButtonBox;

// NotTuru variant: Sayisal (0-100), Harf ("AA".."FF"), Muaf
struct SayisalNot { double deger{0.0}; };
struct HarfNotu   { QString harf; };
struct MuafNot    { };

using NotTuru = std::variant<SayisalNot, HarfNotu, MuafNot>;

// Dialog'un urettigi veri transfer nesnesi
struct NotDTO {
    int     ogrenciId{-1};
    int     dersId{-1};
    NotTuru notu{SayisalNot{0.0}};
};

/**
 * NotGirisDialog
 *
 * OBYS icin ders notu girisi yapan modal dialog.
 * Kullanici ogrenciyi ve dersi secer, not turunu belirler ve
 * degerini girer. Sayisal notlarda anlik harf onizlemesi yapilir.
 */
class NotGirisDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotGirisDialog(QWidget* parent = nullptr);
    ~NotGirisDialog() override = default;

    // Formdan doldurulan DTO'yu doner. accept edildiginde cagrilmalidir.
    NotDTO notDTO() const;

    // Combobox'lari disaridan doldurmak icin (OBYSServis'ten gelir)
    void ogrencileriYukle(const QList<QPair<int, QString>>& ogrenciler);
    void dersleriYukle(const QList<QPair<int, QString>>& dersler);

private slots:
    void notDegistiginde(double yeniDeger);
    void notTuruDegistiginde(int indeks);

private:
    // Yardimci: sayisal notu harf karsiligina cevirir
    static QString sayisalDanHarfe(double puan);

    QComboBox*        m_ogrenciSecici{nullptr};
    QComboBox*        m_dersSecici{nullptr};
    QDoubleSpinBox*   m_notDegeri{nullptr};
    QComboBox*        m_notTuruSecici{nullptr};
    QLabel*           m_harfNotuOnizleme{nullptr};
    QDialogButtonBox* m_butonlar{nullptr};
};

#endif // NOT_GIRIS_DIALOG_H

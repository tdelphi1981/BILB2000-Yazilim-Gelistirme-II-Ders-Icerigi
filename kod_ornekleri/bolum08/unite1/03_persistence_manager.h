/**
 * 03_persistence_manager.h -- PersistenceManager sinif basligi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * OBYSServis icerigini .obys binary dosyasina QDataStream ile
 * kaydeder ve ayni formattaki dosyayi geri yukler. Dosya basinda
 * magic + version alani tasir; eski/bozuk dosyalarda savunmaci
 * hata donusu yapilir.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Core ile birlikte.
 */

#ifndef PERSISTENCE_MANAGER_H
#define PERSISTENCE_MANAGER_H

#include <QString>
#include <cstdint>

class OBYSServis;

/**
 * PersistenceManager
 *
 * Disarida durumu olmayan, stateless benzeri bir yardimci sinif.
 * Son hata mesajini uye olarak tutar, UI tarafindan okunur.
 */
class PersistenceManager {
public:
    // Dosya formati imzasi: "OBYS" ASCII degerleri
    static constexpr quint32 MAGIC   = 0x4F425953u; // 'O','B','Y','S'
    static constexpr quint32 VERSION = 1u;

    PersistenceManager() = default;
    ~PersistenceManager() = default;

    // Servisi dosyaya kaydeder. Basarida true doner.
    bool kaydet(const QString& dosyaYolu, OBYSServis& servis);

    // Dosyayi okur ve servisi doldurur. Basarida true doner.
    bool yukle(const QString& dosyaYolu, OBYSServis& servis);

    // Son basarisiz islemin hata mesaji
    QString sonHata() const { return m_sonHata; }

private:
    QString m_sonHata;
};

#endif // PERSISTENCE_MANAGER_H

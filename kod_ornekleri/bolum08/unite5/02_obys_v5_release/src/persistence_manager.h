/**
 * persistence_manager.h -- PersistenceManager (proje surumu)
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
 *
 * 03 numarali ornekten bu projeye kopyalanmistir. OBYSServis ile
 * birlikte calisir; dosya basliginda magic + version imzasi tasir.
 *
 * Derleme: CMakeLists.txt icerisinde Qt6::Core ile birlikte.
 */

#ifndef PROJ_PERSISTENCE_MANAGER_H
#define PROJ_PERSISTENCE_MANAGER_H

#include <QString>
#include <cstdint>

class OBYSServis;

class PersistenceManager {
public:
    static constexpr quint32 MAGIC   = 0x4F425953u; // 'O','B','Y','S'
    static constexpr quint32 VERSION = 1u;

    PersistenceManager() = default;
    ~PersistenceManager() = default;

    bool kaydet(const QString& dosyaYolu, OBYSServis& servis);
    bool yukle(const QString& dosyaYolu, OBYSServis& servis);

    QString sonHata() const { return m_sonHata; }

private:
    QString m_sonHata;
};

#endif // PROJ_PERSISTENCE_MANAGER_H

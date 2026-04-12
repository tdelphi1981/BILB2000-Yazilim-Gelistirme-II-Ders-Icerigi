#ifndef PERSISTENCE_MANAGER_H
#define PERSISTENCE_MANAGER_H

#include <QString>
#include <cstdint>

class OBYSServis;

class PersistenceManager {
public:
    // Dosya imzasi: 'O','B','Y','S' = 0x4F425953
    static constexpr quint32 MAGIC   = 0x4F425953u;
    static constexpr quint32 VERSION = 1u;

    PersistenceManager() = default;

    bool kaydet(const QString& dosyaYolu,
                OBYSServis& servis);
    bool yukle(const QString& dosyaYolu,
               OBYSServis& servis);

    QString sonHata() const { return m_sonHata; }

private:
    QString m_sonHata;
};

#endif // PERSISTENCE_MANAGER_H

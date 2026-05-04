#pragma once
#include <QString>
#include <QStringList>
#include <memory>

/**
 * Ogrenci - OBYS'nin temel veri sınıfı
 *
 * Bölüm 11 Ünite 4 mini proje
 *
 * Bu sınıf tüm ders boyunca evrildi:
 *  - Bölüm 1-3: STL tabanlı, basit POD
 *  - Bölüm 4-8: QString'leri kullanır (Qt entegre)
 *  - Bölüm 11: thread-safe paylaşıldığı için const referans alımı önemli
 */
class Ogrenci {
public:
    Ogrenci() = default;
    Ogrenci(int numara, QString ad, QString bolum, double gpa = 0.0)
        : numara_(numara), ad_(std::move(ad)),
          bolum_(std::move(bolum)), gpa_(gpa) {}

    int numara() const { return numara_; }
    const QString& ad() const { return ad_; }
    const QString& bolum() const { return bolum_; }
    double gpa() const { return gpa_; }

    void setGpa(double g) { gpa_ = g; }
    void setBolum(const QString& b) { bolum_ = b; }

    QString toCSV() const {
        return QString("%1,%2,%3,%4")
            .arg(numara_).arg(ad_).arg(bolum_)
            .arg(QString::number(gpa_, 'f', 2));
    }

    static std::shared_ptr<Ogrenci> parseCSV(const QString& satir) {
        auto parts = satir.split(',');
        if (parts.size() < 3) {
            throw std::runtime_error("Eksik alan");
        }
        bool ok = false;
        int n = parts[0].toInt(&ok);
        if (!ok) throw std::runtime_error("Numara parse hatasi");
        double gpa = (parts.size() >= 4) ? parts[3].toDouble() : 0.0;
        return std::make_shared<Ogrenci>(n, parts[1], parts[2], gpa);
    }

private:
    int numara_ = 0;
    QString ad_;
    QString bolum_;
    double gpa_ = 0.0;
};

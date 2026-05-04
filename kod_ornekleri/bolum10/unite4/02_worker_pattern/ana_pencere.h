/**
 * AnaPencere - Worker Pattern kullanan UI
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 4 - Qt ile Çok Kanallı Programlama
 */
#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include "hesaplama_controller.h"

class AnaPencere : public QMainWindow {
    Q_OBJECT
public:
    explicit AnaPencere(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        auto* merkez = new QWidget(this);
        auto* yerlesim = new QVBoxLayout(merkez);

        durumEtiketi_ = new QLabel("Hazır", this);
        yerlesim->addWidget(durumEtiketi_);

        ilerleme_ = new QProgressBar(this);
        ilerleme_->setRange(0, 100);
        yerlesim->addWidget(ilerleme_);

        baslatBtn_ = new QPushButton("Hesaplamayı Başlat", this);
        iptalBtn_ = new QPushButton("İptal", this);
        iptalBtn_->setEnabled(false);
        yerlesim->addWidget(baslatBtn_);
        yerlesim->addWidget(iptalBtn_);

        setCentralWidget(merkez);
        setWindowTitle("OBYS - Worker Pattern Örneği");
        resize(400, 200);

        controller_ = new HesaplamaController(this);

        connect(baslatBtn_, &QPushButton::clicked,
                this, &AnaPencere::baslat);
        connect(iptalBtn_, &QPushButton::clicked,
                controller_, &HesaplamaController::iptal);

        connect(controller_, &HesaplamaController::ilerlemeDegisti,
                this, [this](int y, int t) {
                    ilerleme_->setValue(100 * y / t);
                    durumEtiketi_->setText(
                        QString("İşleniyor: %1 / %2").arg(y).arg(t));
                });
        connect(controller_, &HesaplamaController::tumSonuclarHazir,
                this, [this] {
                    durumEtiketi_->setText("Tamamlandı!");
                    baslatBtn_->setEnabled(true);
                    iptalBtn_->setEnabled(false);
                });
        connect(controller_, &HesaplamaController::iptalEdildi,
                this, [this] {
                    durumEtiketi_->setText("İptal edildi");
                    baslatBtn_->setEnabled(true);
                    iptalBtn_->setEnabled(false);
                });
    }

private slots:
    void baslat() {
        baslatBtn_->setEnabled(false);
        iptalBtn_->setEnabled(true);
        durumEtiketi_->setText("Başlatıldı...");

        QVector<HesaplamaVerisi> veriler;
        for (int i = 0; i < 100; ++i) {
            veriler.append({i, static_cast<double>(i)});
        }
        controller_->hesaplamayiBaslat(veriler);
    }

private:
    HesaplamaController* controller_;
    QPushButton* baslatBtn_;
    QPushButton* iptalBtn_;
    QProgressBar* ilerleme_;
    QLabel* durumEtiketi_;
};

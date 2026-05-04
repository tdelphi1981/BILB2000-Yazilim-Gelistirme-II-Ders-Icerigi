/**
 * QThread -- Basit Alt-Sınıflama Örneği
 *
 * Bu program, QThread'in alt-sınıflama yaklaşımıyla nasıl kullanıldığını
 * gösterir. UNUTMA: Bu desen Qt dokümantasyonunda ARTIK ÖNERİLMEZ.
 * Yalnızca karşılaştırma amaçlı buradadır; gerçek projede Worker Pattern
 * (moveToThread) kullanın -- bkz. 02_worker_pattern/
 *
 * Öğrenilecek kavramlar:
 * - QThread::run() fonksiyonunu ezme
 * - finished sinyali
 * - Alt-sınıflama yaklaşımının sınırları
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 4 - Qt ile Çok Kanallı Programlama
 *
 * Derleme (CMake önerilir):
 *   qmake -project
 *   qmake && make
 * Ya da doğrudan:
 *   g++ -std=c++20 $(pkg-config --cflags --libs Qt6Core) \
 *       -fPIC -o 01_qthread 01_qthread_basit.cpp
 */

#include <QThread>
#include <QCoreApplication>
#include <QDebug>

class BasitIs : public QThread {
public:
    explicit BasitIs(int tekrar, QObject* parent = nullptr)
        : QThread(parent), tekrar_(tekrar) {}

protected:
    void run() override {
        qDebug() << "run() başladı, izlek:" << currentThreadId();
        for (int i = 0; i < tekrar_; ++i) {
            qDebug() << "  Adım" << i;
            QThread::msleep(300);
        }
        qDebug() << "run() bitti";
    }

private:
    int tekrar_;
};

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "Ana izlek:" << QThread::currentThreadId();

    BasitIs is(5);
    QObject::connect(&is, &QThread::finished,
                     &app, &QCoreApplication::quit);

    is.start();
    return app.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * Ana izlek: 0x...
 * run() başladı, izlek: 0x... (farklı)
 *   Adım 0
 *   Adım 1
 *   ...
 *   Adım 4
 * run() bitti
 *
 * NOT: Bu desen slot/sinyal entegrasyonu için uygun değildir.
 * İzlek içinde slot çalıştırmak istiyorsanız moveToThread kullanın.
 */

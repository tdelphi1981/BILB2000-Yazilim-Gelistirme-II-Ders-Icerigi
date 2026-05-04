/**
 * QtConcurrent::run -- Tek Satırlık Asenkron Çalıştırma
 *
 * Bu program, QtConcurrent::run ile bir fonksiyonu arka plana
 * gönderip QFuture ile sonucunu alma örüntüsünü gösterir.
 *
 * Öğrenilecek kavramlar:
 * - QtConcurrent::run kullanımı
 * - QFuture<T> ile sonuç alma
 * - QFutureWatcher ile sinyal tabanlı izleme
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 4 - Qt ile Çok Kanallı Programlama
 *
 * Derleme (Qt 6 + CMake):
 *   CMakeLists.txt içinde: target_link_libraries(... Qt6::Core Qt6::Concurrent)
 *
 * Ya da pkg-config ile:
 *   g++ -std=c++20 -fPIC \
 *       $(pkg-config --cflags --libs Qt6Core Qt6Concurrent) \
 *       -o 03_run 03_qtconcurrent_run.cpp
 */

#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QCoreApplication>
#include <QDebug>
#include <QThread>

int agirHesaplama(int girdi) {
    QThread::sleep(2);
    return girdi * girdi;
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "İş başlatılıyor... (ana izlek ID:"
             << QThread::currentThreadId() << ")";

    QFuture<int> gelecek = QtConcurrent::run(agirHesaplama, 10);

    // Sonucu izlemek için watcher
    QFutureWatcher<int> izleyici;
    QObject::connect(&izleyici, &QFutureWatcher<int>::finished,
                     &app, [&]() {
        int sonuc = izleyici.result();
        qDebug() << "Sonuç hazır:" << sonuc;
        qDebug() << "(Şu an ana izlek:"
                 << QThread::currentThreadId() << ")";
        app.quit();
    });

    izleyici.setFuture(gelecek);
    qDebug() << "Ana izlek event loop'a giriyor...";
    return app.exec();
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * İş başlatılıyor... (ana izlek ID: 0x...)
 * Ana izlek event loop'a giriyor...
 * (2 saniye sonra)
 * Sonuç hazır: 100
 * (Şu an ana izlek: 0x...)
 *
 * NOT: finished sinyali ana izlekte işlenir, watcher'ın
 * thread affinity'si sayesinde (oluşturulduğu izlek ana izlek).
 */

/**
 * 02_qprinter_pdf_export.cpp -- QPrinter + QPrintPreviewDialog ornegi
 *
 * Bolum 8 - Qt ile OBYS GUI Tamamlama
 * Unite 2 - Raporlama ve Yazici
 * Konu 2 - QPrinter ile Kagida/PDF'e Yazdirma
 *
 * Bagimsiz derlenebilir tek dosyalik bir Qt GUI ornegi. Uc buton
 * sunar:
 *   [Yazdir]      -> QPrintDialog + QPrinter ile yazici cikisi
 *   [Onizleme]    -> QPrintPreviewDialog ile ekran onizleme
 *   [PDF Kaydet]  -> QPrinter ciktisinin PDF dosyasina yazilmasi
 *
 * Ornek bir QTextDocument sahte veri ile kurulur ve her uc aksiyon
 * ayni belge uzerinden calisir.
 *
 * Derleme (qmake yok, tek dosya):
 *   g++ -std=c++20 -fPIC \
 *       $(pkg-config --cflags --libs Qt6Widgets Qt6PrintSupport) \
 *       -o 02_qprinter 02_qprinter_pdf_export.cpp
 *
 * Ya da CMake ile:
 *   find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport)
 *   target_link_libraries(app PRIVATE
 *       Qt6::Widgets Qt6::PrintSupport)
 */

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextDocument>
#include <QVBoxLayout>
#include <QWidget>

#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>

namespace {

// Sahte veri ile ornek bir QTextDocument olusturur.
QTextDocument* ornekBelgeOlustur(QObject* parent) {
    auto* belge = new QTextDocument(parent);
    belge->setHtml(QStringLiteral(
        "<html><body>"
        "<h1>Ornek Rapor</h1>"
        "<p>Bu belge <b>QPrinter</b> ornegi icin uretilmistir.</p>"
        "<table border=\"1\" cellpadding=\"4\" "
        "cellspacing=\"0\" width=\"100%\">"
        "<tr><th>Ad</th><th>Puan</th></tr>"
        "<tr><td>Ayse Yilmaz</td><td>92</td></tr>"
        "<tr><td>Mehmet Demir</td><td>78</td></tr>"
        "<tr><td>Zeynep Kaya</td><td>85</td></tr>"
        "</table>"
        "<p>Yukaridaki tablo, rapor olusturma akisinin test "
        "edilmesi icin kullanilir.</p>"
        "</body></html>"));
    return belge;
}

} // namespace

int main(int argc, char* argv[]) {
    QApplication uygulama(argc, argv);
    QCoreApplication::setOrganizationName(
        QStringLiteral("KTU Bilgisayar Bilimleri"));
    QCoreApplication::setApplicationName(
        QStringLiteral("QPrinter Ornek"));

    // Ana pencere ve duzen
    QWidget pencere;
    pencere.setWindowTitle(QStringLiteral("QPrinter / PDF Ornegi"));

    auto* dikeyDuzen = new QVBoxLayout(&pencere);

    auto* bilgi = new QPushButton(
        QStringLiteral("Bu ornek bir QTextDocument'i yazici, "
                       "onizleme ve PDF'e gonderir."),
        &pencere);
    bilgi->setFlat(true);
    bilgi->setEnabled(false);
    dikeyDuzen->addWidget(bilgi);

    auto* butonSatiri = new QHBoxLayout();
    auto* yazdirBtn   = new QPushButton(
        QStringLiteral("&Yazdir..."), &pencere);
    auto* onizlemeBtn = new QPushButton(
        QStringLiteral("&Onizleme..."), &pencere);
    auto* pdfBtn      = new QPushButton(
        QStringLiteral("PDF &Kaydet..."), &pencere);
    butonSatiri->addWidget(yazdirBtn);
    butonSatiri->addWidget(onizlemeBtn);
    butonSatiri->addWidget(pdfBtn);
    dikeyDuzen->addLayout(butonSatiri);

    // Ortak belge
    QTextDocument* belge = ornekBelgeOlustur(&pencere);

    // [Yazdir] -- QPrintDialog ile yazici secimi
    QObject::connect(yazdirBtn, &QPushButton::clicked,
                     [&pencere, belge]() {
        QPrinter yazici(QPrinter::HighResolution);
        QPrintDialog dlg(&yazici, &pencere);
        dlg.setWindowTitle(QStringLiteral("Belgeyi Yazdir"));
        if (dlg.exec() == QDialog::Accepted) {
            belge->print(&yazici);
        }
    });

    // [Onizleme] -- QPrintPreviewDialog; paintRequested ile belge cizilir
    QObject::connect(onizlemeBtn, &QPushButton::clicked,
                     [&pencere, belge]() {
        QPrinter yazici(QPrinter::HighResolution);
        QPrintPreviewDialog onizleme(&yazici, &pencere);
        onizleme.setWindowTitle(QStringLiteral("Baski Onizleme"));
        QObject::connect(&onizleme,
                         &QPrintPreviewDialog::paintRequested,
                         [belge](QPrinter* p) { belge->print(p); });
        onizleme.exec();
    });

    // [PDF] -- Dosya sec, OutputFormat'i PDF yap, yazdir.
    QObject::connect(pdfBtn, &QPushButton::clicked,
                     [&pencere, belge]() {
        const QString yol = QFileDialog::getSaveFileName(
            &pencere,
            QStringLiteral("PDF olarak kaydet"),
            QStringLiteral("rapor.pdf"),
            QStringLiteral("PDF Dosyalari (*.pdf)"));
        if (yol.isEmpty()) {
            return;
        }
        QPrinter yazici(QPrinter::HighResolution);
        yazici.setOutputFormat(QPrinter::PdfFormat);
        yazici.setOutputFileName(yol);
        belge->print(&yazici);
    });

    pencere.resize(420, 120);
    pencere.show();
    return uygulama.exec();
}

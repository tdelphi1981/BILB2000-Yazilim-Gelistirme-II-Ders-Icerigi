# Değişiklik Günlüğü

Bu dosyadaki tüm önemli değişiklikler bu belgede kayıt altına alınır.

Format [Keep a Changelog](https://keepachangelog.com/tr-TR/1.1.0/) kılavuzuna
dayalıdır ve bu proje [Semantic Versioning](https://semver.org/lang/tr/)
kurallarına uyar.

## [Yayınlanmadı]

### Eklendi
- Ünite 6 için çoklu dil (i18n) desteği planlanıyor.

## [1.0.0] - 2026-04-11

OBYS'nin ilk kararlı sürümü. Bölüm 8 boyunca geliştirilen tüm Qt GUI
bileşenleri ve Ünite 5 kapsamında eklenen kurulum hazırlığı özellikleri
bu sürüme dahildir.

### Eklendi
- Release ve Debug profilleri için `CMakePresets.json` dosyası.
- `CMAKE_INTERPROCEDURAL_OPTIMIZATION` ile Link Time Optimization (LTO).
- `configure_file` ile otomatik üretilen `version.h` başlık dosyası.
- macOS için `deploy_macos.sh` dağıtım betiği (macdeployqt + DMG).
- Windows için `deploy_windows.ps1` dağıtım betiği (windeployqt + Inno).
- `obys_installer.iss` Inno Setup kurulum betiği (Türkçe arayüz).
- `docs/user-guide.md` son kullanıcı kılavuzu.
- `translations/obys_tr.ts` Qt Linguist çeviri iskeleti.
- "Hakkında" diyaloğunda derleme sürümü bilgisi gösterimi.
- `QStandardPaths::AppDataLocation` altında kullanıcı veri dizini.

### Değişti
- `main.cpp` artık `version.h` başlığından sürüm bilgisini okur.
- CMake proje sürümü `1.0.0` olarak sabitlendi.
- Varsayılan build tipi `Release` olarak ayarlandı.

### Düzeltildi
- Windows dağıtımında eksik Qt eklentilerinin (`platforms`,
  `imageformats`) installer'a dahil edilmemesi sorunu.

## [0.4.0] - 2026-04-04

Ünite 4 kapsamında eklenen mini proje tamamlama özellikleri.

### Eklendi
- `JsonExporter` sınıfı ile JSON dışa aktarma.
- Aydınlık / Karanlık tema (QRC + QSS).
- Son Açılanlar alt menüsü (QSettings tabanlı).
- Genişletilmiş klavye kısayolları.

## [0.3.0] - 2026-03-28

### Eklendi
- `QtCharts` ile not dağılımı grafik paneli (Ünite 3).

## [0.2.0] - 2026-03-21

### Eklendi
- `QAbstractTableModel` tabanlı notlar modeli (Ünite 2).
- `QSortFilterProxyModel` ile sıralama ve filtreleme.

## [0.1.0] - 2026-03-14

### Eklendi
- İlk Qt ana pencere iskeleti, menü ve araç çubuğu (Ünite 1).
- OBYS STL çekirdeğinin Qt GUI'ye entegrasyonu.

[Yayınlanmadı]: https://example.com/obys/compare/v1.0.0...HEAD
[1.0.0]: https://example.com/obys/releases/tag/v1.0.0
[0.4.0]: https://example.com/obys/releases/tag/v0.4.0
[0.3.0]: https://example.com/obys/releases/tag/v0.3.0
[0.2.0]: https://example.com/obys/releases/tag/v0.2.0
[0.1.0]: https://example.com/obys/releases/tag/v0.1.0

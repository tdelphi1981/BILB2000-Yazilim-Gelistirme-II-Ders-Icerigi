# OBYS v3 - Grafik ve Istatistik

Bolum 8 / Unite 3 icin tam ornek proje. Unite 2'deki OBYS v2 Raporlama
projesinin uzerine QTabWidget ile grafik sekmeleri eklenmis halidir.

## Gereksinimler

- CMake >= 3.21
- Qt 6.5+ (Widgets, PrintSupport, Charts)
- C++20 destekli derleyici

## Derleme

```bash
cmake -B build
cmake --build build
./build/obys_v3
```

## Proje Yapisi

```
04_obys_v3_grafik/
├── CMakeLists.txt
├── include/
│   ├── obys_domain.h          # Unite 1'den kopya
│   └── obys_servis.h          # Unite 1'den kopya
└── src/
    ├── main.cpp
    ├── main_window.h/.cpp     # QTabWidget (Notlar / Grafikler / Trend)
    ├── notlar_model.h/.cpp    # YENI: QAbstractTableModel (id/ogrenci/not/harf)
    ├── istatistik_model.h/.cpp# YENI: Harf histogrami proxy model
    ├── grafik_paneli.h/.cpp   # YENI: QChart + QVBarModelMapper sarmalayici
    ├── not_giris_dialog.h/.cpp    # Unite 1'den kopya
    ├── persistence_manager.h/.cpp # Unite 1'den kopya
    ├── transkript_uretici.h/.cpp  # Unite 2'den kopya
    └── ders_raporu.h/.cpp         # Unite 2'den kopya
```

## Smoke Test

1. `./build/obys_v3` ile uygulamayi baslat.
2. **Notlar** sekmesinde onceden yuklenmis 5 satirlik bir tablo gormelisin
   (Ayse Yilmaz / Mehmet Demir / Zeynep Kaya). Sutunlar: `#`, `Ogrenci`,
   `Not`, `Harf`.
3. **Grafikler** sekmesine gec. Bar chart, Notlar sekmesindeki harf
   notlarinin (AA/BA/.../FF) histogramini gostermelidir.
4. Tekrar **Notlar** sekmesine gec, **Not Ekle...** butonuna bas ve
   yeni bir not ekle (ornegin puan 95 -> AA).
5. **Grafikler** sekmesine geri don. AA cubugu otomatik olarak bir
   artmis olmali. Bu, `NotlarModel::modelReset ->
   IstatistikModel::kaynakDegisti -> dataChanged -> QVBarModelMapper ->
   QBarSeries` zincirinin calistigini gosterir.
6. **Trend** sekmesine gec. Uc ogrenci icin dort donemlik (Guz 2023,
   Bahar 2024, Guz 2024, Bahar 2025) ortalama trendini gosteren
   QLineSeries grafigi cikmalidir.
7. **Dosya -> Transkript Yazdir...** ve **Toplu Rapor...** eylemleri
   Unite 2'deki gibi calismaya devam eder (Onizleme / Yazdir / PDF).

## Observer Zinciri

```
OBYSServis.notEkle()
    -> NotlarModel::yenile() (beginResetModel/endResetModel)
    -> NotlarModel::modelReset
    -> IstatistikModel::kaynakDegisti()
    -> IstatistikModel::dataChanged(sol, sag)
    -> QVBarModelMapper
    -> QBarSeries
    -> QChartView repaint
```

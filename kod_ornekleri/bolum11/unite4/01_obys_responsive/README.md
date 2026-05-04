# OBYS Responsive - Bölüm 11 Ünite 4 Mini Proje

Bu proje, Yazılım Geliştirme II kitabının 11. bölümünün dorukda yer alan mini projesidir. OBYS uygulamasının çok kanallı ve responsive (UI bloklamayan) sürümünü içerir.

## Özellikler

- **Thread-safe Depo**: shared_mutex ile çoklu okuyucu / tek yazıcı.
- **CSV İçe Aktarma**: Worker pattern ile arka planda; ilerleme + iptal destekli.
- **CSV Dışa Aktarma**: Worker pattern ile arka planda.
- **GPA Hesaplama**: QtConcurrent::mapped ile paralel; QFutureWatcher ile ilerleme.
- **Rastgele Test Verisi**: Hızlı test için 10.000 kayıt oluşturma.

## Derleme

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build .
```

## Test Senaryoları

1. ``Islem -> Rastgele Test Verisi`` ile 10.000 kayıt oluştur.
2. ``Islem -> GPA Hesapla`` -- ilerleme dialog'u açılır, UI canlı kalır.
3. ``Dosya -> CSV Disa Aktar`` -- 10.000 kayıt dosyaya yazılır.
4. ``Dosya -> CSV Ice Aktar`` -- yeni bir CSV yüklenir.
5. Herhangi bir uzun işlem sırasında iptal'e bas.

## Bağımlılıklar

- Qt 6.x (Core, Gui, Widgets, Concurrent)
- C++20 destekli derleyici (GCC 11+, Clang 14+)

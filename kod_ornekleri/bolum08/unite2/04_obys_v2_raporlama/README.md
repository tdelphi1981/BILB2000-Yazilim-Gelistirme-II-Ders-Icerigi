# OBYS v2 - Raporlama ve Yazıcı

Bölüm 8 / Ünite 2 tam örnek projesi. Ünite 1'deki `04_obys_v1_binary_kayit`
projesinin üzerine raporlama ve yazıcı desteği eklenmiştir:

- **Transkript Yazdır** (Konu 1): Seçilen öğrencinin transkriptini
  HTML olarak üretir, QTextDocument'a yükler ve Önizleme / Yazdır /
  PDF olarak kaydet seçeneklerinden birini sunar.
- **Toplu Rapor** (Konu 3): Seçilen bir dersin istatistiklerini ve
  öğrenci not listesini içeren çok sayfalı bir QTextDocument üretir;
  bölümler arası sayfa sonu ile ayrılır.

## Gereksinimler

- Qt 6.5+
- C++20 destekleyen derleyici
- CMake 3.21+

## Derleme

```bash
cd 04_obys_v2_raporlama
cmake -B build
cmake --build build
./build/obys_v2
```

Qt'yi özel bir yoldan derlediyseniz:

```bash
cmake -B build -DCMAKE_PREFIX_PATH=/Users/foo/Qt/6.7.0/macos
```

## Smoke Test

1. Uygulama açılınca örnek veriler yüklenir (3 öğrenci, 2 ders).
2. **Dosya > Not Ekle...** ile en az bir not girin.
3. (Opsiyonel) **Dosya > Farklı Kaydet...** ile `.obys` dosyasına
   kaydedin.
4. **Dosya > Transkript Yazdır...** seçin -> listeden bir öğrenci
   seçin.
5. Gelen kutuda **Önizleme** butonunu tıklayın; QPrintPreviewDialog
   içinde transkript görünmeli.
6. Aynı akışı **PDF Kaydet** ile tekrarlayın; seçtiğiniz dosya yolunda
   geçerli bir PDF oluşmalıdır.
7. **Dosya > Toplu Rapor...** ile bir ders seçin; çok sayfalı raporu
   önizleyin / PDF'e aktarın.

## Kaynak Dosyalar

| Dosya | Açıklama |
|-------|----------|
| `include/obys_domain.h`    | Ünite 1'den kopyalanan domain struct'ları |
| `include/obys_servis.h`    | Ünite 1'den kopyalanan servis stub'ı |
| `src/main.cpp`             | QApplication giriş noktası |
| `src/main_window.h/.cpp`   | Transkript / Toplu Rapor aksiyonları |
| `src/not_giris_dialog.*`   | Ünite 1'den kopya not giriş formu |
| `src/persistence_manager.*`| Ünite 1'den kopya `.obys` okuma/yazma |
| `src/transkript_uretici.*` | Konu 1: HTML transkript üretici |
| `src/ders_raporu.*`        | Konu 3: Cok sayfalı QTextDocument üretici |

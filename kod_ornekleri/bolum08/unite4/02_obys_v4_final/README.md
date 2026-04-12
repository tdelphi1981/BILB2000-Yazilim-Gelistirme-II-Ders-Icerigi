# OBYS v4 - Final Mini Proje Örneği

Bu dizin, Bölüm 8 - Ünite 4 "OBYS GUI Mini Proje Tamamlama" bölümünün
tam örnek projesidir. Ünite 3'teki `04_obys_v3_grafik/` projesi üzerine
aşağıdaki özellikler eklenmiştir:

- `JsonExporter` sınıfı ve "Dosya > Dışa Aktar > JSON" aksiyonu
- "Dosya > Son Açılanlar" alt menüsü (QSettings tabanlı, 5 giriş)
- "Görünüm > Aydınlık/Karanlık Tema" aksiyonları (QRC + QSS)
- "Yardım > Hakkında..." diyaloğu
- Genişletilmiş klavye kısayolları
- `closeEvent` içinde pencere geometrisi/durumu QSettings'e kaydı
- Kayıp koruma (dirty) sorusu hem açma hem kapanışta

## Derleme

```bash
cmake -B build
cmake --build build
./build/obys_v4
```

Qt6.5+ (Widgets + PrintSupport + Charts) ve C++20 destekli derleyici
gereklidir. QSettings platform başına farklı yerlerde saklar
(macOS: `~/Library/Preferences`, Linux: `~/.config/KTU*`, Windows:
registry), bu yüzden aynı kullanıcıda tek bir ayar deposu tutulur.

## İkon Ekleme (Opsiyonel)

Menülere görsel ikon eklemek için:

1. Kendi SVG/PNG ikonlarınızı `resources/icons/` altına koyun
   (öneri: `yeni.svg`, `ac.svg`, `kaydet.svg`, `disa_aktar.svg`,
   `yazdir.svg`, `not_ekle.svg`).
2. `resources/obys.qrc` içindeki yorum satırlarını açın.
3. `main_window.cpp` içinde `addAction` çağrılarını
   `addAction(QIcon(":/icons/yeni.svg"), tr("&Yeni"))` biçimine çevirin.

Bu örnek, lisans sorunlarını engellemek için ikonları dahil etmez.

## Smoke Test Senaryosu

Aşağıdaki 14 adım, Ünite 4 Konu 3 içeriğiyle birebir örtüşür. Sırasıyla
uygulayarak tüm özelliklerin çalıştığını doğrulayabilirsiniz.

1. Uygulamayı başlat. Pencere başlığı `OBYS v4 - İsimsiz` olmalı,
   Notlar sekmesinde 5 örnek not görünmeli.
2. **Grafikler** sekmesine geç; histogram üç kategori göstermeli
   (Düşük / Orta / Yüksek).
3. **Trend** sekmesine geç; üç öğrenci için çizgi grafiği görünmeli.
4. `Ctrl+Shift+N` (Not Ekle). Bir öğrenci/ders seçip sayısal not gir
   ve Tamam. Başlıkta yıldız (`*`) görünmeli, Grafikler sekmesi canlı
   güncellenmeli.
5. `Ctrl+S` (Kaydet). Bir `oturum1.obys` adıyla kaydet. Yıldız kaybolur,
   başlık dosya adını yansıtır.
6. `Ctrl+Shift+E` (Dışa Aktar > JSON). `oturum1.json` adıyla kaydet ve
   dosyayı metin editöründe aç. `surum`, `olusturulmaZamani`,
   `ogrenciler`, `dersler`, `notlar` alanlarını görmelisin. Not girişi
   SayisalNot olduğundan `{"tur":"sayisal","deger":..}` biçiminde
   olmalı.
7. `Ctrl+2` ile Karanlık Tema'ya geç. Arka plan koyu tonlara dönmeli.
8. `Ctrl+1` ile Aydınlık Tema'ya dön.
9. `Ctrl+P` (Transkript Yazdır) bir öğrenci seç ve "PDF Kaydet" ile
   transkripti PDF olarak üret.
10. `Ctrl+Shift+P` (Toplu Rapor) bir ders seç ve "Önizleme" ile baskı
    önizlemesini kontrol et.
11. `Ctrl+N` (Yeni) - kaydedilmemiş değişiklik olmadığı için doğrudan
    boş oturum açılmalı.
12. `Ctrl+O` (Aç) ve az önce kaydettiğin `oturum1.obys`'u aç. Dosya
    "Son Açılanlar" menüsünde 1. sıraya geçmiş olmalı.
13. Pencereyi yeniden boyutlandır, farklı bir sekmede bırak, `Ctrl+Q`
    ile çık. Uygulamayı tekrar başlat - aynı boyut ve seçili tema
    geri yüklenmeli, "Son Açılanlar" menüsü `oturum1.obys` girişini
    göstermeli.
14. `F1` (Yardım > Hakkında) diyaloğunu aç; sürüm `1.0.0` olarak
    görünmeli.

Bu 14 adım başarıyla tamamlandığında OBYS mini projesinin GUI katmanı
üzerinde yer alan tüm özellikler uçtan uca doğrulanmış olur.

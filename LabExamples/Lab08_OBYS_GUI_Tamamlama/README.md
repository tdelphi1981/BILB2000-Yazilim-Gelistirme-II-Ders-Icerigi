# Lab 08 -- OBYS GUI Tamamlama (Başlangıç Kodu)

Bu dizin, **Lab 08: OBYS GUI Tamamlama** laboratuvarı için başlangıç kodudur.
Proje, Bölüm 8 boyunca adım adım geliştirdiğimiz **OBYS v4** projesinin
birebir kopyasıdır; yalnız `// GOREV-N:` yorumlarıyla işaretlenmiş
yerlerde sizin doldurmanız gereken boşluklar bırakılmıştır. Başlangıç
kodu olduğu gibi uyarısız derlenir ve çalışır -- görevleri tamamladıkça
yeni özellikler aktifleşecektir.

İlgili lab föyü: `Materials/Labs/Lab08_OBYS_GUI_Tamamlama.tex`.

## Gereksinimler

- Qt6 (6.5+) -- `Widgets`, `PrintSupport`, `Charts`
- CMake 3.21+
- C++20 destekli bir derleyici (clang 15+, g++ 11+, MSVC 19.30+)

## Derleme ve Çalıştırma

```bash
cp -r out/LabExamples/Lab08_OBYS_GUI_Tamamlama ~/lab08
cd ~/lab08
cmake -B build                        # gerekirse -DCMAKE_PREFIX_PATH=/Qt/6.x/macos
cmake --build build -j
./build/obys_v4                       # Linux / macOS
./build/obys_v4.exe                   # Windows
```

Qt'nin standart yolda olmadığı sistemlerde:

```bash
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/macos
```

## Görev Listesi

Her `// GOREV-N:` işareti lab föyündeki bir talebe karşılık gelir. Toplam
**11 görev** (+ 3 bonus) vardır. Kaynak dosyalarda `GOREV-` ifadesini
aratarak tüm işaretleri hızlıca bulabilirsiniz:

```bash
grep -rn "GOREV-" include/ src/
```

### Çalışma 1 -- Projeyi Anlama ve Oturum Kaydını Sınama (20 dk)

Çalışma 1 kod değişikliği gerektirmez. Mevcut `obys_v4` uygulamasını
çalıştırıp örnek veri oluşturun, kaydedin, uygulamayı kapatıp yeniden
açın ve oturumu geri yükleyin. Gözlemlerinizi lab defterinize yazın:
hangi sınıflar QDataStream için `operator<<` / `operator>>` aşırı
yüklemesi sağlamak zorunda?

### Çalışma 2 -- Transkripte Öğrenci Fotoğraf Alanı (30 dk)

| Görev | Dosya | Açıklama |
|-------|-------|----------|
| GOREV-1 | `include/obys_domain.h` | `Ogrenci` struct'ına `QString fotografYolu` alanı ekleyin |
| GOREV-2 | `include/obys_domain.h` | `operator<<` içinde `fotografYolu`'nu akışa yazın |
| GOREV-3 | `include/obys_domain.h` | `operator>>` içinde `fotografYolu`'nu okuyun (ve `PersistenceManager::VERSION`'u artırın) |
| GOREV-4 | `src/not_giris_dialog.cpp` | "Fotoğraf Seç" butonu + QFileDialog + önizleme etiketi |
| GOREV-5 | `src/transkript_uretici.cpp` | HTML gövdesine koşullu `<img>` etiketi yerleştirin |

### Çalışma 3 -- Grafik Paneline Ders Filtresi (30 dk)

| Görev | Dosya | Açıklama |
|-------|-------|----------|
| GOREV-6 | `src/grafik_paneli.cpp` / `.h` | Üst serite QComboBox ve "Ders:" etiketi ekleyin |
| GOREV-7 | `src/grafik_paneli.cpp` | `setModel` içinde ders listesini doldurun; `modelReset` sinyalini dinleyin (QSignalBlocker) |
| GOREV-8 | `src/grafik_paneli.cpp` / `.h` | `dersDegisti(int)` slot'u -- seçilen derse göre seriyi güncelleyin |

### Çalışma 4 -- JSON Zenginleştirme ve Tema Kısayolu (30 dk)

| Görev | Dosya | Açıklama |
|-------|-------|----------|
| GOREV-9  | `src/json_exporter.cpp` | Kök nesneye `"kayitlar"` dizisi: `ogrenciNo`, `dersKodu`, `not`, `harfNotu` |
| GOREV-10 | `src/main_window.cpp` | `disaAktarJson` içinde QSettings ile `"disaAktar/sonDizin"` yol hafızası |
| GOREV-11 | `src/main_window.cpp` | `menuleriKur` içinde Ctrl+Shift+T kısayolu ile tema toggle + 3 sn durum mesajı |

### Bonus Görevler (İleri Seviye)

Lab föyündeki Bonus 1-3 bölümünde listelenmiştir -- bu dosyalarda hazır
iskelet yoktur, tamamen öğrenci tarafından eklenecektir:

- **Bonus 1**: Toplu transkript PDF dışa aktarımı (`QProgressDialog`)
- **Bonus 2**: Grafik türü seçimi (Bar / Pie / Line, `QButtonGroup`)
- **Bonus 3**: `QTimer` ile 5 dakikada bir otomatik yedekleme (`obys_oturum.bak`)

## İpuçları

1. Her görevin yorumu, onu tamamlayacak kısa bir ipucu kodu içerir.
2. Tek bir görevi bitirdiğinizde yeniden derleyin:
   `cmake --build build -j && ./build/obys_v4`
3. `-Wall -Wextra -Wpedantic` etkindir -- uyarı vermeyen kod yazın.
4. QDataStream serileştirmesine alan eklerken
   `PersistenceManager::VERSION` sabitini 1'den 2'ye çıkarmadan eski
   dosyalar bozulur. Geriye dönük uyumluluk için `version == 1`
   okumalarında yeni alanı atlayıp boş `QString` döndürün.
5. Grafik panelinde QComboBox sinyalini bağlarken `QSignalBlocker`
   kullanarak ilk doldurma sırasında `currentIndexChanged` tetiklenmesini
   engelleyin.
6. JSON çıktınızı doğrulamak için: `python3 -m json.tool ciktilar.json`.
7. QSettings anahtarları platforma göre farklı yerde saklanır
   (macOS'ta `~/Library/Preferences/com.ktu.obys.plist`,
   Linux'ta `~/.config/KTU/OBYS.conf`).

## Teslim

- Çalışma dizinini (build klasörü hariç) sıkıştırıp yükleyin.
- Dosya adı: `Lab08_OgrenciNo_AdSoyad.zip`
- En az üç ekran görüntüsü: fotoğraflı transkript, filtrelenmiş grafik,
  JSON çıktısı.
- Kısa bir `CEVAP.md` içinde yaptığınız değişiklikleri, karşılaştığınız
  hataları ve çözüm yolunuzu anlatın.

Başarılar!

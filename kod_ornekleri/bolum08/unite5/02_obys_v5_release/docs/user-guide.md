# OBYS Kullanıcı Kılavuzu (v1.0.0)

Hoş geldin! Bu kılavuz, OBYS (Öğrenci Bilgi Yönetim Sistemi) uygulamasının
son kullanıcılar için temel özelliklerini açıklar. Bölüm 8 boyunca
geliştirilen Qt arayüzünü ve Ünite 5 kapsamında dağıtım paketine eklenen
özellikleri kapsar.

## 1. Kurulum

### Windows
1. `OBYS-v1.0.0-Setup.exe` dosyasını çalıştır.
2. Kurulum sihirbazını takip et (Türkçe arayüz).
3. Masaüstü kısayolu isteyip istemediğini seç.
4. Kurulum tamamlandığında OBYS otomatik olarak başlayabilir.

### macOS
1. `OBYS-v1.0.0-macOS.dmg` dosyasını çift tıkla.
2. Açılan pencerede `OBYS.app` simgesini `Applications` klasörüne sürükle.
3. İlk çalıştırmada Gatekeeper uyarısı görürsen Finder'da sağ tıklayıp
   "Aç" seçeneğini kullan.

## 2. İlk Çalıştırma

İlk çalıştırmada OBYS, kullanıcı verilerini saklamak için işletim
sisteminin standart veri dizinini kullanır:
- **Windows:** `%APPDATA%\KTU Bilgisayar Bilimleri\OBYS\`
- **macOS:** `~/Library/Application Support/KTU Bilgisayar Bilimleri/OBYS/`

Pencere boyutu, seçtiğin tema ve son açılan dosyalar bu dizine yazılır.

## 3. Dosya İşlemleri

- **Aç (Ctrl+O):** JSON formatında kaydedilmiş bir OBYS veri dosyasını
  yükler.
- **Kaydet (Ctrl+S):** Mevcut dosyayı günceller.
- **Farklı Kaydet (Ctrl+Shift+S):** Yeni bir dosyaya yazar.
- **Dışa Aktar > JSON:** Tüm öğrenci ve not verisini düz JSON olarak
  dışa aktarır (raporlama amaçlı).

## 4. Not Giriş ve Düzenleme

1. `Kayıt > Yeni Not` menüsünden (veya `Ctrl+N`) not giriş diyaloğunu aç.
2. Öğrenci numarasını, ders kodunu ve notu gir.
3. `Kaydet` düğmesine bas. Not tabloda anında görünür.
4. Tabloda herhangi bir satıra çift tıklayarak düzenleme diyaloğunu açabilirsin.

## 5. Rapor Üretimi

- **Transkript (Ctrl+T):** Seçili öğrenci için PDF transkript üretir.
- **Ders Raporu:** Seçili ders için not dağılımı ve başarı istatistiği
  raporu üretir.
- **Grafik Paneli:** Not dağılımını histogram olarak gösterir.

## 6. Tema Değiştirme

`Görünüm > Tema` menüsünden Aydınlık / Karanlık seçenekleri arasında
geçiş yapabilirsin. Seçim bir sonraki çalıştırmada hatırlanır.

## 7. Hakkında

`Yardım > Hakkında` menüsü açık derleme sürümünü ve yapı tarihini
gösterir. Hata bildirirken bu bilgiyi paylaşman destek sürecini hızlandırır.

## 8. Sorun Giderme

| Sorun | Çözüm |
|-------|-------|
| Uygulama açılmıyor | Qt çalışma zamanı bağımlılıklarının installer ile kurulduğundan emin ol. |
| Dosya açılmıyor | Dosyanın JSON formatında ve okuma izinli olduğunu kontrol et. |
| Tema kaydedilmiyor | `AppData` / `Application Support` dizininin yazılabilir olduğundan emin ol. |

## 9. Kaldırma

- **Windows:** `Ayarlar > Uygulamalar > OBYS > Kaldır` yolunu izle.
- **macOS:** `Applications` klasöründen `OBYS.app` dosyasını çöp kutusuna taşı.

Kullanıcı verileri (ayarlar ve belgeler) kaldırma sırasında silinmez;
manuel olarak kaldırman gerekir.

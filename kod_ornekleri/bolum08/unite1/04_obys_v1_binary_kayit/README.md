# OBYS v1 - Binary Kayıt Örneği

Bu örnek, **Bölüm 8 Ünite 1 - Ders Notu Giriş Formu ve Oturum Kaydı**
konusunun bağımsız derlenebilir tam uygulamasıdır. `QDataStream` ile
`.obys` uzantılı binary dosyaya oturum kaydı yapar ve aynı formattan
geri yükler.

## Gereksinimler

- CMake 3.21 veya üzeri
- Qt 6.5 veya üzeri (Widgets modülü)
- C++20 destekli bir derleyici (GCC 11+, Clang 14+, MSVC 2022+)

## Derleme

Proje dizininde:

```bash
cmake -B build
cmake --build build
```

Qt kurulumunuz standart bir yerde değilse `CMAKE_PREFIX_PATH` ile
belirtin:

```bash
cmake -B build -DCMAKE_PREFIX_PATH=/opt/Qt/6.7.0/gcc_64
cmake --build build
```

## Çalıştırma

```bash
./build/obys_v1
```

## Dizin Yapısı

```
04_obys_v1_binary_kayit/
  CMakeLists.txt          - CMake yapılandırması
  README.md               - Bu dosya
  include/
    obys_domain.h         - Ogrenci/Ders/DersKaydi + operator<< / >>
    obys_servis.h         - Basit bellek-içi OBYSServis stub'ı
  src/
    main.cpp              - QApplication giriş noktası
    main_window.h/.cpp    - Dosya menüsü + Not Ekle butonu
    not_giris_dialog.h/.cpp - Modal not girişi dialog'u
    persistence_manager.h/.cpp - .obys kaydet / yükle
```

## Dosya Formatı (.obys v1)

```
[MAGIC    : quint32 = 0x4F425953 ('OBYS')]
[VERSION  : quint32 = 1]
[ogrenciSayisi : quint32]
  [Ogrenci x N]
[dersSayisi : quint32]
  [Ders x N]
[notSayisi : quint32]
  [DersKaydi x N]
```

Okuma sırasında magic değeri kontrol edilir; uyuşmazsa dosya
reddedilir. `VERSION` değeri gelecekte artabilir; yükleyici yüksek
sürüm numaralarını savunmacı şekilde reddeder.

## Smoke Test

Aşağıdaki adımları uygulayarak serileştirmenin doğru çalıştığını
doğrulayabilirsiniz:

1. `./build/obys_v1` ile uygulamayı başlatın.
2. **Not Ekle...** butonuna tıklayın. Açılan dialog'da:
   - Bir öğrenci ve ders seçin.
   - Not türünü *Sayısal* bırakın, değer olarak `85` girin.
   - *Tamam* ile kaydedin.
3. Liste alanında yeni not görünmelidir, başlıkta `*` belirir.
4. **Dosya → Farklı Kaydet...** ile `test.obys` adıyla kaydedin.
   Başlıktaki `*` kaybolmalı, dosya adı güncellenmelidir.
5. **Dosya → Yeni** ile listeyi temizleyin.
6. **Dosya → Aç...** ile az önce kaydettiğiniz `test.obys` dosyasını
   seçin. Eklediğiniz not listede tekrar görünmelidir.

Eğer 6. adımda not tekrar görünüyorsa binary kaydet/yükle döngüsü
sorunsuz çalışıyor demektir.

## Notlar

- Domain sınıfları bu örnekte bağımsızlık için `include/obys_domain.h`
  içinde yeniden tanımlanmıştır. Gerçek kitap projesinde bu sınıflar
  Bölüm 3 Ünite 5'te tanımlanan ortak kütüphaneden gelir.
- `NotTuru` bir `std::variant<SayisalNot, HarfNotu, MuafNot>` tipidir;
  serileştirmede önce bir etiket (tag) baytı yazılır, ardından o
  alternatifin içeriği.
- Kod C++20 standardı ile yazılmıştır (`auto`, structured bindings,
  `if constexpr`, `std::visit`).

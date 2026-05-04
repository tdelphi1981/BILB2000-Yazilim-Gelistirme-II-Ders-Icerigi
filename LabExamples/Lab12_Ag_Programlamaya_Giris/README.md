# Lab 12 - Ağ Programlamaya Giriş (Başlangıç Kodları)

Bu dizin, Lab 12 çalışmaları için iskelet kod dosyalarını içerir. Her dosyadaki
`// GOREV` yorumlarını uygun kodla değiştirin.

## İÇERİK

| Dizin                      | Açıklama                                              |
|----------------------------|-------------------------------------------------------|
| `calisma1_posix/`          | POSIX TCP echo sunucu/istemci (saf C++)               |
| `calisma2_qt_istemci/`     | Qt TCP istemci (QTcpSocket + sinyal/slot)             |
| `calisma3_qt_chat/`        | Qt TCP server + length-prefix chat (QTcpServer)       |
| `bonus_json/`              | JSON komut çözücü (QJsonDocument)                     |

## DERLEME (calisma1 - POSIX)

POSIX echo sunucu ve istemci için Qt gerekmez:

```
cd calisma1_posix
cmake -B build
cmake --build build
./build/echo_sunucu      # Bir terminalde
./build/echo_istemci     # Baska terminalde
```

Veya doğrudan g++ ile:

```
g++ -std=c++20 -O2 -o echo_sunucu echo_sunucu.cpp
g++ -std=c++20 -O2 -o echo_istemci echo_istemci.cpp
```

Test için netcat veya telnet:

```
nc localhost 9000
telnet localhost 9000
```

## DERLEME (calisma2, calisma3, bonus_json - Qt)

Qt 6 (Core + Network + Widgets) kurulu olmalıdır:

```
cd calisma2_qt_istemci
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build build
./build/qt_istemci
```

## TÜMÜNÜ BİRDEN DERLEME

Üst dizindeki `CMakeLists.txt` tüm alt projeleri içerir:

```
cmake -B build
cmake --build build
```

Qt 6 yoksa POSIX kısmı yine derlenir; Qt projeleri uyarı ile atlanır.

## TEST SENARYOSU

1. **Çalışma 1**: `./echo_sunucu` çalıştır, başka terminalde
   `./echo_istemci` veya `nc localhost 9000` ile bağlan, mesaj yaz.
2. **Çalışma 2**: Çalışma 1'in sunucusu çalışırken Qt istemci ile bağlan.
3. **Çalışma 3**: Qt chat sunucusunu çalıştır, 2 ayrı `nc localhost 9100`
   istemci ile bağlanıp birinden gönderilen mesajın diğerinde göründüğünü
   doğrula. Length-prefix çerçevelemesi nedeniyle gönderim için yardımcı
   `gonderim_aracilari/` script'i veya manuel hex girdi gerekebilir.
4. **Bonus**: `{"komut":"ping"}` gibi JSON mesajlar gönder, cevap al.

## KAYNAKLAR

- Lab 12 föyü: `Materials/Labs/Lab12_Ag_Programlamaya_Giris.tex`
- Cevap anahtarı (eğitimci): `Lab12_Ag_Programlamaya_Giris_Cevap.tex`
- Bölüm 12 Ünite 2-4: POSIX, Qt soket, çerçeveleme

## YAYGIN HATALAR

- `socket()` çağrısı için `<sys/socket.h>` ve `<netinet/in.h>` ekle.
- `SO_REUSEADDR` koymazsan sunucuyu yeniden başlatırken
  "Address already in use" alırsın.
- `QTcpSocket` için `find_package(Qt6 COMPONENTS Network)` ve
  `Qt6::Network` link kütüphanesi gerekir.
- Yeni `connect` syntax kullan; eski SIGNAL/SLOT macros kullanma.
- `htonl`/`ntohl` ile bayt sırası dönüşümü ZORUNLU; localhost'ta
  çalışıp uzakta çökmenin baş suçlusudur.

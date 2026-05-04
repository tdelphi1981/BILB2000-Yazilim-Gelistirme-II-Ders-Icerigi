# Qt TCP Soket İstemci

Bölüm 12 Ünite 3 Konu 1 — Qt'nin `QTcpSocket` sınıfını kullanarak
sinyal/slot tabanlı bir konsol istemcisi.

## Derleme

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build .
```

## Çalıştırma

Önce bir TCP echo sunucu başlatın (Ünite 2 örneklerinden veya bu
ünitenin `02_qt_tcpserver_echo` projesinden):

```bash
./qt_tcpsocket_istemci localhost 9000
```

Stdin'den satır yazın, Enter ile sunucuya gönderilir. Sunucudan gelen
yanıt `<<` prefix'iyle ekrana basılır. Çıkmak için Ctrl-D.

## Öğrendiklerimiz

- `QTcpSocket` ile bağlantı kurma (`connectToHost`)
- `connected`, `disconnected`, `readyRead`, `errorOccurred` sinyalleri
- `QSocketNotifier` ile stdin'i event loop içinde okuma
- Satır tamponu mantığı (parça parça gelen veriyi `\n`'e göre bölmek)

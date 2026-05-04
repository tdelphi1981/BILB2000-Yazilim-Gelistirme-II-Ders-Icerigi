# Qt TCP Echo Sunucu

Bölüm 12 Ünite 3 Konu 2 — `QTcpServer` ile sinyal/slot tabanlı echo sunucu.

## Derleme

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build .
```

## Çalıştırma

```bash
./qt_tcpserver_echo 9000
```

Başka bir terminalde test:

```bash
nc localhost 9000
# ya da bu klasörün kardeş projesi:
../01_qt_tcpsocket_istemci/build/qt_tcpsocket_istemci localhost 9000
```

## Öğrendiklerimiz

- `QTcpServer::listen` ve `newConnection` sinyali
- `nextPendingConnection()` ile bağlantı kabul
- Her istemci için `readyRead` ve `disconnected` sinyallerini bağlama
- `sender()` ile sinyali yayınlayan nesneyi bulma
- `deleteLater()` ile event loop güvenli silme

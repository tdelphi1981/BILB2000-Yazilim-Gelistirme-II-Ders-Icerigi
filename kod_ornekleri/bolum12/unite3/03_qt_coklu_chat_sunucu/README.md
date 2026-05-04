# Qt Çoklu Chat Sunucu

Bölüm 12 Ünite 3 Konu 3 — Bir istemciden gelen mesajı diğer tüm bağlı
istemcilere yayan basit bir broadcast chat sunucu.

## Derleme

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build .
```

## Çalıştırma

Sunucu:

```bash
./qt_coklu_chat_sunucu 9100
```

İki ya da daha fazla istemci (her biri ayrı terminalde):

```bash
nc localhost 9100
```

Birinde yazılan mesaj diğerlerine `<kullanici#> mesaj` formatında düşer.

## Öğrendiklerimiz

- Birden fazla `QTcpSocket*` örneğini `QList` içinde yönetme
- `QHash<QTcpSocket*, T>` ile soket başına meta veri (takma ad, tampon)
- Broadcast pattern: gönderen hariç tüm istemcilere yayın
- Bağlantı koparken listeden çıkarıp `deleteLater()` ile temiz silme
- Satır tamponu (her istemcinin ayrı tamponu olur)

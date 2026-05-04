# JSON Komut/Cevap Protokolü

Bölüm 12 Ünite 4 Konu 2 — `QJsonDocument` ve `QJsonObject` ile basit
bir komut/cevap protokolünün kodlama/çözme katmanı.

## Komut Formatı

```json
{"komut":"login","id":1,"veri":{"kullanici":"ali","sifre":"gizli"}}
```

## Cevap Formatı

```json
{"id":1,"durum":"ok","veri":{"oturum_no":"abc-123"}}
{"id":2,"durum":"hata","hata":"Şifre hatalı"}
```

## Derleme

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build .
```

## Çalıştırma

```bash
./json_komut_cevap
```

Stand-alone test programı round-trip kodla/çöz, hata mesajları ve
Türkçe karakter desteğini sınar.

## Öğrendiklerimiz

- `QJsonDocument::toJson` / `fromJson` ile UTF-8 byte dönüşümü
- `QJsonParseError` ile hata yakalama
- POD-benzeri yapı (struct) ile protokol tipi modelleme
- `std::optional<T>` ile parse hatasını ifade etme
- `QJsonDocument::Compact` ile minimum bayt akışı

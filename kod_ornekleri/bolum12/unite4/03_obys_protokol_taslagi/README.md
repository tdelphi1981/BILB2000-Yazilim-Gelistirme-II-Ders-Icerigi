# OBYS Protokol Taslağı — Bölüm 12'nin Final Mini Projesi

Bölüm 12 Ünite 4 Konu 3 — Length-prefix çerçeveleme + JSON komut/cevap
mimarisinin somut hâli. Bölüm 13'te bu iskelet üzerine gerçek OBYS
işlevleri inşa edilecek.

## Mimari

```
+------------------+        +-------------------+
|  obys_istemci    |  TCP   |   obys_sunucu     |
|  (QTcpSocket)    | <----> |  (QTcpServer)     |
|                  |        |                   |
| Komut JSON       |        | Komut tablosu:    |
|  -> length-prefix|        |  ping, login,     |
|     -> bytes     |        |  ogrenci_listele, |
|                  |        |  ogrenci_ekle     |
| Cevap bytes      |        |                   |
|  -> length-prefix|        | Bellek içi depo   |
|     -> JSON      |        | (QHash<int,...>)  |
+------------------+        +-------------------+
```

İki ortak katman:
- **CerceveCozucu** — length-prefix (4-byte big-endian) sarma/çözme
- **Protokol** — JSON komut/cevap kodlama/çözme

## Komutlar

| Komut | Argümanlar | Yetki |
|-------|------------|-------|
| `ping` | yok | açık |
| `login` | `kullanici`, `sifre` | açık |
| `ogrenci_listele` | yok | login gerekli |
| `ogrenci_ekle` | `numara`, `ad`, `bolum` | login gerekli |

Demo hesap: `admin` / `1234`

## Derleme

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build .
```

## Çalıştırma

İki terminal:

```bash
# Terminal 1
./obys_sunucu 9200
```

```bash
# Terminal 2
./obys_istemci localhost 9200
ping
login admin 1234
listele
ekle 100003 Mehmet Bilgisayar
cikis
```

## Bölüm 13'e Hazırlık

Bu iskelet üzerine eklenecekler:
- Kalıcı veri tabanı (SQLite)
- Daha fazla komut: ders, not, dönem yönetimi
- Şifrelenmiş aktarım (TLS)
- Çoklu kullanıcı eşzamanlılığı (mutex / shared_mutex)
- Olay yayını (push notification)

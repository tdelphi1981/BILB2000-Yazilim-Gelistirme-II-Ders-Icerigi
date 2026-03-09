# BILB2000 - Yazılım Geliştirme II Ders İçeriği

**STL, Qt, Çok Kanallı ve Ağ Programlama** dersi için materyaller.

Karadeniz Teknik Üniversitesi | Fen Fakültesi, Bilgisayar Bilimleri | Bahar Dönemi

## Ders Bilgileri

| | |
|---|---|
| **Ders Kodu** | BILB2000 |
| **Süre** | 14 hafta |
| **Haftalık** | 2 saat teori + 2 saat uygulama |
| **Dil** | C++20 |

## İçerik

| Klasör | Açıklama |
|--------|----------|
| [`yg2_kitap.pdf`](yg2_kitap.pdf) | Ders kitabı |
| [`slides/`](slides/) | Haftalık ders slaytları |
| [`labs/`](labs/) | Lab föyleri (uygulamalı çalışma kağıtları) |
| [`LabExamples/`](LabExamples/) | Lab başlangıç kodları (.cpp dosyaları) |
| [`quizzes/`](quizzes/) | Haftalık quizler |
| [`worksheets/`](worksheets/) | Haftalık çalışma kağıtları |

## Haftalık Plan

| Hafta | Konu | Slayt | Lab | Quiz |
|-------|------|-------|-----|------|
| 1 | STL Kapsayıcılara Giriş | [Slayt](slides/Hafta01_STL_Kapsayicilara_Giris.pdf) | [Lab](labs/Lab01_STL_Kapsayicilara_Giris.pdf) | [Quiz](quizzes/Hafta01_Ogrenci.pdf) |
| 2 | İteratörler ve STL Algoritmaları | [Slayt](slides/Hafta02_Iteratorler_ve_STL_Algoritmalari.pdf) | [Lab](labs/Lab02_Iteratorler_ve_Algoritmalar.pdf) | [Quiz](quizzes/Hafta02_Ogrenci.pdf) |
| 3 | Modern C++ ve STL Uygulamaları | [Slayt](slides/Hafta03_Modern_Cpp_ve_STL_Uygulamalari.pdf) | [Lab](labs/Lab03_Modern_Cpp_ve_STL_Uygulamalari.pdf) | [Quiz](quizzes/Hafta03_Ogrenci.pdf) |
| 3.5 | ÖBYS Tam Proje | [Slayt](slides/Hafta03_5_OBYS_Tam_Proje.pdf) | [Lab](labs/Lab03_5_OBYS_Tam_Proje.pdf) | |
| 4 | Qt Framework'üne Giriş | [Slayt](slides/Hafta04_Qt_Frameworkune_Giris.pdf) | [Lab](labs/Lab04_Qt_Frameworkune_Giris.pdf) | [Quiz](quizzes/Hafta04_Ogrenci.pdf) |
| 5-14 | *İçerik hazırlanıyor* | | | |

## Lab Kodları

Lab başlangıç kodlarını [`LabExamples/`](LabExamples/) klasöründen indirin. Her dosyada `GOREV` işaretli bölgeler bulunur - lab föyündeki talimatlara göre bu görevleri tamamlayın.

```bash
# Tekil C++ dosyası derleme
g++ -std=c++20 -o calisma1 calisma1.cpp
./calisma1

# Qt projesi derleme (Lab04+)
cd calisma1
mkdir build && cd build
cmake ..
make
./calisma1
```

Detaylar için [`LabExamples/README.txt`](LabExamples/README.txt) dosyasına bakın.

## Lisans

Bu materyaller akademik kullanım için hazırlanmıştır. Detaylar için [LICENSE](LICENSE) dosyasına bakın.

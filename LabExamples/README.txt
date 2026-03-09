============================================================
  YAZILIM GELİŞTİRME II - Lab Başlangıç Kodları
  Fen Fakültesi, Bilgisayar Bilimleri
  BILB2000 - Bahar Dönemi
============================================================

Bu klasör, her hafta lab föylerinde kullanacağınız başlangıç
C++ dosyalarını içerir. Dosyaları indirip üzerinde çalışın.

------------------------------------------------------------
  DERLEME
------------------------------------------------------------

STL projeleri (Lab 01-03) C++20 standardıyla derlenir:

    g++ -std=c++20 -o calisma1 calisma1.cpp
    ./calisma1

macOS kullanıcıları:
    clang++ -std=c++20 -o calisma1 calisma1.cpp

Qt projeleri (Lab 04+) CMake ile derlenir:

    cd calisma1
    mkdir build && cd build
    cmake ..
    cmake --build .
    ./app

Qt yolunu belirtmeniz gerekebilir:
    cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/macos

------------------------------------------------------------
  NASIL KULLANILIR
------------------------------------------------------------

1. Lab föyünüzü açın (örneğin Lab01_STL_Kapsayicilara_Giris.pdf)
2. İlgili klasördeki .cpp dosyasını açın
3. Önce temel kodu derleyip çalıştırın (çıktıyı kontrol edin)
4. "GOREV" işaretli bölgeleri bulun
5. Lab föyündeki talimatlara göre GOREV'leri tamamlayın

------------------------------------------------------------
  GOREV İŞARETLERİ
------------------------------------------------------------

Her dosyada şu işaretleri bulacaksınız:

    // === GOREV X.Y - Görev Başlığı (süre dk) ===
    // TODO: a) Yapılacak işlem
    // TODO: b) Yapılacak işlem
    // ============================================

- GOREV numaraları lab föyündeki görev numaralarıyla eşleşiyor
- TODO satırları yapmanız gereken adımları belirtir
- Yorum içindeki örnek kodları referans olarak kullanabilirsiniz

------------------------------------------------------------
  KLASÖR YAPISI
------------------------------------------------------------

LabExamples/
  README.txt                             <- Bu dosya

  Lab01_STL_Kapsayicilar/
    calisma1.cpp  - vector ile Öğrenci Listesi
    calisma2.cpp  - deque ve Karşılaştırma
    calisma3.cpp  - map ile OBYS Prototipi
    calisma4.cpp  - unordered_map ve Uyarlayıcılar

  Lab02_Iteratorler_Algoritmalar/
    calisma1.cpp  - İteratör Kullanımı
    calisma2.cpp  - Sıralama ve Arama
    calisma3.cpp  - Transform ve Filtreleme
    calisma4.cpp  - Lambda ve İleri Algoritmalar

  Lab03_Modern_Cpp_ve_STL_Uygulamalari/
    calisma1.cpp  - Ranges ve Projeksiyonlar
    calisma2.cpp  - string_view ve std::format
    calisma3.cpp  - optional ve variant
    calisma4.cpp  - OBYS Entegrasyonu

  Lab04_Qt_Frameworkune_Giris/
    calisma1/                <- Qt projesi (CMake)
      CMakeLists.txt
      main.cpp  - İlk Qt Penceresi
    calisma2/
      CMakeLists.txt
      main.cpp  - Parent-Child ve QObject
    calisma3/
      CMakeLists.txt
      main.cpp  - Sinyal/Slot Bağlantıları
    calisma4/
      CMakeLists.txt
      main.cpp  - Widget'larla Form Oluşturma
    calisma5/
      CMakeLists.txt
      main.cpp  - OBYS Kayıt Formu
    calisma6/
      CMakeLists.txt
      main.cpp  - QString ve Qt Kapsayıcıları

------------------------------------------------------------
  TESLİM KONTROL LİSTESİ
------------------------------------------------------------

Her lab için teslim etmeden önce kontrol edin:

  [ ] Tüm GOREV bölgeleri tamamlandı
  [ ] Kod hatasız derleniyor (g++ -std=c++20 veya cmake)
  [ ] Program çalışıyor ve beklenen çıktıyı veriyor
  [ ] Yorum satırları ile gözlemleriniz eklendi
  [ ] Bonus görevler (varsa) eklendi

------------------------------------------------------------
  YARDIM
------------------------------------------------------------

- Derleme hatası: Hata mesajını dikkatlice okuyun, satır
  numarası ve hata türüyle eşleştirin
- Mantık hatası: Beklenen çıktı ile kendi çıktınızı
  karşılaştırın
- STL referansı: https://en.cppreference.com
- Qt referansı: https://doc.qt.io/qt-6/

============================================================

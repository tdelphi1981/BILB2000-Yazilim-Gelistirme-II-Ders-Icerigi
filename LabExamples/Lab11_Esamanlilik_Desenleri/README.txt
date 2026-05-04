Lab 11 - Eşzamanlılık Desenleri (Başlangıç Kodları)
====================================================

Bu dizin, Lab 11 çalışmaları için iskelet kod dosyalarını içerir.
Her dosyadaki "// GOREV" yorumlarını uygun kodla değiştirin.

İÇERİK
------
calisma1.cpp - ThreadSafeQueue<T> implementasyonu
calisma2.cpp - Sharded ConcurrentMap
calisma3.cpp - Paralel CSV içe aktarma + jthread + stop_token
calisma4.cpp - Atomic sayaç + ASCII progress bar
calisma5/    - Qt mini OBYS responsive (CMake projesi)

DERLEME (calisma1-4)
--------------------
g++ -std=c++20 -pthread -O2 -o calisma1 calisma1.cpp

Yarış tespiti için TSan ile derleme:
g++ -std=c++20 -O1 -g -fsanitize=thread -pthread -o calisma1_tsan calisma1.cpp

DERLEME (calisma5)
------------------
cd calisma5
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build .
./calisma5

KAYNAKLAR
---------
- Lab 11 föyü: Materials/Labs/Lab11_Esamanlilik_Desenleri.tex
- Cevap anahtarı (eğitimci): Lab11_Esamanlilik_Desenleri_Cevap.tex
- Tam OBYS responsive örneği: out/kod_ornekleri/bolum11/unite4/01_obys_responsive/

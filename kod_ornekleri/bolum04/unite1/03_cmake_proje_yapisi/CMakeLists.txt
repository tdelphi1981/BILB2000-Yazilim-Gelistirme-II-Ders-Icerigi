# CMake Proje Yapisi - Qt6 Uygulamasi
#
# Bu CMakeLists.txt dosyasi, Qt6 Widgets kullanan bir C++20
# uygulamasini yapilandirmak icin gereken tum adimlari icerir.
#
# Derleme adimlari:
#   mkdir build && cd build
#   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
#   cmake --build .
#
# macOS (Homebrew ile yuklu Qt):
#   cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)
#
# Linux:
#   cmake .. (Qt6 sistem genelinde yukluyse yol belirtmeye gerek yok)

# Minimum CMake surumu (Qt6 icin 3.16 gereklidir)
cmake_minimum_required(VERSION 3.16)

# Proje adi ve dili
project(cmake_proje_yapisi
    VERSION 1.0
    DESCRIPTION "OBYS - Qt6 CMake Ornek Projesi"
    LANGUAGES CXX
)

# C++20 standardini zorunlu kil
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt'nin otomatik araclarini etkinlestir:
#   AUTOMOC - Meta-Object Compiler (Q_OBJECT iceren siniflar icin)
#   AUTORCC - Resource Compiler (.qrc dosyalari icin)
#   AUTOUIC - User Interface Compiler (.ui dosyalari icin)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 Widgets modulunu bul
# REQUIRED: Bulunamazsa CMake hata verir
# COMPONENTS: Hangi Qt modullerinin gerekli oldugunu belirtir
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Qt6 standart proje yapilandirmasi
# (onerilen derleme ayarlarini otomatik uygular)
qt_standard_project_setup()

# Calistirilabilir dosyayi olustur
qt_add_executable(cmake_proje_yapisi
    main.cpp
)

# Qt6 Widgets kutuphanesini bagla
target_link_libraries(cmake_proje_yapisi
    PRIVATE Qt6::Widgets
)

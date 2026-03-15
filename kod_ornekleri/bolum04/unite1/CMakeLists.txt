# CMakeLists.txt - Bolum 04 Unite 1: Merhaba Qt
#
# Bu CMakeLists.txt dosyasi, 01_merhaba_qt.cpp ornegini
# Qt6 Widgets ile derlemek icin kullanilir.
#
# Derleme:
#   mkdir build && cd build
#   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
#   cmake --build .

cmake_minimum_required(VERSION 3.16)

project(merhaba_qt
    VERSION 1.0
    DESCRIPTION "Ilk Qt Uygulamasi"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

# 01_merhaba_qt
qt_add_executable(01_merhaba_qt
    01_merhaba_qt.cpp
)

target_link_libraries(01_merhaba_qt
    PRIVATE Qt6::Widgets
)

# 02_qt_modulleri
qt_add_executable(02_qt_modulleri
    02_qt_modulleri.cpp
)

target_link_libraries(02_qt_modulleri
    PRIVATE Qt6::Widgets
)

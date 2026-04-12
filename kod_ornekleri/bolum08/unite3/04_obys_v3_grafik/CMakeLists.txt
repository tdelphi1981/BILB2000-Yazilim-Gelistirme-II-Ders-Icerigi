# CMakeLists.txt -- OBYS v3 Grafik ve Istatistik projesi
#
# Bolum 8 - Qt ile OBYS GUI Tamamlama
# Unite 3 - Grafik ve Istatistik
#
# Unite 2'deki OBYS v2 Raporlama projesinin uzerine QTabWidget ile
# Grafik sekmeleri (bar chart + line chart) eklenmis surumdur.
# Qt6 Widgets + PrintSupport + Charts bilesenleri ve C++20 gerekir.
#
# Derleme:
#   cmake -B build
#   cmake --build build
#
# Calistirma:
#   ./build/obys_v3

cmake_minimum_required(VERSION 3.21)

project(obys_v3
    VERSION 3.0.0
    DESCRIPTION "OBYS v3 Grafik ve Istatistik Ornek Uygulamasi"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6: Widgets + PrintSupport + Charts (Charts yeni bilesen)
find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets PrintSupport Charts)

qt_standard_project_setup()

set(OBYS_SOURCES
    src/main.cpp
    src/main_window.cpp
    src/main_window.h
    src/not_giris_dialog.cpp
    src/not_giris_dialog.h
    src/persistence_manager.cpp
    src/persistence_manager.h
    src/transkript_uretici.cpp
    src/transkript_uretici.h
    src/ders_raporu.cpp
    src/ders_raporu.h
    src/notlar_model.cpp
    src/notlar_model.h
    src/istatistik_model.cpp
    src/istatistik_model.h
    src/grafik_paneli.cpp
    src/grafik_paneli.h
)

qt_add_executable(obys_v3 ${OBYS_SOURCES})

target_include_directories(obys_v3
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(obys_v3
    PRIVATE
        Qt6::Widgets
        Qt6::PrintSupport
        Qt6::Charts
)

if(MSVC)
    target_compile_options(obys_v3 PRIVATE /W4 /permissive-)
else()
    target_compile_options(obys_v3 PRIVATE
        -Wall -Wextra -Wpedantic)
endif()

# CMakeLists.txt -- OBYS v4 Final (Mini Proje Tamamlama)
#
# Bolum 8 - Qt ile OBYS GUI Tamamlama
# Unite 4 - OBYS GUI Mini Proje Tamamlama
#
# Unite 3'teki OBYS v3 Grafik projesinin uzerine su bilesenler eklendi:
#   - JsonExporter sinifi (Dosya > Disa Aktar > JSON)
#   - Son Acilanlar alt menusu (QSettings tabanli)
#   - Aydinlik / Karanlik tema (QRC + QSS)
#   - Genisletilmis klavye kisayollari
#   - Hakkinda diyalogu
#   - Pencere durumu ve oturum tercihleri QSettings ile saklanir
#
# Qt6 Widgets + PrintSupport + Charts bilesenleri ve C++20 gerekir.
#
# Derleme:
#   cmake -B build
#   cmake --build build
#
# Calistirma:
#   ./build/obys_v4

cmake_minimum_required(VERSION 3.21)

project(obys_v4
    VERSION 1.0.0
    DESCRIPTION "OBYS v4 Final Mini Proje Ornek Uygulamasi"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6: Widgets + PrintSupport + Charts
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
    src/json_exporter.cpp
    src/json_exporter.h
)

qt_add_executable(obys_v4 ${OBYS_SOURCES})

# QRC kaynaklari (temalar ve opsiyonel ikonlar)
qt_add_resources(obys_v4 "obys_res"
    PREFIX "/"
    FILES
        resources/themes/light.qss
        resources/themes/dark.qss
)

target_include_directories(obys_v4
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(obys_v4
    PRIVATE
        Qt6::Widgets
        Qt6::PrintSupport
        Qt6::Charts
)

if(MSVC)
    target_compile_options(obys_v4 PRIVATE /W4 /permissive-)
else()
    target_compile_options(obys_v4 PRIVATE
        -Wall -Wextra -Wpedantic)
endif()

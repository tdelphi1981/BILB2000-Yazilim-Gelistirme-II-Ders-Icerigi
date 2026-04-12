# CMakeLists.txt -- OBYS v2 Raporlama projesi
#
# Bolum 8 - Qt ile OBYS GUI Tamamlama
# Unite 2 - Raporlama ve Yazici
#
# Unite 1'deki OBYS v1 projesinin uzerine raporlama ve yazici
# desteginin eklendigi tam ornek. Qt6 Widgets + Qt6 PrintSupport
# bilesenleri ve C++20 gerekir.
#
# Derleme:
#   cmake -B build
#   cmake --build build
#
# Calistirma:
#   ./build/obys_v2

cmake_minimum_required(VERSION 3.21)

project(obys_v2
    VERSION 2.0.0
    DESCRIPTION "OBYS v2 Raporlama ve Yazici Ornek Uygulamasi"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6: Widgets + PrintSupport (yeni bilesen)
find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets PrintSupport)

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
)

qt_add_executable(obys_v2 ${OBYS_SOURCES})

target_include_directories(obys_v2
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(obys_v2
    PRIVATE
        Qt6::Widgets
        Qt6::PrintSupport
)

if(MSVC)
    target_compile_options(obys_v2 PRIVATE /W4 /permissive-)
else()
    target_compile_options(obys_v2 PRIVATE
        -Wall -Wextra -Wpedantic)
endif()

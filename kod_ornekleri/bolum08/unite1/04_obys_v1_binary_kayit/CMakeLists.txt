# CMakeLists.txt -- OBYS v1 Binary Kayit projesi
#
# Bolum 8 - Qt ile OBYS GUI Tamamlama
# Unite 1 - Ders Notu Giris Formu ve Oturum Kaydi
#
# Bagimsiz derlenebilir tam ornek. Qt6 Widgets ve C++20 gerekir.
#
# Derleme:
#   cmake -B build
#   cmake --build build
#
# Calistirma:
#   ./build/obys_v1

cmake_minimum_required(VERSION 3.21)

project(obys_v1
    VERSION 1.0.0
    DESCRIPTION "OBYS v1 Binary Kayit Ornek Uygulamasi"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 gereklidir (Qt6_DIR ayarlanmamissa sistem yoluna bakar)
find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

set(OBYS_SOURCES
    src/main.cpp
    src/main_window.cpp
    src/main_window.h
    src/not_giris_dialog.cpp
    src/not_giris_dialog.h
    src/persistence_manager.cpp
    src/persistence_manager.h
)

qt_add_executable(obys_v1 ${OBYS_SOURCES})

target_include_directories(obys_v1
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(obys_v1 PRIVATE Qt6::Widgets)

if(MSVC)
    target_compile_options(obys_v1 PRIVATE /W4 /permissive-)
else()
    target_compile_options(obys_v1 PRIVATE
        -Wall -Wextra -Wpedantic)
endif()

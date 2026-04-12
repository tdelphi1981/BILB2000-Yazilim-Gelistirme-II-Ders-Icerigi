cmake_minimum_required(VERSION 3.21)

project(obys_tutorial
    VERSION 1.0.0
    DESCRIPTION "OBYS - Ogrenci Bilgi Yonetim Sistemi"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Widgets PrintSupport Charts)

qt_standard_project_setup()

# Kaynak dosyalar (her asamada genisleyecek)
set(OBYS_SOURCES
    include/obys_domain.h
    include/obys_servis.h
    src/main.cpp
    src/main_window.cpp        src/main_window.h
    src/persistence_manager.cpp src/persistence_manager.h
    src/not_giris_dialog.cpp   src/not_giris_dialog.h
    src/notlar_model.cpp       src/notlar_model.h
    ui/ogrenci_ekle_dialog.ui
    ui/ders_ekle_dialog.ui
)

qt_add_executable(obys_tutorial ${OBYS_SOURCES})

set_property(TARGET obys_tutorial PROPERTY
    AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

qt_add_resources(obys_tutorial "obys_res"
    PREFIX "/"
    FILES
        resources/icons/file-plus.svg
        resources/icons/folder-open.svg
        resources/icons/save.svg
        resources/icons/download.svg
        resources/icons/printer.svg
        resources/icons/log-out.svg
        resources/icons/user-plus.svg
        resources/icons/book-plus.svg
        resources/icons/circle-plus.svg
        resources/icons/settings.svg
        resources/icons/help-circle.svg
        resources/icons/chart-bar.svg
        resources/icons/file-text.svg
        resources/icons/sun.svg
        resources/icons/moon.svg
)

target_include_directories(obys_tutorial
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(obys_tutorial
    PRIVATE
        Qt6::Widgets
        Qt6::PrintSupport
        Qt6::Charts
)

if(MSVC)
    target_compile_options(obys_tutorial PRIVATE /W4)
else()
    target_compile_options(obys_tutorial PRIVATE
        -Wall -Wextra -Wpedantic)
endif()

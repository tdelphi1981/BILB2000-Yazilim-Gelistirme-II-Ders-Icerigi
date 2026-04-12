# CMakeLists.txt -- OBYS v5 Release (Dagitilabilir Paket)
#
# Bolum 8 - Qt ile OBYS GUI Tamamlama
# Unite 5 - Kurulum Hazirlama ve Dagitilabilir Paket
#
# Unite 4'teki OBYS v4 Final projesi uzerine asagidaki bilesenler eklendi:
#   - CMakePresets.json ile debug/release profilleri
#   - Link Time Optimization (LTO) release modunda
#   - configure_file ile otomatik uretilen version.h
#   - LinguistTools ile Qt ceviri (i18n) iskeleti
#   - Varsayilan build tipi Release
#
# Qt6 Widgets + PrintSupport + Charts + LinguistTools bilesenleri ve
# C++20 gerekir.
#
# Derleme (preset ile):
#   cmake --preset release
#   cmake --build --preset release
#
# Calistirma:
#   ./build/release/obys_v4

cmake_minimum_required(VERSION 3.21)

project(obys_v4
    VERSION 1.0.0
    DESCRIPTION "OBYS v5 Release - Dagitilabilir Paket"
    LANGUAGES CXX)

# Varsayilan build tipi: Release (kullanici ustunden gecebilir)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Varsayilan derleme tipi" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Release'te LTO / IPO aktif
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if(ipo_supported AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "LTO / IPO aktif (Release)")
else()
    message(STATUS "LTO / IPO devre disi: ${ipo_error}")
endif()

# Qt6: Widgets + PrintSupport + Charts + LinguistTools
find_package(Qt6 6.5 REQUIRED COMPONENTS
    Widgets PrintSupport Charts LinguistTools)

qt_standard_project_setup()

# --- version.h uretimi ------------------------------------------------------
# src/version.h.in sablonundan build dizininde version.h olusturulur.
# @PROJECT_VERSION_*@ makrolari CMake tarafindan doldurulur.
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/version.h"
    @ONLY)

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

# QRC kaynaklari (temalar)
qt_add_resources(obys_v4 "obys_res"
    PREFIX "/"
    FILES
        resources/themes/light.qss
        resources/themes/dark.qss
)

# Qt ceviri dosyalari (i18n iskeleti)
set(OBYS_TS_FILES
    translations/obys_tr.ts
)
qt_add_translations(obys_v4 TS_FILES ${OBYS_TS_FILES})

target_include_directories(obys_v4
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}/generated
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

# macOS icin bundle ozellikleri (macdeployqt icin gerekli)
set_target_properties(obys_v4 PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "OBYS"
    MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
    MACOSX_BUNDLE_GUI_IDENTIFIER "tr.edu.ktu.obys"
    WIN32_EXECUTABLE TRUE
)

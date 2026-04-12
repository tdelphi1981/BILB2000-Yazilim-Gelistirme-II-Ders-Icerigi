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
    src/persistence_manager.cpp
    src/persistence_manager.h
)

qt_add_executable(obys_tutorial ${OBYS_SOURCES})

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

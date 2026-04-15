cmake_minimum_required(VERSION 3.20)

project(yurtsis
    VERSION 1.0.0
    DESCRIPTION "YURTSIS - Ogrenci Yurt Yonetim Sistemi"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Faz 1: Sadece STL (Qt yok)
include_directories(include)

add_executable(yurtsis src/main.cpp)

# --- Faz 2 icin: Asagidaki satirlarin basindaki # isaretlerini kaldirin ---
# set(CMAKE_AUTOMOC ON)
# find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets Charts)
# qt_standard_project_setup()
# target_link_libraries(yurtsis PRIVATE Qt6::Widgets Qt6::Charts)

if(MSVC)
    target_compile_options(yurtsis PRIVATE /W4 /permissive-)
else()
    target_compile_options(yurtsis PRIVATE -Wall -Wextra -Wpedantic)
endif()

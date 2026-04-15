cmake_minimum_required(VERSION 3.20)

project(sporsis
    VERSION 1.0.0
    DESCRIPTION "SPORSIS - Spor Salonu Uyelik Sistemi"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Faz 1: Sadece STL (Qt yok)
include_directories(include)

add_executable(sporsis src/main.cpp)

# --- Faz 2 icin: Asagidaki satirlarin basindaki # isaretlerini kaldirin ---
# set(CMAKE_AUTOMOC ON)
# find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets Charts)
# qt_standard_project_setup()
# target_link_libraries(sporsis PRIVATE Qt6::Widgets Qt6::Charts)

if(MSVC)
    target_compile_options(sporsis PRIVATE /W4 /permissive-)
else()
    target_compile_options(sporsis PRIVATE -Wall -Wextra -Wpedantic)
endif()

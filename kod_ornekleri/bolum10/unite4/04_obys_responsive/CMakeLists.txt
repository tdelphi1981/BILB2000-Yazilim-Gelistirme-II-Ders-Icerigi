# OBYS Responsive UI - Toplu Not Hesaplama - Qt 6
cmake_minimum_required(VERSION 3.16)
project(obys_responsive LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Concurrent)

add_executable(obys_responsive
    main.cpp
    rapor_penceresi.h
)

target_link_libraries(obys_responsive
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent)

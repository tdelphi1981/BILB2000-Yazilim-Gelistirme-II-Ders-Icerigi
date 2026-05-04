# Lab 12 - Çalışma 2: Qt TCP İstemci
cmake_minimum_required(VERSION 3.20)
project(calisma2_qt_istemci LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

add_executable(qt_istemci
    src/main.cpp
    src/Istemci.cpp
    include/Istemci.h
)

target_include_directories(qt_istemci PRIVATE include)
target_link_libraries(qt_istemci PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Widgets
)

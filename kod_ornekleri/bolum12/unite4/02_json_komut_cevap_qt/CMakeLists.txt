cmake_minimum_required(VERSION 3.20)
project(json_komut_cevap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(json_komut_cevap
    src/main.cpp
    src/Protokol.cpp
    include/Protokol.h
)

target_include_directories(json_komut_cevap PRIVATE include)

target_link_libraries(json_komut_cevap PRIVATE Qt6::Core)

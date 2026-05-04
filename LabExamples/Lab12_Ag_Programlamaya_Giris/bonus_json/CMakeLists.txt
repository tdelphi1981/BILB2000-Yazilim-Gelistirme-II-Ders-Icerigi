# Lab 12 - Bonus: JSON Komut Çözücü
cmake_minimum_required(VERSION 3.20)
project(bonus_json LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(bonus_json main.cpp)

target_link_libraries(bonus_json PRIVATE Qt6::Core)

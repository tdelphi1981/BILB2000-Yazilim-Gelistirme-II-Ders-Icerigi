cmake_minimum_required(VERSION 3.16)
project(lab10_calisma5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Concurrent)

add_executable(lab10_calisma5 main.cpp)

target_link_libraries(lab10_calisma5
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent)

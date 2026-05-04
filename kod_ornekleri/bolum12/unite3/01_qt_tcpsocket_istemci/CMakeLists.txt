cmake_minimum_required(VERSION 3.20)
project(qt_tcpsocket_istemci LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_executable(qt_tcpsocket_istemci
    src/main.cpp
    src/Istemci.cpp
    include/Istemci.h
)

target_include_directories(qt_tcpsocket_istemci PRIVATE include)

target_link_libraries(qt_tcpsocket_istemci PRIVATE
    Qt6::Core
    Qt6::Network
)

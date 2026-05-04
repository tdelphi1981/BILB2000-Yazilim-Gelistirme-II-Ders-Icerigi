cmake_minimum_required(VERSION 3.20)
project(qt_tcpserver_echo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_executable(qt_tcpserver_echo
    src/main.cpp
    src/EchoSunucu.cpp
    include/EchoSunucu.h
)

target_include_directories(qt_tcpserver_echo PRIVATE include)

target_link_libraries(qt_tcpserver_echo PRIVATE
    Qt6::Core
    Qt6::Network
)

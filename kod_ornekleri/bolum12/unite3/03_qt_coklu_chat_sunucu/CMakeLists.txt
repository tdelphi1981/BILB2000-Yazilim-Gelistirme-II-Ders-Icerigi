cmake_minimum_required(VERSION 3.20)
project(qt_coklu_chat_sunucu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_executable(qt_coklu_chat_sunucu
    src/main.cpp
    src/ChatSunucu.cpp
    include/ChatSunucu.h
)

target_include_directories(qt_coklu_chat_sunucu PRIVATE include)

target_link_libraries(qt_coklu_chat_sunucu PRIVATE
    Qt6::Core
    Qt6::Network
)

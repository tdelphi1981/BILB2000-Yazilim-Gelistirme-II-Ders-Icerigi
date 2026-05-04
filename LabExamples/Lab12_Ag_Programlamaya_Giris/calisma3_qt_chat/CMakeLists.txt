# Lab 12 - Çalışma 3: Qt TCP Chat (Length-Prefix Çerçeveleme)
cmake_minimum_required(VERSION 3.20)
project(calisma3_qt_chat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_executable(qt_chat_sunucu
    src/main.cpp
    src/ChatSunucu.cpp
    src/cerceveleme.cpp
    include/ChatSunucu.h
    include/cerceveleme.h
)

target_include_directories(qt_chat_sunucu PRIVATE include)
target_link_libraries(qt_chat_sunucu PRIVATE
    Qt6::Core
    Qt6::Network
)

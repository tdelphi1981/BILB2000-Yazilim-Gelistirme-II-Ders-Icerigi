# Lab 12 - Çalışma 1: POSIX TCP Echo Sunucu/İstemci
cmake_minimum_required(VERSION 3.20)
project(calisma1_posix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(echo_sunucu echo_sunucu.cpp)
add_executable(echo_istemci echo_istemci.cpp)

# POSIX soket API'si Linux/macOS'ta dogal olarak gelir; ek kutuphane
# gerekmez. Windows icin Winsock ws2_32 baglanmasi gerekir.
if(WIN32)
    target_link_libraries(echo_sunucu PRIVATE ws2_32)
    target_link_libraries(echo_istemci PRIVATE ws2_32)
endif()

cmake_minimum_required(VERSION 3.20)
project(Uygulama3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Ana calistirici
add_executable(Uygulama3
    src/main.cpp
    src/servisler/obys_servis.cpp
)

# Test calistirici
add_executable(TestUygulama3
    src/test_main.cpp
    src/servisler/obys_servis.cpp
)

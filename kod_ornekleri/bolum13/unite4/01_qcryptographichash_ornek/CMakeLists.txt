cmake_minimum_required(VERSION 3.16)
project(qcryptographichash_ornek LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(qcryptographichash_ornek
    main.cpp
)

target_link_libraries(qcryptographichash_ornek PRIVATE
    Qt6::Core
)

cmake_minimum_required(VERSION 3.20)
project(obys_protokol LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

# Ortak protokol katmanı (length-prefix + JSON)
add_library(obys_protokol_lib STATIC
    src/CerceveCozucu.cpp
    src/Protokol.cpp
    include/CerceveCozucu.h
    include/Protokol.h
)
target_include_directories(obys_protokol_lib PUBLIC include)
target_link_libraries(obys_protokol_lib PUBLIC Qt6::Core Qt6::Network)

# Sunucu binary
add_executable(obys_sunucu
    src/sunucu_main.cpp
    src/Sunucu.cpp
    include/Sunucu.h
)
target_link_libraries(obys_sunucu PRIVATE obys_protokol_lib)

# İstemci binary
add_executable(obys_istemci
    src/istemci_main.cpp
)
target_link_libraries(obys_istemci PRIVATE obys_protokol_lib)

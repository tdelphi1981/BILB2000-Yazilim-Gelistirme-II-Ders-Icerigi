cmake_minimum_required(VERSION 3.20)
project(obys_responsive LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent)

add_executable(obys_responsive
    src/main.cpp
    src/AnaPencere.cpp
    src/IceAktarmaServisi.cpp
    src/DisaAktarmaServisi.cpp
    src/HesaplamaServisi.cpp
    include/Ogrenci.h
    include/Depo.h
    include/AnaPencere.h
    include/IceAktarmaServisi.h
    include/DisaAktarmaServisi.h
    include/HesaplamaServisi.h
)

target_include_directories(obys_responsive PRIVATE include)

target_link_libraries(obys_responsive PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Concurrent
)

if(UNIX)
    target_link_libraries(obys_responsive PRIVATE pthread)
endif()

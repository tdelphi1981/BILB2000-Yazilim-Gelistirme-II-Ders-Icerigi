cmake_minimum_required(VERSION 3.20)
project(calisma5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(calisma5
    src/main.cpp
    src/AnaPencere.cpp
    src/Worker.cpp
    include/AnaPencere.h
    include/Worker.h
)

target_include_directories(calisma5 PRIVATE include)
target_link_libraries(calisma5 PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
if(UNIX)
    target_link_libraries(calisma5 PRIVATE pthread)
endif()

cmake_minimum_required(VERSION 3.20)
project(ss05_obys_splitter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.h mainwindow.cpp
    ogrencimodel.h ogrencimodel.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

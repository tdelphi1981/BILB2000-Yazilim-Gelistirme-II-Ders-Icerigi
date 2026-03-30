cmake_minimum_required(VERSION 3.16)
project(Lab07_Calisma1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(app
    main.cpp
    resources.qrc
)
target_link_libraries(app PRIVATE Qt6::Widgets)

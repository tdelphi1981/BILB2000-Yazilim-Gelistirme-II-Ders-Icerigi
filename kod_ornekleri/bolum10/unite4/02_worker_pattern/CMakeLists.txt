# Worker Pattern (moveToThread) Örneği - Qt 6
cmake_minimum_required(VERSION 3.16)
project(worker_pattern LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(worker_pattern
    main.cpp
    hesaplama_worker.h
    hesaplama_controller.h
    ana_pencere.h
)

target_link_libraries(worker_pattern PRIVATE Qt6::Core Qt6::Widgets)

cmake_minimum_required(VERSION 3.20)
project(bolum05_unite4 LANGUAGES CXX)

# C++20 standardı
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 otomatik araçları
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 Widgets modülünü bul
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Örnek 1: Designer ile UI dosyası kullanımı
add_executable(01_designer_ornek
    01_designer_ornek.cpp
    01_designer_ornek.ui
)
target_link_libraries(01_designer_ornek PRIVATE Qt6::Widgets)

# Örnek 2: OBYS Ana Pencere (header/source ayrımı)
add_executable(02_obys_ana_pencere
    02_obys_ana_pencere.cpp
    02_obys_ana_pencere.h
)
target_link_libraries(02_obys_ana_pencere PRIVATE Qt6::Widgets)

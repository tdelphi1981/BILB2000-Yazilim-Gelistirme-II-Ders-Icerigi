# Lab 12 - Ağ Programlamaya Giriş
# Üst düzey CMakeLists: tüm alt projeleri toplar.
# POSIX bölümü her zaman derlenir; Qt projeleri Qt6 bulunduğunda derlenir.

cmake_minimum_required(VERSION 3.20)
project(Lab12_Ag_Programlamaya_Giris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# POSIX bölüm her zaman var
add_subdirectory(calisma1_posix)

# Qt6 isteğe bağlı: bulunursa Qt projelerini de derle
find_package(Qt6 QUIET COMPONENTS Core Network Widgets)
if(Qt6_FOUND)
    message(STATUS "Qt6 bulundu: Qt projeleri de derlenecek")
    add_subdirectory(calisma2_qt_istemci)
    add_subdirectory(calisma3_qt_chat)
    add_subdirectory(bonus_json)
else()
    message(WARNING "Qt6 bulunamadi - sadece POSIX projesi derlenecek. "
                    "Qt projeleri icin: -DCMAKE_PREFIX_PATH=/path/to/Qt6")
endif()

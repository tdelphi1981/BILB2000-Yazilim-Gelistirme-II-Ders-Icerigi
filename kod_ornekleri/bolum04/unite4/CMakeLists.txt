# CMakeLists.txt - Bolum 04 Unite 4: OBYS Kayit Formu
#
# Bu CMakeLists.txt dosyasi, unite4 altindaki Qt projelerini
# derlemek icin kullanilir. Ozellikle 03_obys_kayit_formu.cpp
# gibi QFormLayout ve QMessageBox kullanan ornekler icindir.
#
# Derleme:
#   mkdir build && cd build
#   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x
#   cmake --build .

cmake_minimum_required(VERSION 3.16)

project(unite4_ornekleri
    VERSION 1.0
    DESCRIPTION "OBYS Kayit Formu ve Diger Widget Ornekleri"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

# 01_buton_etiket
qt_add_executable(01_buton_etiket
    01_buton_etiket.cpp
)
target_link_libraries(01_buton_etiket
    PRIVATE Qt6::Widgets
)

# 02_giris_widgetlari
qt_add_executable(02_giris_widgetlari
    02_giris_widgetlari.cpp
)
target_link_libraries(02_giris_widgetlari
    PRIVATE Qt6::Widgets
)

# 03_obys_kayit_formu
qt_add_executable(03_obys_kayit_formu
    03_obys_kayit_formu.cpp
)
target_link_libraries(03_obys_kayit_formu
    PRIVATE Qt6::Widgets
)

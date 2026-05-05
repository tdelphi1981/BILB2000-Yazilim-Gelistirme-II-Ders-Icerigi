add_executable(obys_sunucu
    main.cpp
    obys_sunucu.cpp
    obys_sunucu.h
)

target_link_libraries(obys_sunucu PRIVATE
    Qt6::Core
    Qt6::Network
)

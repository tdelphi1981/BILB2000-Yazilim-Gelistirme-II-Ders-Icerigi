add_executable(obys_istemci
    main.cpp
)

target_link_libraries(obys_istemci PRIVATE
    Qt6::Core
    Qt6::Network
)

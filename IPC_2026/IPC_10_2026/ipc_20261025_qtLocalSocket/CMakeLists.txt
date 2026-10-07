cmake_minimum_required(VERSION 3.16)

project(qtLocalSocket)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

qt_add_executable(qtLocalSocket
    main.cpp
)

target_link_libraries(qtLocalSocket
    PRIVATE
    Qt6::Core
    Qt6::Network
)





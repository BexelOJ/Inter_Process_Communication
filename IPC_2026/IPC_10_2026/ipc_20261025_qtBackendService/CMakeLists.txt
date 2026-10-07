cmake_minimum_required(VERSION 3.16)

project(qtBackendService)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

qt_add_executable(qtBackendService
    main.cpp
)

target_link_libraries(qtBackendService
    PRIVATE
    Qt6::Core
    Qt6::Network
)





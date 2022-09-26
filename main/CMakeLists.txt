cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        main.cpp
        Controller.cpp)

set(COMPONENTS
        indicator
        button
        bluetooth
        ecu)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})

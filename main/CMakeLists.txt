cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
#        main.c
        main.cpp
        )

set(COMPONENTS
        arduino
        nvs_flash
        ecu
        indicator
        test_button
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})

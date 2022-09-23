cmake_minimum_required(VERSION 3.17)

set(SOURCES
        Indicator.cpp)

set(COMPONENTS
        arduino)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

set(SOURCES
        Ecu.cpp
        utils.cpp
        Engine.cpp)

set(COMPONENTS arduino)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

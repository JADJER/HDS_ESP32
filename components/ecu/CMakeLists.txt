set(SOURCES
        Communication.cpp
        utils.cpp
        Ecu.cpp)

set(COMPONENTS arduino)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

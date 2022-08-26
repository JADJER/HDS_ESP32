set(SOURCES
        Ecu.cpp)

set(COMPONENTS
        connection)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

set(SOURCES
        Indicator.cpp)

set(COMPONENTS

        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

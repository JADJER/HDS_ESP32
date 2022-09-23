set(SOURCES
        sensors.c)

set(COMPONENTS
        bt
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

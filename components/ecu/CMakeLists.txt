set(SOURCES
        utils.c
        ecu.c)

set(COMPONENTS
        honda_protocol
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

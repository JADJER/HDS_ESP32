set(SOURCES
        main.c
        )

set(COMPONENTS
        nvs_flash
        ecu
        honda_protocol
        indicator
        mesh
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})

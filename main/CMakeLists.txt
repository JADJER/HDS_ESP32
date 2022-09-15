set(SOURCES
        main.c
        )

set(COMPONENTS
        nvs_flash
        ecu
        indicator
        mesh
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})

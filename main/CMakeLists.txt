set(SOURCES
        main.cpp
        )

set(COMPONENTS
        nvs_flash
        ecu
        honda_protocol
        indicator
        mesh
#        bluetooth
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})

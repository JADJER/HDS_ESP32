set(SOURCES
        button.c)

set(COMPONENTS
        driver
        freertos
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

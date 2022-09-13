set(SOURCES
        indicator.c)

set(COMPONENTS
        driver
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

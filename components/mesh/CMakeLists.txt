set(SOURCES
        mesh.c)

set(COMPONENTS
        bt
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

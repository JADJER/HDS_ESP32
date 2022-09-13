set(SOURCES
        Mesh.cpp
        MeshServer.cpp)

set(COMPONENTS
        bt
        )

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

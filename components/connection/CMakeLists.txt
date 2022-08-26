set(SOURCES
        Connection.cpp
        ConnectionFactory.cpp)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include)

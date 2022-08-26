set(SOURCES main.cpp)
set(COMPONENTS connection ecu)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})

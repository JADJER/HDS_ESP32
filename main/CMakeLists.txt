set(SOURCES main.cpp)
set(COMPONENTS ecu)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})

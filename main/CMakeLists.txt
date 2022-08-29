set(SOURCES
        main.cpp
        Controller.cpp)

set(COMPONENTS ecu ble indicator)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})

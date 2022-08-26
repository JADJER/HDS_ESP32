set(SOURCES
        main.cpp
        ecu.cpp
        utils.cpp)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS "include")

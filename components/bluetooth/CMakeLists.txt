cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        Bluetooth.cpp
        BluetoothClient.cpp
        BluetoothServer.cpp
        AdvertisedHdsDeviceCallbacks.cpp)

set(COMPONENTS arduino)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})

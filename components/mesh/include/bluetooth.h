//
// Created by jadjer on 15.09.22.
//

#pragma once

#include <esp_err.h>

esp_err_t bluetoothInit();

void bluetoothGetDeviceUUID(uint8_t* devUuid);

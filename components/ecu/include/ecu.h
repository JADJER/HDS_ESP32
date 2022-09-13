//
// Created by jadjer on 01.09.22.
//

#pragma once

#include "data/engine_data.h"
#include "data/error_data.h"
#include "data/sensors_data.h"
#include "data/unknown_data.h"
#include "data/vehicle_data.h"
#include <esp_err.h>

esp_err_t connect();

void detect_active_tables();
void test();

char* get_id();
vehicle_data_t get_vehicle_data();
engine_data_t get_engine_data();
sensors_data_t get_sensors_data();
error_data_t get_error_data();
unknown_data_t get_unknown_data();

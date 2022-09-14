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

/**
 * @brief
 * @return
 */
esp_err_t connectToEcu();

/**
 * @brief
 */
void detectActiveTables();

/**
 * @brief
 */
void test();

/**
 * @brief
 */
void updateAllData();

/**
 * @brief
 * @return
 */
char* getId();

/**
 * @brief
 * @return
 */
VehicleData_t getVehicleData();

/**
 * @brief
 * @return
 */
EngineData_t getEngineData();

/**
 * @brief
 * @return
 */
SensorsData_t getSensorsData();

/**
 * @brief
 * @return
 */
ErrorData_t getErrorData();

/**
 * @brief
 * @return
 */
UnknownData_t getUnknownData();

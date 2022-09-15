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
esp_err_t ECU_connect();

/**
 * @brief
 */
void ECU_detectAllTables();

/**
 * @brief
 */
void ECU_detectActiveTables();

/**
 * @brief
 */
void ECU_updateAllData();

/**
 * @brief
 * @return
 */
char* ECU_getId();

/**
 * @brief
 * @return
 */
VehicleData_t ECU_getVehicleData();

/**
 * @brief
 * @return
 */
EngineData_t ECU_getEngineData();

/**
 * @brief
 * @return
 */
SensorsData_t ECU_getSensorsData();

/**
 * @brief
 * @return
 */
ErrorData_t ECU_getErrorData();

/**
 * @brief
 * @return
 */
UnknownData_t ECU_getUnknownData();

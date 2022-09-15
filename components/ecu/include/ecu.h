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
esp_err_t ecuConnect();

/**
 * @brief
 */
void ecuDetectAllTables();

/**
 * @brief
 */
void ecuDetectActiveTables();

/**
 * @brief
 */
void ecuUpdateAllData();

/**
 * @brief
 * @return
 */
char* ecuGetId();

/**
 * @brief
 * @return
 */
VehicleData_t ecuGetVehicleData();

/**
 * @brief
 * @return
 */
EngineData_t ecuGetEngineData();

/**
 * @brief
 * @return
 */
SensorsData_t ecuGetSensorsData();

/**
 * @brief
 * @return
 */
ErrorData_t ecuGetErrorData();

/**
 * @brief
 * @return
 */
UnknownData_t ecuGetUnknownData();

//
// Created by jadjer on 21.09.22.
//

#pragma once

#include "data/engine_data.h"
#include "data/error_data.h"
#include "data/sensors_data.h"
#include "data/unknown_data.h"
#include "data/vehicle_data.h"
#include <esp_ble_mesh_sensor_model_api.h>

extern esp_ble_mesh_sensor_state_t sensors[3];

/**
 * @brief
 * @param data
 */
void sensorsUpdateVehicleData(VehicleData_t* data);

/**
 * @brief
 * @param data
 */
void sensorsUpdateEngineData(EngineData_t* data);

/**
 * @brief
 * @param data
 */
void sensorsUpdateSensorsData(SensorsData_t * data);

/**
 * @brief
 * @param data
 */
void sensorsUpdateErrorData(ErrorData_t* data);

/**
 * @brief
 * @param data
 */
void sensorsUpdateUnknownData(UnknownData_t* data);
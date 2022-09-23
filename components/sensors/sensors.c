//
// Created by jadjer on 21.09.22.
//

#include "sensors.h"

#include <esp_ble_mesh_sensor_model_api.h>

#define SENSOR_ADD(_id, _data)                                               \
  {                                                                                 \
    .sensor_property_id = _id,                                                      \
    .descriptor.positive_tolerance = ESP_BLE_MESH_SENSOR_UNSPECIFIED_POS_TOLERANCE, \
    .descriptor.negative_tolerance = ESP_BLE_MESH_SENSOR_UNSPECIFIED_NEG_TOLERANCE, \
    .descriptor.sampling_function = ESP_BLE_MESH_SAMPLE_FUNC_UNSPECIFIED,           \
    .descriptor.measure_period = ESP_BLE_MESH_SENSOR_NOT_APPL_MEASURE_PERIOD,       \
    .descriptor.update_interval = ESP_BLE_MESH_SENSOR_NOT_APPL_UPDATE_INTERVAL,     \
    .sensor_data.format = ESP_BLE_MESH_SENSOR_DATA_FORMAT_A,                        \
    .sensor_data.length = 0,                                                        \
    .sensor_data.raw_value = _data,                                                 \
  }

NET_BUF_SIMPLE_DEFINE_STATIC(rpm, 2);
NET_BUF_SIMPLE_DEFINE_STATIC(speed, 1);
NET_BUF_SIMPLE_DEFINE_STATIC(tpsPercent, 1);

esp_ble_mesh_sensor_state_t sensors[] = {
    SENSOR_ADD(0x0051, &rpm),
    SENSOR_ADD(0x0056, &speed),
    SENSOR_ADD(0x005B, &tpsPercent),
};

void sensorsUpdateVehicleData(VehicleData_t* data) {
  net_buf_simple_add_u8(&speed, data->speed);
}

void sensorsUpdateEngineData(EngineData_t* data) {
  net_buf_simple_add_u8(&rpm, data->rpm);
}

void sensorsUpdateSensorsData(SensorsData_t* data) {
  net_buf_simple_add_le16(&tpsPercent, data->tpsPercent);
}

void sensorsUpdateErrorData(ErrorData_t* data) {
}

void sensorsUpdateUnknownData(UnknownData_t* data) {
}

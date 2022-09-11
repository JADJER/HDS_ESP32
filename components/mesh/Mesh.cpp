//
// Created by jadjer on 08.09.22.
//

#include "Mesh.hpp"

#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_sensor_model_api.h"

Mesh::Mesh() = default;

Mesh::~Mesh() = default;

esp_err_t Mesh::init() {
  esp_err_t err;

  esp_ble_mesh_register_prov_callback(example_ble_mesh_provisioning_cb);
  esp_ble_mesh_register_config_server_callback(example_ble_mesh_config_server_cb);
  esp_ble_mesh_register_sensor_server_callback(example_ble_mesh_sensor_server_cb);

  err = esp_ble_mesh_init(&provision, &composition);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize mesh stack");
    return err;
  }

  err = esp_ble_mesh_node_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to enable mesh node");
    return err;
  }

  board_led_operation(LED_G, LED_ON);

  ESP_LOGI(TAG, "BLE Mesh sensor server initialized");

  return ESP_OK;
}

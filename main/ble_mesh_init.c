
/*
 * Copyright (c) 2017 Intel Corporation
 * Additional Copyright (c) 2020 Espressif Systems (Shanghai) PTE LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sdkconfig.h>
#include <stdio.h>
#include <string.h>

#include "esp_bt.h"
#include "esp_bt_device.h"
#include "esp_bt_main.h"

#include "esp_ble_mesh_defs.h"

#define TAG "EXAMPLE_INIT"

void ble_mesh_get_dev_uuid(uint8_t* dev_uuid) {
  if (dev_uuid == NULL) {
    ESP_LOGE(TAG, "%s, Invalid device uuid", __func__);
    return;
  }

  /* Copy device address to the device uuid with offset equals to 2 here.
     * The first two bytes is used for matching device uuid by Provisioner.
     * And using device address here is to avoid using the same device uuid
     * by different unprovisioned devices.
     */
  memcpy(dev_uuid + 2, esp_bt_dev_get_address(), BD_ADDR_LEN);
}

esp_err_t ble_mesh_init(void) {
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

  //  board_led_operation(LED_G, LED_ON);

  ESP_LOGI(TAG, "BLE Mesh sensor server initialized");

  return ESP_OK;
}

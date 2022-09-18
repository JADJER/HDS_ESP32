//
// Created by jadjer on 15.09.22.
//

#include "bluetooth.h"
#include <esp_ble_mesh_defs.h>
#include <esp_bt.h>
#include <esp_bt_device.h>
#include <esp_bt_main.h>
#include <esp_err.h>
#include <esp_log.h>
#include <string.h>

#define TAG "Bluetooth"

esp_err_t bluetoothInit() {

  esp_err_t ret;

  ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  ret = esp_bt_controller_init(&bt_cfg);
  if (ret) {
    ESP_LOGE(TAG, "%s initialize controller failed", __func__);
    return ret;
  }

  ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
  if (ret) {
    ESP_LOGE(TAG, "%s enable controller failed", __func__);
    return ret;
  }
  ret = esp_bluedroid_init();
  if (ret) {
    ESP_LOGE(TAG, "%s init bluetooth failed", __func__);
    return ret;
  }
  ret = esp_bluedroid_enable();
  if (ret) {
    ESP_LOGE(TAG, "%s enable bluetooth failed", __func__);
    return ret;
  }

  return ret;
}

void bluetoothGetDeviceUUID(uint8_t* devUuid) {
  if (devUuid == NULL) {
    ESP_LOGE(TAG, "%s, Invalid device uuid", __func__);
    return;
  }

  /* Copy device address to the device uuid with offset equals to 2 here.
     * The first two bytes is used for matching device uuid by Provisioner.
     * And using device address here is to avoid using the same device uuid
     * by different unprovisioned devices.
     */
  memcpy(devUuid + 2, esp_bt_dev_get_address(), BD_ADDR_LEN);
}

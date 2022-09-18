//
// Created by jadjer on 15.09.22.
//

#include "bluetooth.h"
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_err.h>
#include <esp_log.h>

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

#include "bluetooth.h"
#include "button.h"
#include "ecu.h"
#include "indicator.h"
#include "mesh.h"

#include <esp_err.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>

#define TAG "HDS"

void testButton() {
  indicatorBlink(500);
  ecuDetectAllTables();
  indicatorEnable();
}

void app_main() {
  esp_err_t err;

  ESP_LOGI(TAG, "Initializing...");

  err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  err = indicatorInit();
  if (err) {
    ESP_LOGE(TAG, "Indicator init failed (err %d)", err);
    return;
  }

  indicatorBlink(100);

  err = ecuConnect();
  if (err) {
    ESP_LOGE(TAG, "Connect failed to Honda ECU (err %d)", err);

//    if (err == ESP_ERR_INVALID_RESPONSE) { indicatorBlinkErrorCode(1); }
//    if (err == ESP_ERR_INVALID_SIZE) { indicatorBlinkErrorCode(2); }
//    if (err == ESP_ERR_INVALID_ARG) { indicatorBlinkErrorCode(3); }
//    if (err == ESP_ERR_INVALID_CRC) { indicatorBlinkErrorCode(4); }
//
//    return;
  }

  err = bluetoothInit();
  if (err) {
    ESP_LOGE(TAG, "Bluetooth init failed (err %d)", err);
    indicatorBlinkErrorCode(5);
    return;
  }

  err = bluetoothMeshInit();
  if (err) {
    ESP_LOGE(TAG, "Bluetooth mesh init failed (err %d)", err);
    indicatorBlinkErrorCode(6);
    return;
  }

  indicatorBlink(500);
  ecuDetectActiveTables();
  indicatorEnable();

  ESP_LOGI(TAG, "Minimum free heap size: %d bytes", esp_get_minimum_free_heap_size());

  err = buttonInit();
  if (err) {
    ESP_LOGE(TAG, "Test button init failed (err %d)", err);
    indicatorBlinkErrorCode(7);
  }

  buttonSetInterruptCallback(testButton);
}

#include "bluetooth.h"
#include "ecu.h"
#include "indicator.h"
#include "mesh.h"
#include <esp_err.h>
#include <esp_log.h>
#include <esp_system.h>

#define TAG "HDS"

void app_main() {
  esp_err_t err;

  ESP_LOGI(TAG, "Initializing...");

  err = indicatorInit();
  if (err) {
    ESP_LOGE(TAG, "Indicator init failed (err %d)", err);
    return;
  }

  indicatorBlink(100);

  err = ecuConnect();
  if (err) {
    ESP_LOGE(TAG, "Connect failed to Honda ECU (err %d)", err);

    if (err == ESP_ERR_INVALID_RESPONSE) { indicatorBlinkErrorCode(1); }
    if (err == ESP_ERR_INVALID_SIZE) { indicatorBlinkErrorCode(2); }
    if (err == ESP_ERR_INVALID_ARG) { indicatorBlinkErrorCode(3); }
    if (err == ESP_ERR_INVALID_CRC) { indicatorBlinkErrorCode(4); }
    return;
  }

  indicatorBlink(500);

  ecuDetectActiveTables();

  indicatorBlink(2000);

  err = bluetoothInit();
  if (err) {
    ESP_LOGE(TAG, "Bluetooth init failed (err %d)", err);
    indicatorBlinkErrorCode(5);
    return;
  }

  err = meshInit();
  if (err) {
    ESP_LOGE(TAG, "Mesh init failed (err %d)", err);
    indicatorBlinkErrorCode(6);
    return;
  }

  indicatorEnable();

  //  ble_mesh_get_dev_uuid(deviceUuid);

  ESP_LOGI(TAG, "Minimum free heap size: %d bytes", esp_get_minimum_free_heap_size());
}

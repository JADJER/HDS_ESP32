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
    indicatorBlinkErrorCode(1);
    return;
  }

  indicatorBlink(500);

  ecuDetectActiveTables();

  indicatorBlink(2000);

  err = bluetoothInit();
  if (err) {
    ESP_LOGE(TAG, "Bluetooth init failed (err %d)", err);
    indicatorBlinkErrorCode(2);
    return;
  }

  err = meshInit();
  if (err) {
    ESP_LOGE(TAG, "Mesh init failed (err %d)", err);
    indicatorBlinkErrorCode(3);
    return;
  }

  indicatorEnable();

  //  ble_mesh_get_dev_uuid(deviceUuid);

  ESP_LOGI(TAG, "Minimum free heap size: %d bytes", esp_get_minimum_free_heap_size());
}

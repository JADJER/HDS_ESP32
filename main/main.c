#include "ecu.h"
#include "indicator.h"
//#include "mesh.h"
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

  err = ECU_connect();
  if (err) {
    ESP_LOGE(TAG, "Connect failed to Honda ECU (err %d)", err);
    indicatorDisable();
    return;
  }

  indicatorBlink(500);

  ECU_detectAllTables();
  ECU_detectActiveTables();

  indicatorBlink(2000);

  //  ble_mesh_get_dev_uuid(deviceUuid);

  ESP_LOGI(TAG, "Minimum free heap size: %d bytes", esp_get_minimum_free_heap_size());
}

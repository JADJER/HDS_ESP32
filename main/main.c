#include "ecu.h"
#include "indicator.h"
#include "mesh.h"
#include "protocol.h"
#include <esp_err.h>
#include <esp_log.h>

#define TAG "HDS"

void app_main() {
  esp_err_t err;

  ESP_LOGI(TAG, "Initializing...");

  //  ble_mesh_get_dev_uuid(deviceUuid);
}

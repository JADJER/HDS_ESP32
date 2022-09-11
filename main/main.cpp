#include <nvs_flash.h>
#include "Ecu.hpp"
#include "Protocol.hpp"
#include "Indicator.hpp"

extern "C" void app_main() {
  esp_err_t err;

  log_d("Initializing...");

  err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  //  err = bluetooth.init();
  //  if (err) {
  //    ESP_LOGE(TAG, "esp32_bluetooth_init failed (err %d)", err);
  //    return;
  //  }

  Indicator indicator(2);
  Protocol protocol(16, 17);
  Ecu ecu(protocol);

  indicator.blink(500);

  ecu.connect();

  indicator.blink(100);

  ecu.detectActiveTables();
  ecu.test();

  indicator.blink(2000);

  //  ble_mesh_get_dev_uuid(deviceUuid);
  //
  //  /* Initialize the Bluetooth Mesh Subsystem */
  //  err = ble_mesh_init();
  //  if (err) {
  //    ESP_LOGE(TAG, "Bluetooth mesh init failed (err %d)", err);
  //  }
}

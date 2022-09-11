#include "Ecu.hpp"
#include "Indicator.hpp"
#include "Mesh.hpp"
#include "Protocol.hpp"
#include <BLEDevice.h>

extern "C" void app_main() {
  esp_err_t err;

  log_d("Initializing...");

  BLEDevice::init("ECU");

  Indicator indicator(2);
  Protocol protocol(16, 17);
  Ecu ecu(protocol);
  Mesh mesh;

  err = mesh.init();
  if (err) {
    log_d("Bluetooth mesh init failed (err %d)", err);
  }

  indicator.blink(500);

  ecu.connect();

  indicator.blink(100);

  ecu.detectActiveTables();
  ecu.test();

  indicator.blink(2000);

  //  ble_mesh_get_dev_uuid(deviceUuid);
}

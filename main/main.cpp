//
// Created by jadjer on 24.08.22.
//

#include <Arduino.h>
#include "ecu.hpp"

extern "C" void app_main(void) {
  auto ecu = ECU();
  ecu.setup();

  delay(50);

  while (true) {
    auto table11 = ecu.showDataTable11();
    auto tableD1 = ecu.showDataTableD1();

    if (ecu.isConnected()) {
      ESP_LOGI("HDS", "RPM: %d;", table11.rpm);
      ESP_LOGI("HDS", "Ect temp: %f;", table11.ectTemp);
      ESP_LOGI("HDS", "Battery volt: %f;", table11.battVolts);
      ESP_LOGI("HDS", "TPS: %d;", table11.tpsPercent);
      ESP_LOGI("HDS", "Iat temp: %f;", table11.iatTemp);
      ESP_LOGI("HDS", "Speed: %d;", table11.speedKPH);
      ESP_LOGI("HDS", "Engine state: %d;", tableD1.engState);
      ESP_LOGI("HDS", "Side stand state: %d;", tableD1.switchState);
    }
  }
}

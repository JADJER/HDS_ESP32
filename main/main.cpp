// Copyright 2022 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by jadjer on 23.09.22.
//

#include "Button.hpp"
#include "ECU.hpp"
#include "Indicator.hpp"
#include "Protocol.hpp"
#include <Arduino.h>

#define TAG "HDS"

Indicator indicator;
Protocol protocol;
ECU ecu(&protocol);
Button button;

extern "C" void app_main() {
  esp_err_t err;

  ESP_LOGI(TAG, "Initializing...");

  err = indicator.init();
  if (err) {
    ESP_LOGE(TAG, "Indicator init failed (err %d)", err);
    return;
  }

  indicator.blink(100);

  err = ecu.connect();
  if (err) {
    ESP_LOGE(TAG, "Connect failed to Honda ECU (err %d)", err);

    if (err == ESP_ERR_INVALID_RESPONSE) { indicator.blinkErrorCode(1); }
    if (err == ESP_ERR_INVALID_SIZE) { indicator.blinkErrorCode(2); }
    if (err == ESP_ERR_INVALID_ARG) { indicator.blinkErrorCode(3); }
    if (err == ESP_ERR_INVALID_CRC) { indicator.blinkErrorCode(4); }

    return;
  }

  indicator.blink(500);

  ecu.detectActiveTables();

  indicator.enable();

  err = button.init();
  if (err) {
    ESP_LOGE(TAG, "Test button init failed (err %d)", err);
    indicator.blinkErrorCode(7);
  }

  while (true) {
    if (button.isPressed()) {
      indicator.blink(500);
      ecu.detectAllTables();
      indicator.enable();
      button.resetState();
    }
  }
}

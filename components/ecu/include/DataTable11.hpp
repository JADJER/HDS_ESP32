//
// Created by jadjer on 30.08.22.
//

#pragma once

#include <Arduino.h>

struct DataTable11 {
  uint16_t rpm;
  uint8_t speed;

  float tpsPercent;
  float tpsVolts;

  float ectTemp;
  float ectVolts;

  float iatTemp;
  float iatVolts;

  float batteryVolts;

  float mapPressure;
  float mapVolts;
};

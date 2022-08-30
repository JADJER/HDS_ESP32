//
// Created by jadjer on 30.08.22.
//

#pragma once

#include <Arduino.h>

struct DataTable11 {
  uint16_t rpm;
  uint8_t speedKPH;

  uint8_t tpsPercent;
  uint8_t tpsVolts;

  float ectTemp;
  uint8_t ectVolts;

  float iatTemp;
  uint8_t iatVolts;

  float batteryVolts;

  float mapPressure;
  uint8_t mapVolts;
};

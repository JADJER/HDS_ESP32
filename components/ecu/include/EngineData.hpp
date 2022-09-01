//
// Created by jadjer on 01.09.22.
//

#pragma once

#include <Arduino.h>

struct EngineData {
  int rpm;
  int speed;

  int tpsPercent;
  float tpsVolts;

  float ectTemp;
  float ectVolts;

  float iatTemp;
  float iatVolts;

  float batteryVolts;

  float mapPressure;
  float mapVolts;
};

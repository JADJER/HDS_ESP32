//
// Created by jadjer on 01.09.22.
//

#pragma once

#include <Arduino.h>
#include <string>

struct EngineData {
  std::string id;

  int rpm;
  int speed;
  float batteryVolts;

  float tpsPercent;
  float tpsVolts;

  int ectTemp;
  float ectVolts;

  int iatTemp;
  float iatVolts;

  float mapPressure;
  float mapVolts;

  int fuelInject;
  float ignitionAngle;

  int unk_data_1;
  int unk_data_2;
  int unk_data_3;
};

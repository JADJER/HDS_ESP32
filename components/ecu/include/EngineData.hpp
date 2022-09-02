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

  int tpsPercent;
  float tpsVolts;

  float ectTemp;
  float ectVolts;

  float iatTemp;
  float iatVolts;

  float mapPressure;
  float mapVolts;

  int unk_data_1;
  float unk_data_2;
  float unk_data_3;
  float unk_data_4;
  float unk_data_5;
};

//
// Created by jadjer on 07.09.22.
//

#pragma once

typedef struct {
  float tpsPercent;
  float tpsVolts;
  int ectTemp;
  float ectVolts;
  int iatTemp;
  float iatVolts;
  float mapPressure;
  float mapVolts;
} SensorsData_t;

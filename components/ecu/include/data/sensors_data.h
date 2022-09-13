//
// Created by jadjer on 07.09.22.
//

#pragma once

typedef struct {
  float tps_percent;
  float tps_volts;
  int ect_temp;
  float ect_volts;
  int iat_temp;
  float iat_volts;
  float map_pressure;
  float map_volts;
} sensors_data_t;

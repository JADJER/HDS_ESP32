//
// Created by jadjer on 24.08.22.
//

#include <Arduino.h>

#include "utils.hpp"

float calcValueDivide256(float val) {
  //convert to dec, multiple by 5, then divide result by 256
  //used for TPS Volt, ECT Volt, IAT Volt, MAP Volt
  return val * 5 / 256;
}

int calcValueMinus40(int val) {
  //value minus 40
  //used for ECT Temp, IAT Temp
  return val - 40;
}

float calcValueDivide10(float val) {
  //value divided by 10
  //used for Batt Volt
  return val / 10;
}

float calcValueDivide16(float val) {
  //value divided by 16 and times 10
  //used for TPS%
  return val / 16 * 10;
}

uint16_t calcChecksum(uint8_t const* data, size_t len) {
  uint16_t checkSum = 0;

  for (size_t i = 0; i < len; i++) {
    checkSum -= data[i];
  }

  return checkSum;
}

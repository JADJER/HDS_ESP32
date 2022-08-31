//
// Created by jadjer on 24.08.22.
//

#include <Arduino.h>

#include "utils.hpp"

float calcValueDivide256(int val) {
  //convert to dec, multiple by 5, then divide result by 256
  //used for TPS Volt, ECT Volt, IAT Volt, MAP Volt
  return (val * 5) / 256;
}

float calcValueMinus40(int val) {
  //value minus 40
  //used for ECT Temp, IAT Temp
  return val - 40;
}

float calcValueDivide10(int val) {
  //value divided by 10
  //used for Batt Volt
  return val / 10;
}

float calcValueDivide16(int val) {
  //value divided by 16 and times 10
  //used for TPS%
  return (val / 16) * 10;
}

uint8_t calcChecksum(uint8_t const* data, size_t len) {
  uint8_t checkSum = 0;

  for (size_t i = 0; i < len; i++) {
    checkSum -= data[i];
  }

  return checkSum;
}

uint8_t calcChecksum(uint8_t const* data, size_t start, size_t stop) {
  uint8_t checkSum = 0;

  for (size_t i = start; i < stop; i++) {
    checkSum -= data[i];
  }

  return checkSum;
}

float calcKPHtoMPH(int val) {
  return val / 1.609344;
}

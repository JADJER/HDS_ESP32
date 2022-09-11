//
// Created by jadjer on 24.08.22.
//

#include "utils.hpp"

uint16_t calcChecksum(uint8_t const* data, size_t len) {
  uint16_t checkSum = 0;

  for (size_t i = 0; i < len; i++) {
    checkSum -= data[i];
  }

  return checkSum;
}

//
// Created by jadjer on 31.08.22.
//

#pragma once

#include <stdlib.h>

typedef struct {
  uint8_t code;
  uint8_t command;
  uint8_t checksum;
  uint8_t* data;
  size_t len;
} CommandResult_t;

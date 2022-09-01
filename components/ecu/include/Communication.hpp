//
// Created by jadjer on 01.09.22.
//

#pragma once

#include <Arduino.h>
#include "CommandResult.hpp"

class Communication {
 public:
  virtual ~Communication() = default;

 private:
  virtual bool initialize() const = 0;

 private:
  virtual CommandResult* readData(bool print) const = 0;
  virtual void writeData(uint8_t const* data, size_t len, bool print) const = 0;
};

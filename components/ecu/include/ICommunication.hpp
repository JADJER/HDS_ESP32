//
// Created by jadjer on 01.09.22.
//

#pragma once

#include "CommandResult.hpp"
#include <Arduino.h>

class ICommunication {
 public:
  virtual ~ICommunication() = default;

 public:
  virtual CommandResult* readData() const = 0;
  virtual void writeData(uint8_t const* data, size_t len) const = 0;

 private:
  virtual bool initialize() const = 0;
};

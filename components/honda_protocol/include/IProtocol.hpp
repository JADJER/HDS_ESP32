//
// Created by jadjer on 01.09.22.
//

#pragma once

#include "CommandResult.hpp"

class IProtocol {
 public:
  virtual ~IProtocol() = default;

 public:
  virtual bool connect() = 0;

 public:
  virtual CommandResult* readData() = 0;
  virtual void writeData(uint8_t const* data, size_t len) = 0;

 private:
  virtual bool initialize() = 0;
};

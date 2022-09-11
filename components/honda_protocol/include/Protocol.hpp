//
// Created by jadjer on 24.08.22.
//

#pragma once

#include "CommandResult.hpp"
#include "IProtocol.hpp"
#include <HardwareSerial.h>
#include <string>

class Protocol : public IProtocol {
 public:
  Protocol(int8_t rx, int8_t tx);
  ~Protocol() override;

 public:
  bool connect() override;

 public:
  CommandResult* readData() override;
  void writeData(uint8_t const* data, size_t len) override;

 private:
  int8_t m_rx;
  int8_t m_tx;
  std::string m_tag;
  size_t m_bufferMaxSize;
  HardwareSerial m_serial;

 private:
  void wakeup() const;
  bool initialize() override;
};

//
// Created by jadjer on 26.08.22.
//

#pragma once

#include "driver/uart.h"
#include "IConnection.hpp"
#include <vector>

class Connection : public IConnection {
 public:
  Connection();
  ~Connection() override = default;

 public:
  std::vector<uint8_t> readData(std::string const& logName) override;
  int writeString(std::string const& logName, std::string const& data) override;
  int writeData(std::string const& logName, std::vector<uint8_t> const& data) override;

 private:
  int m_rx;
  int m_tx;
  int m_buffSize;
  uart_config_t m_uartConfig;

 private:
  bool setPinConfig() const;
};

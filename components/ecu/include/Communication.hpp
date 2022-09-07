//
// Created by jadjer on 24.08.22.
//

#pragma once

#include "CommandResult.hpp"
#include "ICommunication.hpp"

class Communication : public ICommunication {
 public:
  Communication(int rx_pin, int tx_pin);
  ~Communication() override;

 public:
  bool connect() const;

 public:
  CommandResult* readData() const override;
  void writeData(uint8_t const* data, size_t len) const override;

 private:
  int m_rx;
  int m_tx;
  size_t m_bufferMaxSize;

 private:
  void wakeup() const;
  bool initialize() const override;
};

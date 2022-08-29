//
// Created by jadjer on 24.08.22.
//

#pragma once

#include <memory>
#include <vector>

class ECU {
 public:
  ECU(int rx_pin, int tx_pin);
  ~ECU();

 public:
  bool connect() const;
  void test() const;

 public:
  int getTest();

 private:
  int m_i;
  int m_rx;
  int m_tx;

 private:
  void wakeup() const;
  bool initialize() const;

 private:
  std::vector<int> readData() const;
  void writeData(uint8_t const* data, size_t len) const;
};

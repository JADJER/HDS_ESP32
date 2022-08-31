//
// Created by jadjer on 24.08.22.
//

#pragma once

#include "CommandResult.hpp"
#include "DataTable11.hpp"

class ECU {
 public:
  ECU(int rx_pin, int tx_pin);
  ~ECU();

 public:
  bool connect() const;
  void spinOnce();
  void test() const;

 public:
  uint16_t getRpm() const;
  uint8_t getSpeed() const;
  float getTpsPercent() const;
  float getTpsVolts() const;
  float getEctTemp() const;
  float getEctVolts() const;

  float getIatTemp() const;
  float getIatVolts() const;

  float getBatteryVolts() const;

  float getMapPressure() const;
  float getMapVolts() const;

 private:
  int m_rx;
  int m_tx;
  DataTable11 m_dataTable11;

 private:
  void wakeup() const;
  bool initialize() const;

 private:
  void updateDataFromTable11();

 private:
  void sendCommand(uint8_t const* data, size_t len) const;
  CommandResult sendCommandAndRead(uint8_t const* data, size_t len) const;
  CommandResult sendCommandAndPrint(uint8_t const* data, size_t len) const;
};

//
// Created by jadjer on 24.08.22.
//

#pragma once

#include "DataTable11.hpp"
#include "DataTableD1.hpp"

class ECU {
 public:
  ECU(int rx_pin, int tx_pin);
  ~ECU();

 public:
  bool connect() const;
  void spinOnce();

 public:
  uint16_t getRpm() const;
  uint8_t getSpeed() const;
  uint8_t getTpsPercent() const;
  uint8_t getTpsVolts() const;
  float getEctTemp() const;
  uint8_t getEctVolts() const;

  float getIatTemp() const;
  uint8_t getIatVolts() const;

  float getBatteryVolts() const;

  float getMapPressure() const;
  uint8_t getMapVolts() const;

  uint8_t isStartedEngine() const;
  uint8_t isClutchOff() const;

 private:
  int m_rx;
  int m_tx;
  uint8_t m_bufferMaxSize;
  DataTable11 m_dataTable11;
  DataTableD1 m_dataTableD1;

 private:
  void wakeup() const;
  bool initialize() const;

 private:
  void updateDataFromTable11();
  void updateDataFromTableD1();

 private:
  void readData(uint8_t * data, size_t& len) const;
  void writeData(uint8_t const* data, size_t len) const;
};

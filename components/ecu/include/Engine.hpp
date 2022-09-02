//
// Created by jadjer on 01.09.22.
//

#pragma once

#include "EngineData.hpp"
#include "ICommunication.hpp"
#include "VehicleState.hpp"
#include <string>

class Engine {
 public:
  explicit Engine(ICommunication& ecu);

 public:
  void detectActiveTables();
  void test() const;

 public:
  std::string getId() const;

 public:
  int getState() const;
  int getRpm() const;
  int getSpeed() const;
  int getTpsPercent() const;
  float getTpsVolts() const;
  float getEctTemp() const;
  float getEctVolts() const;

  float getIatTemp() const;
  float getIatVolts() const;

  float getBatteryVolts() const;

  float getMapPressure() const;
  float getMapVolts() const;

  int getUnknown1() const;
  float getUnknown2() const;
  float getUnknown3() const;
  float getUnknown4() const;
  float getUnknown5() const;

 public:
  void spinOnce();

 private:
  bool m_enableTable10;
  bool m_enableTable11;
  bool m_enableTable20;
  bool m_enableTable21;
  EngineData m_data;
  VehicleState m_state;
  ICommunication& m_ecu;

 private:
  CommandResult* updateDataFromTable(uint8_t table) const;
  bool updateDataFromTable0();
  bool updateDataFromTable10();
  bool updateDataFromTable11();
  bool updateDataFromTable20();
  bool updateDataFromTable21();
  bool updateDataFromTableD0();
  bool updateDataFromTableD1();
};

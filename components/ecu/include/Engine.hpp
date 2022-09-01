//
// Created by jadjer on 01.09.22.
//

#pragma once

#include "EngineData.hpp"
#include "ICommunication.hpp"

class Engine {
 public:
  explicit Engine(ICommunication& ecu);

 public:
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

 public:
  void spinOnce();

 private:
  EngineData m_data;
  ICommunication& m_ecu;

 private:
  void updateDataFromTable0();
  void updateDataFromTable10();
  void updateDataFromTable11();
  void updateDataFromTable20();
  void updateDataFromTable21();
  void updateDataFromTable60();
  void updateDataFromTable61();
  void updateDataFromTable70();
  void updateDataFromTable71();
  void updateDataFromTableD0();
  void updateDataFromTableD1();
};

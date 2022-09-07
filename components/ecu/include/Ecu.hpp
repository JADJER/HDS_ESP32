//
// Created by jadjer on 01.09.22.
//

#pragma once

#include "ICommunication.hpp"
#include "data/EngineData.hpp"
#include "data/ErrorData.hpp"
#include "data/SensorsData.hpp"
#include "data/UnknownData.hpp"
#include "data/VehicleData.hpp"
#include <string>

class Ecu {
 public:
  explicit Ecu(ICommunication& communication);

 public:
  void detectActiveTables();
  void test() const;

 public:
  std::string getId() const;
  VehicleData getVehicleData() const;
  EngineData getEngineData() const;
  SensorsData getSensorsData() const;
  ErrorData getErrorData() const;
  UnknownData getUnknownData() const;

 public:
 public:
  void spinOnce();

 private:
  std::string m_id;
  bool m_enableTable10;
  bool m_enableTable11;
  bool m_enableTable20;
  bool m_enableTable21;

  ICommunication& m_communication;

  VehicleData m_vehicleData;
  EngineData m_engineData;
  SensorsData m_sensorsData;
  ErrorData m_errorData;
  UnknownData m_unknownData;

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

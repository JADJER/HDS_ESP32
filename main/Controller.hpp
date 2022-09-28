//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "Button.hpp"
#include "ECU.hpp"
#include "Indicator.hpp"
#include "Protocol.hpp"
#include "ServerCallback.hpp"
#include <BLECharacteristic.h>
#include <BLEServer.h>

class Controller {
 public:
  Controller();
  ~Controller();

 public:
  [[noreturn]] void spin();

 private:
  Indicator* m_indicator;
  Button m_button;
  Protocol m_protocol;
  ECU m_ecu;
  BLEServer* m_server;
  ServerCallback m_serverCallback;
  BLECharacteristic* m_vehicleBattery;
  BLECharacteristic* m_vehicleSpeed;
  BLECharacteristic* m_vehicleState;
  BLECharacteristic* m_engineRpm;
  BLECharacteristic* m_engineFuelInject;
  BLECharacteristic* m_engineIgnitionAdvance;
  BLECharacteristic* m_engineUnk1;
  BLECharacteristic* m_engineUnk2;
  BLECharacteristic* m_engineUnk3;
  BLECharacteristic* m_sensorTpsPercent;
  BLECharacteristic* m_sensorTpsVoltage;
  BLECharacteristic* m_sensorEctTemp;
  BLECharacteristic* m_sensorEctVoltage;
  BLECharacteristic* m_sensorIatTemp;
  BLECharacteristic* m_sensorIatVoltage;
  BLECharacteristic* m_sensorMapPressure;
  BLECharacteristic* m_sensorMapVoltage;

 private:
  void testTables();
  void updateCharacteristics();
};

//
// Created by jadjer on 29.08.22.
//

#pragma once

#include <BLECharacteristic.h>
#include <BLEServer.h>
#include <BLEService.h>
#include <string>

class BLE {
 public:
  explicit BLE(std::string const& name);
  ~BLE();

 public:
  BLEService* createService(std::string const& uuid);
  BLECharacteristic* createCharacteristic(std::string const& serviceUuid, std::string const& characteristicUuid);
  BLECharacteristic* createCharacteristic(std::string const& serviceUuid, std::string const& characteristicUuid, uint32_t property);

 public:
  void start();
  void stop();
  void advertising();

 public:
  void setValueUInt16(std::string const& characteristicUuid, uint16_t value);
  void setValueUInt32(std::string const& characteristicUuid, uint32_t value);
  void setValueInt(std::string const& characteristicUuid, int value);
  void setValueFloat(std::string const& characteristicUuid, float value);
  void setValueDouble(std::string const& characteristicUuid, double value);
  void setValueString(std::string const& characteristicUuid, std::string const& value);

 private:
  BLEServer* m_server;
  BLEServiceMap m_bleServiceMap;
  BLECharacteristicMap m_bleCharacteristicMap;
};

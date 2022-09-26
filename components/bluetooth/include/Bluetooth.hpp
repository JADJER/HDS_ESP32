//
// Created by jadjer on 29.08.22.
//

#pragma once

#include <BLECharacteristic.h>
#include <BLEServer.h>
#include <BLEService.h>
#include <string>

class Bluetooth {
 public:
  explicit Bluetooth(std::string const& name);
  ~Bluetooth();

 public:
  BLEService* createService(std::string const& serviceName, std::vector<std::string> const& characteristicsUUID);

 public:
  void start();
  void stop();
  void advertising();

 public:
  template<typename T>
  void setValue(std::string const& characteristicUuid, T value) {
    auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
    if (not characteristic) {
      return;
    }

    characteristic->setValue(value);
    characteristic->notify();
  }

 private:
  BLEServer* m_server;
  BLEServiceMap m_bleServiceMap;
  BLECharacteristicMap m_bleCharacteristicMap;
};

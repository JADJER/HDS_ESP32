// Copyright 2022 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by jadjer on 24.09.22.
//

#pragma once

#include <BLECharacteristic.h>
#include <BLEClient.h>
#include <BLEServer.h>
#include <BLEService.h>
#include <string>

class BluetoothServer {
 public:
  BluetoothServer();
  ~BluetoothServer();

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

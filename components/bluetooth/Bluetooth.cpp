//
// Created by jadjer on 29.08.22.
//

#include "Bluetooth.hpp"
#include <BLEDevice.h>

Bluetooth::Bluetooth(std::string const& name) {
  BLEDevice::init(name);
}

Bluetooth::~Bluetooth() = default;

BluetoothClient* Bluetooth::createClient() {
  return new BluetoothClient();
}

BluetoothServer* Bluetooth::createServer() {
  return new BluetoothServer();
}

//
// Created by jadjer on 24.09.22.
//

#include "include/BluetoothServer.hpp"
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEUUID.h>
#include "AdvertisedHdsDeviceCallbacks.hpp"

BluetoothServer::BluetoothServer() : m_bleCharacteristicMap() {
  m_server = BLEDevice::createServer();
}

BluetoothServer::~BluetoothServer() = default;

BLEService* BluetoothServer::createService(std::string const& serviceUUID, std::vector<std::string> const& characteristicsUUID) {
  auto service = m_server->createService(serviceUUID, characteristicsUUID.size() * 3 + 1);
  m_bleServiceMap.setByUUID(serviceUUID, service);

  for (auto& characteristicUUID : characteristicsUUID) {
    auto characteristic = service->createCharacteristic(
        characteristicUUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    characteristic->addDescriptor(new BLE2902());

    m_bleCharacteristicMap.setByUUID(characteristic, characteristicUUID);
  }

  return service;
}

void BluetoothServer::start() {
  auto service = m_bleServiceMap.getFirst();
  while (service) {
    service->start();
    service = m_bleServiceMap.getNext();
  }
}

void BluetoothServer::stop() {
  auto service = m_bleServiceMap.getFirst();
  while (service) {
    service->stop();
    service = m_bleServiceMap.getNext();
  }
}

void BluetoothServer::advertising() {
  auto service = m_bleServiceMap.getFirst();
  if (not service) {
    return;
  }

  auto advertising = BLEDevice::getAdvertising();

  while (service) {
    advertising->addServiceUUID(service->getUUID());
    service = m_bleServiceMap.getNext();
  }

  advertising->setScanResponse(false);
  advertising->setMinPreferred(0x00);

  BLEDevice::startAdvertising();
}

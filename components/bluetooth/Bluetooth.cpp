//
// Created by jadjer on 29.08.22.
//

#include "Bluetooth.hpp"
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEUUID.h>

Bluetooth::Bluetooth(std::string const& name) : m_bleCharacteristicMap() {
  BLEDevice::init(name);
  m_server = BLEDevice::createServer();
}

Bluetooth::~Bluetooth() = default;

BLEService* Bluetooth::createService(std::string const& serviceUUID, std::vector<std::string> const& characteristicsUUID) {
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

void Bluetooth::start() {
  auto service = m_bleServiceMap.getFirst();
  while (service) {
    service->start();
    service = m_bleServiceMap.getNext();
  }
}

void Bluetooth::stop() {
  auto service = m_bleServiceMap.getFirst();
  while (service) {
    service->stop();
    service = m_bleServiceMap.getNext();
  }
}

void Bluetooth::advertising() {
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

void Bluetooth::setValueUInt16(std::string const& characteristicUuid, uint16_t value) {
  auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
  if (not characteristic) {
    return;
  }

  characteristic->setValue(value);
  characteristic->notify();
}

void Bluetooth::setValueFloat(std::string const& characteristicUuid, float value) {
  auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
  if (not characteristic) {
    return;
  }

  characteristic->setValue(value);
  characteristic->notify();
}

void Bluetooth::setValueString(std::string const& characteristicUuid, std::string const& value) {
  auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
  if (not characteristic) {
    return;
  }

  characteristic->setValue(value);
  characteristic->notify();

  delay(10);
}

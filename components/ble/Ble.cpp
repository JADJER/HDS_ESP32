//
// Created by jadjer on 29.08.22.
//

#include "Ble.hpp"
#include <BLE2902.h>
#include <BLE2904.h>
#include <BLEDevice.h>

BLE::BLE(std::string const& name) : m_bleCharacteristicMap() {
  BLEDevice::init(name);
  m_server = BLEDevice::createServer();
}

BLE::~BLE() = default;

BLEService* BLE::createService(std::string const& uuid) {
  auto service = m_server->createService(uuid);
  m_bleServiceMap.setByUUID(uuid, service);

  return service;
}

BLECharacteristic* BLE::createCharacteristic(std::string const& serviceUuid, std::string const& characteristicUuid) {
  return createCharacteristic(
      serviceUuid,
      characteristicUuid,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
}

BLECharacteristic* BLE::createCharacteristic(std::string const& serviceUuid, std::string const& characteristicUuid, uint32_t property) {
  auto service = m_bleServiceMap.getByUUID(serviceUuid);
  if (not service) {
    return nullptr;
  }

  auto characteristic = service->createCharacteristic(characteristicUuid, property);
  m_bleCharacteristicMap.setByUUID(characteristic, characteristicUuid);

  characteristic->addDescriptor(new BLE2902());

  return characteristic;
}

void BLE::start() {
  auto service = m_bleServiceMap.getFirst();
  while (service) {
    service->start();
    service = m_bleServiceMap.getNext();
  }
}

void BLE::stop() {
  auto service = m_bleServiceMap.getFirst();
  while (service) {
    service->stop();
    service = m_bleServiceMap.getNext();
  }
}

void BLE::advertising() {
  auto service = m_bleServiceMap.getFirst();
  if (not service) {
    return;
  }

  auto advertising = BLEDevice::getAdvertising();

  while (service) {
    advertising->addServiceUUID(service->getUUID());
    service = m_bleServiceMap.getNext();
  }

  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06);// functions that help with iPhone connections issue
  advertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void BLE::setValueUInt16(std::string const& characteristicUuid, uint16_t value) {
  auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
  if (not characteristic) {
    return;
  }

  characteristic->setValue(value);
  characteristic->notify();
}

void BLE::setValueUInt32(std::string const& characteristicUuid, uint32_t value) {
  auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
  if (not characteristic) {
    return;
  }

  characteristic->setValue(value);
  characteristic->notify();
}

void BLE::setValueInt(std::string const& characteristicUuid, int value) {
  auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
  if (not characteristic) {
    return;
  }

  characteristic->setValue(value);
  characteristic->notify();
}

void BLE::setValueFloat(std::string const& characteristicUuid, float value) {
  auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
  if (not characteristic) {
    return;
  }

  characteristic->setValue(value);
  characteristic->notify();
}

void BLE::setValueDouble(std::string const& characteristicUuid, double value) {
  auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
  if (not characteristic) {
    return;
  }

  characteristic->setValue(value);
  characteristic->notify();
}

void BLE::setValueString(std::string const& characteristicUuid, std::string const& value) {
  auto characteristic = m_bleCharacteristicMap.getByUUID(characteristicUuid);
  if (not characteristic) {
    return;
  }

  characteristic->setValue(value);
  characteristic->notify();
}

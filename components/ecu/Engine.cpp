//
// Created by jadjer on 01.09.22.
//

#include "Engine.hpp"
#include "utils.hpp"

Engine::Engine(ICommunication& ecu) : m_data(), m_ecu(ecu) {}

void Engine::spinOnce() {
  updateDataFromTable0();
  updateDataFromTable10();
  updateDataFromTable11();
  updateDataFromTable20();
  updateDataFromTable21();
  updateDataFromTable60();
  updateDataFromTable61();
  updateDataFromTable70();
  updateDataFromTable71();
  updateDataFromTableD0();
  updateDataFromTableD1();
}

int Engine::getRpm() const {
  return m_data.rpm;
}

int Engine::getSpeed() const {
  return m_data.speed;
}

int Engine::getTpsPercent() const {
  return m_data.tpsPercent;
}

float Engine::getTpsVolts() const {
  return m_data.tpsVolts;
}

float Engine::getEctTemp() const {
  return m_data.ectTemp;
}

float Engine::getEctVolts() const {
  return m_data.ectVolts;
}

float Engine::getIatTemp() const {
  return m_data.iatTemp;
}

float Engine::getIatVolts() const {
  return m_data.iatVolts;
}

float Engine::getBatteryVolts() const {
  return m_data.batteryVolts;
}

float Engine::getMapPressure() const {
  return m_data.mapPressure;
}

float Engine::getMapVolts() const {
  return m_data.mapVolts;
}

void Engine::updateDataFromTable0() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0x0};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }
}

void Engine::updateDataFromTable10() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0x10};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }

  m_data.rpm = (uint16_t) (response->data[4] << 8) + response->data[5];
  m_data.tpsVolts = calcValueDivide256(response->data[6]);
  m_data.tpsPercent = calcValueDivide16(response->data[7]);
  m_data.tpsPercent = 18;
  m_data.ectVolts = calcValueDivide256(response->data[8]);
  m_data.ectTemp = calcValueMinus40(response->data[9]);
  m_data.iatVolts = calcValueDivide256(response->data[10]);
  m_data.iatTemp = calcValueMinus40(response->data[11]);
  m_data.mapVolts = calcValueDivide256(response->data[12]);
  m_data.mapPressure = response->data[13];
  m_data.batteryVolts = calcValueDivide10(response->data[16]);
  m_data.speed = response->data[17];
}

void Engine::updateDataFromTable11() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0x11};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    m_data.rpm = 1500;
    m_data.tpsVolts = 1.0;
    m_data.tpsPercent = 18;
    return;
  }

  m_data.rpm = (uint16_t) (response->data[4] << 8) + response->data[5];
  m_data.tpsVolts = calcValueDivide256(response->data[6]);
  m_data.tpsPercent = calcValueDivide16(response->data[7]);
  m_data.tpsPercent = 18;
  m_data.ectVolts = calcValueDivide256(response->data[8]);
  m_data.ectTemp = calcValueMinus40(response->data[9]);
  m_data.iatVolts = calcValueDivide256(response->data[10]);
  m_data.iatTemp = calcValueMinus40(response->data[11]);
  m_data.mapVolts = calcValueDivide256(response->data[12]);
  m_data.mapPressure = response->data[13];
  m_data.batteryVolts = calcValueDivide10(response->data[16]);
  m_data.speed = response->data[17];
}

void Engine::updateDataFromTable20() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0x20};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }
}

void Engine::updateDataFromTable21() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0x21};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }
}

void Engine::updateDataFromTable60() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0x60};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }
}

void Engine::updateDataFromTable61() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0x61};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }
}

void Engine::updateDataFromTable70() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0x70};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }
}

void Engine::updateDataFromTable71() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0x71};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }
}

void Engine::updateDataFromTableD0() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0xD0};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }
}

void Engine::updateDataFromTableD1() {
  uint8_t message[5] = {0x72, 0x05, 0x71, 0xD1};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  auto response = m_ecu.readData();
  if (response == nullptr) {
    return;
  }
}

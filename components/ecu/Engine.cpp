//
// Created by jadjer on 01.09.22.
//

#include "Engine.hpp"
#include "utils.hpp"

Engine::Engine(ICommunication& ecu) : m_data(), m_state(), m_ecu(ecu) {
  m_enableTable10 = false;
  m_enableTable11 = false;
  m_enableTable20 = false;
  m_enableTable21 = false;
}

void Engine::detectActiveTables() {
  updateDataFromTable0();

  if (updateDataFromTable10()) { m_enableTable10 = true; }
  if (updateDataFromTable11()) { m_enableTable11 = true; }
  if (updateDataFromTable20()) { m_enableTable20 = true; }
  if (updateDataFromTable21()) { m_enableTable21 = true; }
}

void Engine::test() const {
  uint8_t address = 0x0;

  for (size_t i = 0; i <= 255; i++) {
    uint8_t data[] = {0x72, 0x05, 0x71, address};
    uint8_t checkSum = calcChecksum(data, sizeof(data));
    uint8_t dataWithChk[] = {0x72, 0x05, 0x71, address, checkSum};

    address++;

    m_ecu.writeData(dataWithChk, sizeof(dataWithChk));
    m_ecu.readData();
  }
}

std::string Engine::getId() const {
  return m_data.id;
}

int Engine::getState() const {
  return m_state.state;
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

int Engine::getUnknown1() const {
  return m_data.unk_data_1;
}

float Engine::getUnknown2() const {
  return m_data.unk_data_2;
}

float Engine::getUnknown3() const {
  return m_data.unk_data_3;
}

float Engine::getUnknown4() const {
  return m_data.unk_data_4;
}

float Engine::getUnknown5() const {
  return m_data.unk_data_5;
}

void Engine::spinOnce() {
  if (m_enableTable10) { updateDataFromTable10(); }
  if (m_enableTable11) { updateDataFromTable11(); }
  if (m_enableTable20) { updateDataFromTable20(); }
  if (m_enableTable21) { updateDataFromTable21(); }

  updateDataFromTableD0();
  updateDataFromTableD1();
}

CommandResult* Engine::updateDataFromTable(uint8_t table) const {
  uint8_t message[5] = {0x72, 0x05, 0x71, table};
  uint8_t checksum = calcChecksum(message, sizeof(message));

  message[4] = checksum;

  m_ecu.writeData(message, sizeof(message));

  return m_ecu.readData();
}

bool Engine::updateDataFromTable0() {
  auto response = updateDataFromTable(0x0);
  if (response == nullptr) {
    m_data.id = "undefined";
    return false;
  }

  for (size_t i = 4; i < response->len - 1; i++) {
    m_data.id += std::to_string(static_cast<int>(response->data[i]));
  }

  return true;
}

bool Engine::updateDataFromTable10() {
  auto response = updateDataFromTable(0x10);
  if (response == nullptr) {
    return false;
  }

  //  0   1    2    3     4   5     6    7     8    9      10   11     12   13    14   15    16     17    18  19    20    21
  //  0x2 0x16 0x71 0x10 {0x0 0x0} {0x18 0x0} {0x1b 0x8e} {0x87 0x47} {0x90 0x62} 0xff 0xff {0x77} {0x0} {0x0 0x0} {0x80} 0xf1

  m_data.rpm = (response->data[4] << 8) + response->data[5];
  m_data.tpsVolts = calcValueDivide256(response->data[6]);
  m_data.tpsPercent = static_cast<int>(calcValueDivide16(response->data[7]));
  m_data.ectVolts = calcValueDivide256(response->data[8]);
  m_data.ectTemp = calcValueMinus40(response->data[9]);
  m_data.iatVolts = calcValueDivide256(response->data[10]);
  m_data.iatTemp = calcValueMinus40(response->data[11]);
  m_data.mapVolts = calcValueDivide256(response->data[12]);
  m_data.mapPressure = response->data[13];
  m_data.batteryVolts = calcValueDivide10(response->data[16]);
  m_data.speed = response->data[17];

  m_data.unk_data_1 = static_cast<int>(calcValueDivide10((response->data[18] << 8) + response->data[19]));
  m_data.unk_data_2 = calcValueDivide256(response->data[20]);

  return true;
}

bool Engine::updateDataFromTable11() {
  auto response = updateDataFromTable(0x11);
  if (response == nullptr) {
    return false;
  }

  //  0   1    2    3     4   5      6    7     8    9      10   11     12   13    14   15    16     17    18  19     20     21     22     23    24
  //  0x2 0x19 0x71 0x11 {0x1 0x3c} {0x1a 0x0} {0xac 0x38} {0xac 0x37} {0x7c 0x53} 0xff 0xff {0x60} {0x0} {0xb 0x1c} {0x94} {0x27} {0x2d} {0x2b} 0xde

  m_data.rpm = (response->data[4] << 8) + response->data[5];
  m_data.tpsVolts = calcValueDivide256(response->data[6]);
  m_data.tpsPercent = static_cast<int>(calcValueDivide16(response->data[7]));
  m_data.ectVolts = calcValueDivide256(response->data[8]);
  m_data.ectTemp = calcValueMinus40(response->data[9]);
  m_data.iatVolts = calcValueDivide256(response->data[10]);
  m_data.iatTemp = calcValueMinus40(response->data[11]);
  m_data.mapVolts = calcValueDivide256(response->data[12]);
  m_data.mapPressure = response->data[13];
  m_data.batteryVolts = calcValueDivide10(response->data[16]);
  m_data.speed = response->data[17];

  m_data.unk_data_1 = static_cast<int>(calcValueDivide10((response->data[18] << 8) + response->data[19]));
  m_data.unk_data_2 = calcValueDivide256(response->data[20]);
  m_data.unk_data_3 = calcValueDivide256(response->data[21]);
  m_data.unk_data_4 = calcValueDivide256(response->data[22]);
  m_data.unk_data_5 = calcValueDivide256(response->data[23]);

  return true;
}

bool Engine::updateDataFromTable20() {
  auto response = updateDataFromTable(0x20);
  if (response == nullptr) {
    return false;
  }

  (void) response->data[4];
  (void) response->data[5];
  (void) response->data[6];

  return true;
}

bool Engine::updateDataFromTable21() {
  auto response = updateDataFromTable(0x21);
  if (response == nullptr) {
    return false;
  }

  (void) response->data[4];
  (void) response->data[5];
  (void) response->data[6];

  m_data.unk_data_3 = calcValueDivide256(response->data[7]);
  m_data.unk_data_4 = calcValueDivide256(response->data[8]);
  m_data.unk_data_5 = calcValueDivide256(response->data[9]);

  return true;
}

bool Engine::updateDataFromTableD0() {
  auto response = updateDataFromTable(0xD0);
  if (response == nullptr) {
    return false;
  }

  return true;
}

bool Engine::updateDataFromTableD1() {
  auto response = updateDataFromTable(0xD1);
  if (response == nullptr) {
    return false;
  }

  m_state.state = response->data[4];

  return true;
}

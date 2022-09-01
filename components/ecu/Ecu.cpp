//
// Created by jadjer on 24.08.22.
//

#include "Ecu.hpp"
#include "utils.hpp"
#include <Arduino.h>
#include <atomic>
#include <iostream>

ECU::ECU(int rx_pin, int tx_pin) : m_dataTable11() {
  m_rx = rx_pin;
  m_tx = tx_pin;
  m_bufferMaxSize = 128;
}

ECU::~ECU() = default;

bool ECU::connect() const {
  std::cout << "Connect to ECU:" << std::endl;

  wakeup();
  return initialize();
}

void ECU::spinOnce() {
  updateDataFromTable11();
}

void ECU::test() const {
  uint8_t address = 0x0;

  for (size_t i = 0; i <= 255; i++) {
    uint8_t data[] = {0x72, 0x05, 0x71, address};
    uint8_t checkSum = calcChecksum(data, sizeof(data));
    uint8_t dataWithChk[] = {0x72, 0x05, 0x71, address, checkSum};

    address++;

    writeData(dataWithChk, sizeof(dataWithChk), true);
    readData(true);
  }
}

uint16_t ECU::getRpm() const {
  return m_dataTable11.rpm;
}

uint8_t ECU::getSpeed() const {
  return m_dataTable11.speed;
}

float ECU::getTpsPercent() const {
  return m_dataTable11.tpsPercent;
}

float ECU::getTpsVolts() const {
  return m_dataTable11.tpsVolts;
}

float ECU::getEctTemp() const {
  return m_dataTable11.ectTemp;
}

float ECU::getEctVolts() const {
  return m_dataTable11.ectVolts;
}

float ECU::getIatTemp() const {
  return m_dataTable11.iatTemp;
}

float ECU::getIatVolts() const {
  return m_dataTable11.iatVolts;
}

float ECU::getBatteryVolts() const {
  return m_dataTable11.batteryVolts;
}

float ECU::getMapPressure() const {
  return m_dataTable11.mapPressure;
}

float ECU::getMapVolts() const {
  return m_dataTable11.mapVolts;
}

void ECU::wakeup() const {
  std::cout << "Wakeup ECU..." << std::endl;

  pinMode(m_tx, OUTPUT);

  digitalWrite(m_tx, LOW);
  delay(70);

  digitalWrite(m_tx, HIGH);
  delay(130);
}

bool ECU::initialize() const {
  std::cout << "Initialize ECU..." << std::endl;

  uint8_t wakeupMessage[] = {0xFE, 0x04, 0xFF, 0xFF};
  uint8_t initMessage[] = {0x72, 0x05, 0x00, 0xF0, 0x99};

  Serial2.begin(10400);

  writeData(wakeupMessage, sizeof(wakeupMessage), true);
  writeData(initMessage, sizeof(initMessage), true);

  auto data = readData(true);

  if (data == nullptr) { return false; }
  if (data->code != 0x02) { return false; }
  if (data->len != 0x04) { return false; }
  if (data->command != 0x00) { return false; }
  if (data->checksum != 0xFA) { return false; }

  return true;
}

void ECU::updateDataFromTable11() {
  uint8_t message[] = {0x72, 0x05, 0x71, 0x11};
  uint8_t checksum = calcChecksum(message, sizeof(message));
  uint8_t messageWithChecksum[] = {0x72, 0x05, 0x71, 0x11, checksum};

  writeData(messageWithChecksum, sizeof(messageWithChecksum), false);

  auto response = readData(false);
  //  0   1    2    3    4   5   6    7   8    9    10   11   12   13   14   15   16   17  18  19  20   21   22   23   24
  //  0x2 0x19 0x71 0x11 0x0 0x0 0x19 0x0 0x3f 0x6a 0x8c 0x45 0x8f 0x61 0xff 0xff 0x77 0x0 0x0 0x0 0x80 0x16 0x1d 0xb7 0x1

  m_dataTable11.rpm = (uint16_t) (response->data[4] << 8) + response->data[5];
  m_dataTable11.tpsVolts = calcValueDivide256(response->data[6]);
  m_dataTable11.tpsPercent = calcValueDivide16(response->data[7]);
  m_dataTable11.ectVolts = calcValueDivide256(response->data[8]);
  m_dataTable11.ectTemp = calcValueMinus40(response->data[9]);
  m_dataTable11.iatVolts = calcValueDivide256(response->data[10]);
  m_dataTable11.iatTemp = calcValueMinus40(response->data[11]);
  m_dataTable11.mapVolts = calcValueDivide256(response->data[12]);
  m_dataTable11.mapPressure = response->data[13];
  m_dataTable11.batteryVolts = calcValueDivide10(response->data[16]);
  m_dataTable11.speed = response->data[17];
}

CommandResult* ECU::readData(bool print) const {
  //    Wait minimum package from uart
  while (Serial2.available() < 4) {
    delay(100);
  }

  uint8_t responseConfirm = Serial2.read();
  uint8_t responseLength = Serial2.read();
  uint8_t responseCommand = Serial2.read();

  //    Create buffer for read
  auto buffer = new uint8_t[responseLength];
  buffer[0] = responseConfirm;
  buffer[1] = responseLength;
  buffer[2] = responseCommand;

  if (responseLength > m_bufferMaxSize) {
    std::cerr << "The message is too big (size: " << std::hex << "0x" << (int) responseLength << ")" << std::endl;

    for (size_t i = 0; i < responseLength; i++) {
      std::cerr << std::hex << "0x" << (int) buffer[i] << " ";
    }
    std::cerr << std::endl;

    return nullptr;
  }

  //    Wait full package from uart
  while (Serial2.available() < (responseLength - 3)) {
    delay(100);
  }

  //    Read package data from uart
  for (size_t i = 3; i < responseLength; i++) {
    uint8_t value = Serial2.read();
    buffer[i] = value;
  }

  uint8_t responseChecksum = buffer[responseLength - 1];
  uint8_t calculatedChecksum = calcChecksum(buffer, 0, responseLength - 1);

  if (responseChecksum != calculatedChecksum) {
    std::cerr << "The checksum does not match" << std::endl;

    for (size_t i = 0; i < responseLength; i++) {
      std::cerr << std::hex << "0x" << (int) buffer[i] << " ";
    }
    std::cerr << std::endl;

    return nullptr;
  }

  auto result = new CommandResult();
  result->code = responseConfirm;
  result->command = responseCommand;
  result->checksum = responseChecksum;
  result->data = buffer;
  result->len = responseLength;

  if (print) {
    std::cout << "ECU >> ";
    for (size_t i = 0; i < responseLength; i++) {
      std::cout << std::hex << "0x" << (int) buffer[i] << " ";
    }
    std::cout << std::endl;
  }

  return result;
}

void ECU::writeData(uint8_t const* data, size_t len, bool print) const {
  Serial2.flush(false);
  delay(100);

  if (print) {
    std::cout << "ECU << ";
    for (size_t i = 0; i < len; i++) {
      std::cout << std::hex << "0x" << (int) data[i] << " ";
    }
    std::cout << std::endl;
  }

  Serial2.write(data, len);
  delay(100);

  while (Serial2.available() < len) {
    delay(100);
  }

  for (size_t i = 0; i < len; i++) {
    uint8_t nextValue = Serial2.peek();
    if (nextValue == data[i]) {
      Serial2.read();
    }
  }
}

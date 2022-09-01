//
// Created by jadjer on 24.08.22.
//

#include "Ecu.hpp"
#include "utils.hpp"
#include <Arduino.h>
#include <iostream>

ECU::ECU(int rx_pin, int tx_pin) {
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

void ECU::test() const {
  uint8_t address = 0x0;

  for (size_t i = 0; i <= 255; i++) {
    uint8_t data[] = {0x72, 0x05, 0x71, address};
    uint8_t checkSum = calcChecksum(data, sizeof(data));
    uint8_t dataWithChk[] = {0x72, 0x05, 0x71, address, checkSum};

    address++;

    writeData(dataWithChk, sizeof(dataWithChk));
    readData();
  }
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

  writeData(wakeupMessage, sizeof(wakeupMessage));
  writeData(initMessage, sizeof(initMessage));

  auto data = readData();

  if (data == nullptr) { return false; }
  if (data->code != 0x02) { return false; }
  if (data->len != 0x04) { return false; }
  if (data->command != 0x00) { return false; }
  if (data->checksum != 0xFA) { return false; }

  return true;
}

CommandResult* ECU::readData() const {
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

  if (responseLength == 0) {
    std::cerr << "ECU >> The message is very small (size: 0x" << std::hex << (int) responseLength << ")" << std::endl;
    return nullptr;
  }

  if (responseLength > m_bufferMaxSize) {
    std::cerr << "ECU >> The message is too big (size: 0x" << std::hex << (int) responseLength << "): ";
    for (size_t i = 0; i < responseLength; i++) {
      std::cerr << "0x" << std::hex << (int) buffer[i] << " ";
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
  uint8_t calculatedChecksum = calcChecksum(buffer, responseLength - 1);

  if (responseChecksum != calculatedChecksum) {
    std::cerr << "ECU >> The checksum does not match (0x" << std::hex << (int) calculatedChecksum << "): ";
    for (size_t i = 0; i < responseLength; i++) {
      std::cerr << "0x" << std::hex << (int) buffer[i] << " ";
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

  std::cout << "ECU >> ";
  for (size_t i = 0; i < responseLength; i++) {
    std::cout << "0x" << std::hex << (int) buffer[i] << " ";
  }
  std::cout << std::endl;

  return result;
}

void ECU::writeData(uint8_t const* data, size_t len) const {
  Serial2.flush(false);
  delay(100);

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

  std::cout << "ECU << ";
  for (size_t i = 0; i < len; i++) {
    std::cout << "0x" << std::hex << (int) data[i] << " ";
  }
  std::cout << std::endl;
}

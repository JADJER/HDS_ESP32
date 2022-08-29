//
// Created by jadjer on 24.08.22.
//

#include "Ecu.hpp"
#include <Arduino.h>
#include <iostream>

ECU::ECU(int rx_pin, int tx_pin) {
  m_i = 0;
  m_rx = rx_pin;
  m_tx = tx_pin;
}

ECU::~ECU() = default;

bool ECU::connect() const {
  std::cout << "Connect to ECU:" << std::endl;

  wakeup();
  return initialize();
}

void ECU::test() const {
  uint8_t message1[] = {0x72, 0x07, 0x72, 0xD1, 0x00, 0x05, 0x3F};
  uint8_t message2[] = {0x72, 0x07, 0x72, 0xD0, 0x00, 0x05, 0x40};
  uint8_t message3[] = {0x72, 0x07, 0x72, 0xD0, 0x00, 0x06, 0x3F};

  Serial2.begin(10400);

  {
    writeData(message1, sizeof(message1));
    delay(100);

    std::cout << "OK: 02 0B 72 D1 00 03 00 00 00 00 AD" << std::endl;

    readData();
    delay(100);
  }

  {
    writeData(message2, sizeof(message2));
    delay(100);

    std::cout << "OK: 02 0B 72 D0 00 00 00 00 00 00 B1" << std::endl;

    readData();
    delay(100);
  }

  {
    writeData(message3, sizeof(message3));
    delay(100);

    std::cout << "OK: 02 0B 72 D0 00 00 00 00 00 00 00 B0" << std::endl;

    readData();
    delay(100);
  }
}

int ECU::getTest() {
  return m_i++;
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
  uint8_t initMessageRequest[] = {0x72, 0x05, 0x00, 0xF0, 0x99};
  uint8_t initMessageResponse[] = {0x02, 0x04, 0x00, 0xFA};

  Serial2.begin(10400);

  writeData(wakeupMessage, sizeof(wakeupMessage));
  delay(100);

  writeData(initMessageRequest, sizeof(initMessageRequest));
  delay(100);

  auto initResponse = readData();

  //  if (initSum == checkSum) {
  //    return true;
  //  }

  return true;
}

std::vector<int> ECU::readData() const {
  std::vector<int> data;

  std::cout << "ECU >> ";
  while (Serial2.available() > 0) {
    int value = Serial2.read();
    data.push_back(value);
    std::cout << std::hex << "0x" << value << " ";
  }
  std::cout << std::endl;

  return data;
}

void ECU::writeData(uint8_t const* data, size_t len) const {
  std::cout << "ECU << ";
  for (size_t i = 0; i < len; i++) {
    std::cout << std::hex << "0x" << (int) data[i] << " ";
  }
  std::cout << std::endl;

  Serial2.write(data, len);
  Serial2.flush();
}

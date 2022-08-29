//
// Created by jadjer on 24.08.22.
//

#include "Ecu.hpp"
#include "Arduino.h"
#include <iostream>

ECU::ECU() {
  m_rx = 16;
  m_tx = 17;
}

ECU::~ECU() = default;

bool ECU::connect() const {
  std::cout << "Connect to ECU:" << std::endl;
  wakeup();
  initialize();
  return true;
//  return initialize();
}

void ECU::test() const {
  uint8_t message1[] = {0x72, 0x07, 0x72, 0xD1, 0x00, 0x05, 0x3F};
  uint8_t message2[] = {0x72, 0x07, 0x72, 0xD0, 0x00, 0x05, 0x40};
  uint8_t message3[] = {0x72, 0x07, 0x72, 0xD0, 0x00, 0x06, 0x3F};

  Serial2.begin(10400);

  {
    Serial2.write(message1, sizeof(message1));
    Serial2.flush();
    delay(50);

    std::cout << "OK: 02 0B 72 D1 00 03 00 00 00 00 AD" << std::endl;

    int buffCount = 0;
    while (Serial2.available() > 0 and buffCount < 32) {
      int value = Serial2.read();
      std::cout << std::hex << "0x" << value << " ";
      buffCount++;
    }
    std::cout << std::endl;
  }

  {
    Serial2.write(message2, sizeof(message2));
    Serial2.flush();
    delay(50);

    std::cout << "OK: 02 0B 72 D0 00 00 00 00 00 00 B1" << std::endl;

    int buffCount = 0;
    while (Serial2.available() > 0 and buffCount < 32) {
      int value = Serial2.read();
      std::cout << std::hex << "0x" << value << " ";
      buffCount++;
    }
    std::cout << std::endl;
  }

  {
    Serial2.write(message3, sizeof(message3));
    Serial2.flush();
    delay(50);

    std::cout << "OK: 02 0B 72 D0 00 00 00 00 00 00 00 B0" << std::endl;

    int buffCount = 0;
    while (Serial2.available() > 0 and buffCount < 32) {
      int value = Serial2.read();
      std::cout << std::hex << "0x" << value << " ";
      buffCount++;
    }
    std::cout << std::endl;
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
  uint16_t checksum = 0x100;

  Serial2.begin(10400);

  Serial2.write(wakeupMessage, sizeof(wakeupMessage));
  delay(200);

  Serial2.write(initMessage, sizeof(initMessage));
  Serial2.flush();
  delay(50);

  int initBuffCount = 0;
  int initSum = 0;
  while (Serial2.available() > 0 and initBuffCount < 32) {
    int value = Serial2.read();
    std::cout << std::hex << "0x" << value << " ";
    initBuffCount++;

    initSum += value;
  }
  std::cout << "SUM: " << std::hex << "0x" << initSum << std::endl;

  if(initSum == checksum) {
    return true;
  }

  return false;
}

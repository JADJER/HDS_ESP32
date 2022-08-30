//
// Created by jadjer on 24.08.22.
//

#include "Ecu.hpp"
#include "utils.hpp"
#include <Arduino.h>
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

    writeDataAndPrint(dataWithChk, sizeof(dataWithChk));
    delay(100);

    uint8_t responseData[m_bufferMaxSize];
    size_t responseDataLen = 0;
    readDataAndPrint(responseData, responseDataLen);
    delay(100);
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
  uint8_t initMessageRequest[] = {0x72, 0x05, 0x00, 0xF0, 0x99};
  uint8_t initMessageResponse[] = {0x02, 0x04, 0x00, 0xFA};
  uint8_t initMessageResponseChecksum = calcChecksum(initMessageResponse, sizeof(initMessageResponse));

  Serial2.begin(10400);

  writeDataAndPrint(wakeupMessage, sizeof(wakeupMessage));
  delay(100);

  writeDataAndPrint(initMessageRequest, sizeof(initMessageRequest));
  delay(100);

  uint8_t responseData[m_bufferMaxSize];
  size_t responseDataLen = 0;

  auto isRead = readDataAndPrint(responseData, responseDataLen);
  if (not isRead) {
    return false;
  }

  auto initResponseChecksum = calcChecksum(responseData, responseDataLen);
  if (initResponseChecksum == initMessageResponseChecksum) {
    return true;
  }

  return false;
}

void ECU::updateDataFromTable11() {
  uint8_t data[] = {0x72, 0x05, 0x71, 0x11};
  uint8_t checkSum = calcChecksum(data, sizeof(data));
  uint8_t dataWithChk[] = {0x72, 0x05, 0x71, 0x11, checkSum};

  writeData(dataWithChk, sizeof(dataWithChk));

  uint8_t responseData[m_bufferMaxSize];
  size_t responseDataLen = 0;

  auto isRead = readData(responseData, responseDataLen);
  if (not isRead) {
    return;
  }

  m_dataTable11.rpm = (uint16_t) (responseData[9] << 8) + responseData[10];
  m_dataTable11.tpsVolts = calcValueDivide256(responseData[11]);
  m_dataTable11.tpsPercent = calcValueDivide16(responseData[12]);
  m_dataTable11.ectVolts = calcValueDivide256(responseData[13]);
  m_dataTable11.ectTemp = calcValueMinus40(responseData[14]);
  m_dataTable11.iatVolts = calcValueDivide256(responseData[15]);
  m_dataTable11.iatTemp = calcValueMinus40(responseData[16]);
  m_dataTable11.mapVolts = calcValueDivide256(responseData[17]);
  m_dataTable11.mapPressure = responseData[18];
  m_dataTable11.batteryVolts = calcValueDivide10(responseData[21]);
  m_dataTable11.speed = responseData[22];
}

bool ECU::readData(uint8_t* data, size_t& len) const {
  len = 0;

  while (Serial2.available() > 0 and len < m_bufferMaxSize) {
    uint8_t value = Serial2.read();
    data[len++] = value;
  }

  if (len == 0) {
    return false;
  }

  return true;
}

void ECU::writeData(uint8_t const* data, size_t len) const {
  Serial2.write(data, len);
  Serial2.flush();
}

bool ECU::readDataAndPrint(uint8_t* data, size_t& len) const {
  len = 0;

  std::cout << "ECU >> ";
  while (Serial2.available() > 0 and len < m_bufferMaxSize) {
    uint8_t value = Serial2.read();

    data[len++] = value;
    std::cout << std::hex << "0x" << (int) value << " ";
  }
  std::cout << std::endl;

  if (len == 0) {
    return false;
  }

  return true;
}

void ECU::writeDataAndPrint(uint8_t const* data, size_t len) const {
  std::cout << "ECU << ";
  for (size_t i = 0; i < len; i++) {
    std::cout << std::hex << "0x" << (int) data[i] << " ";
  }
  std::cout << std::endl;

  Serial2.write(data, len);
  Serial2.flush();
}

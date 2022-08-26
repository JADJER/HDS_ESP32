//
// Created by jadjer on 24.08.22.
//

#include <Arduino.h>
#include <sstream>
#include <iostream>

#include "ecu.hpp"
#include "utils.hpp"

#define bike Serial2

ECU::ECU() : m_rx(16), m_tx(17), m_bufferSize(128) {
  m_isConnected = false;
}

ECU::~ECU() = default;

void ECU::wakeup() const {
  //Honda ECU Init sequence
  pinMode(m_tx, OUTPUT);
  digitalWrite(m_tx, LOW);//TX Low for 70ms
  delay(70);
  digitalWrite(m_tx, HIGH);//TX High for 120ms
  delay(120);
}

void ECU::setup() {
  //Honda ecu communication handshake
  byte ECU_WAKEUP_MESSAGE[] = {0xFE, 0x04, 0xFF, 0xFF};
  byte ECU_INIT_MESSAGE[] = {0x72, 0x05, 0x00, 0xF0, 0x99};
  uint16_t ECU_SUCCESS_CHECKSUM = 0x6FB;

  ESP_LOGI("HDS.ECU", "Starting up...");
  ESP_LOGI("HDS.ECU", "Setting line low 70ms, high 120ms");

  wakeup();
  bike.begin(10400);

  ESP_LOGI("HDS.ECU", "Sending ECU Wakeup");
  bike.write(ECU_WAKEUP_MESSAGE, sizeof(ECU_WAKEUP_MESSAGE));
  delay(200);

  ESP_LOGI("HDS.ECU", "Sending ECU Init String");
  bike.write(ECU_INIT_MESSAGE, sizeof(ECU_INIT_MESSAGE));
  bike.flush();
  delay(50);

  int initBuffCount = 0;
  byte initBuff[32];
  while (bike.available() > 0 and initBuffCount < 32) {
    initBuff[initBuffCount++] = bike.read();
  }

  printBuffer(initBuff, 32);

  int initSum = 0;
  for (int i = 0; i < initBuffCount; i++) {
    initSum += initBuff[i];
  }

  if (initSum == ECU_SUCCESS_CHECKSUM) {
    ESP_LOGI("HDS.ECU", "Successfully opened connection to ECU");
    m_isConnected = true;
  } else {
    ESP_LOGI("HDS.ECU", "Failed to open connection to ECU, trying again in 2s");
    m_isConnected = false;
  }
}

bool ECU::isConnected() const {
  return m_isConnected;
}

//Shows most other info
TABLE11RESPONSE ECU::showDataTable11() {

  byte data[] = {0x72, 0x05, 0x71, 0x11};
  byte chk = calcChecksum(data, sizeof(data));
  byte dataWithChk[] = {0x72, 0x05, 0x71, 0x11, chk};

  bike.write(dataWithChk, sizeof(dataWithChk));
  delay(50);

  int buffCount = 0;
  byte buff[m_bufferSize];
  while ((bike.available() > 0) and (buffCount < m_bufferSize)) {
    buff[buffCount++] = bike.read();
  }

  TABLE11RESPONSE response;

  response.rpm = (uint16_t) (buff[9] << 8) + buff[10];
  response.tpsPercent = calcValueDivide16(buff[12]);
  response.ectTemp = calcValueMinus40(buff[14]);
  response.iatTemp = calcValueMinus40(buff[16]);
  response.battVolts = calcValueDivide10(buff[21]);
  response.speedKPH = buff[22];
  response.tpsVolts = calcValueDivide256(buff[11]);
  response.mapVolts = calcValueDivide256(buff[17]);
  response.mapPressure = buff[18];
  response.iatVolts = calcValueDivide256(buff[15]);
  response.ectVolts = calcValueDivide256(buff[13]);

  return response;
}

//Shows info like neutral switch, engine on
TABLED1RESPONSE ECU::showDataTableD1() {

  byte data[] = {0x72, 0x05, 0x71, 0xD1};
  byte chk = calcChecksum(data, sizeof(data));
  byte dataWithChk[] = {0x72, 0x05, 0x71, 0xD1, chk};

  bike.write(dataWithChk, sizeof(dataWithChk));

  delay(50);

  int buffCount = 0;
  byte buff[m_bufferSize];
  while ((bike.available() > 0) and (buffCount < m_bufferSize)) {
    buff[buffCount++] = bike.read();
  }

  TABLED1RESPONSE response {
      .engState = buff[13],
      .switchState = buff[9]
  };

  return response;
}

void ECU::bufferECUResponse() {
  int buffCount = 0;
  byte buff[m_bufferSize];

  while ((bike.available() > 0) and (buffCount < m_bufferSize)) {
    buff[buffCount++] = bike.read();
  }

  for (int i = 0; i < buffCount; i++) {
    std::stringstream ss;
    ss << buff[i];

    if (i != buffCount - 1) {
      ESP_LOGI("HDS.ECU", "Data from ECU: %s;", ss.str().c_str());
    }
  }
}

void ECU::printBuffer(byte* buffer, size_t buffer_size) {
  ESP_LOGI("HDS.ECU", "Data from ECU:");

  for (int i = 0; i < buffer_size; i++) {
    std::cout << std::hex << std::uppercase << buffer[i];
  }

  std::cout << std::endl;
}

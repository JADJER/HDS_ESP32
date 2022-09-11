//
// Created by jadjer on 24.08.22.
//

#include "Protocol.hpp"
#include "utils.hpp"
#include <esp32-hal-gpio.h>

Protocol::Protocol(int8_t rx, int8_t tx) : m_serial(2) {
  m_rx = rx;
  m_tx = tx;
  m_bufferMaxSize = 128;
}

Protocol::~Protocol() {
  m_serial.end();
}

bool Protocol::connect() {
  log_d("Connect to ECU:");

  wakeup();
  return initialize();
}

void Protocol::wakeup() const {
  log_d("Wakeup ECU...");

  pinMode(m_tx, OUTPUT);

  digitalWrite(m_tx, LOW);
  delay(70);

  digitalWrite(m_tx, HIGH);
  delay(130);
}

bool Protocol::initialize() {
  log_d("Initialize ECU...");

  uint8_t wakeupMessage[] = {0xFE, 0x04, 0xFF, 0xFF};
  uint8_t initializeMessage[] = {0x72, 0x05, 0x00, 0xF0, 0x99};

  m_serial.begin(10400, SERIAL_8N1, m_rx, m_tx);

  log_d("Send wakeup message...");
  writeData(wakeupMessage, sizeof(wakeupMessage));

  delay(20);

  log_d("Send initialize message...");
  writeData(initializeMessage, sizeof(initializeMessage));

  auto data = readData();

  if (data == nullptr) { return false; }
  if (data->code != 0x02) { return false; }
  if (data->len != 0x04) { return false; }
  if (data->command != 0x00) { return false; }
  if (data->checksum != 0xFA) { return false; }

  return true;
}

CommandResult* Protocol::readData() {
  //    Wait minimum package from uart
  while (m_serial.available() < 4) {
    delay(1);
  }

  uint8_t responseConfirm = m_serial.read();
  uint8_t responseLength = m_serial.read();
  uint8_t responseCommand = m_serial.read();

  if (responseLength == 0) {
    log_d("The message is very small");
    return nullptr;
  }

  if (responseLength > m_bufferMaxSize) {
    log_d("The message is too big");
    return nullptr;
  }

  //    Create buffer for read
  auto data = new uint8_t[responseLength];
  data[0] = responseConfirm;
  data[1] = responseLength;
  data[2] = responseCommand;

  //    Wait full package from uart
  while (m_serial.available() < (responseLength - 3)) {
    delay(1);
  }

  //    Read package data from uart
  for (size_t i = 3; i < responseLength; i++) {
    uint8_t value = m_serial.read();
    data[i] = value;
  }

  uint8_t responseChecksum = data[responseLength - 1];
  uint8_t calculatedChecksum = calcChecksum(data, responseLength - 1);

  if (responseChecksum != calculatedChecksum) {
    log_d("The checksum does not match");
    return nullptr;
  }

  auto result = new CommandResult();
  result->code = responseConfirm;
  result->command = responseCommand;
  result->checksum = responseChecksum;
  result->data = data;
  result->len = responseLength;

  log_buf_d(data, responseLength);

  return result;
}

void Protocol::writeData(uint8_t const* data, size_t len) {
  m_serial.flush(false);
  m_serial.write(data, len);

  while (m_serial.available() < len) {
    delay(1);
  }

  for (size_t i = 0; i < len; i++) {
    uint8_t nextValue = m_serial.peek();
    if (nextValue == data[i]) {
      m_serial.read();
    }
  }

  log_buf_d(data, len);
}

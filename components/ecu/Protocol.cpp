//
// Created by jadjer on 24.08.22.
//

#include "Protocol.hpp"
#include "utils.hpp"
#include <esp_log.h>
#include <memory>

Protocol::Protocol() : Protocol(16, 17) {}

Protocol::Protocol(int8_t rxPin, int8_t txPin) : m_serial(2) {
  m_uartRx = rxPin;
  m_uartTx = txPin;
}

Protocol::~Protocol() = default;

esp_err_t Protocol::connect() {
  wakeup();
  return initialize();
}

std::optional<CommandResult> Protocol::readData() {
  if (not m_serial) { return std::nullopt; }

  // Wait minimum package from uart
  waitDataFromUart(4);

  uint8_t responseConfirm;
  uint8_t responseLength;
  uint8_t responseCommand;

  {
    uint8_t buffer[3];

    size_t len = m_serial.readBytes(buffer, 3);
    if (len != 3) {
      log_w("Error read data from uart");
      return std::nullopt;
    }

    responseConfirm = buffer[0];
    responseLength = buffer[1];
    responseCommand = buffer[2];
  }

  if (responseLength == 0) {
    log_w("The message is very small");
    return std::nullopt;
  }

  if (responseLength > SOC_UART_FIFO_LEN) {
    log_w("The message is too big");
    return std::nullopt;
  }

  //    Create buffer for read
  uint8_t data[responseLength];
//  auto data = new uint8_t[responseLength];

  data[0] = responseConfirm;
  data[1] = responseLength;
  data[2] = responseCommand;

  size_t responsePayloadLength = responseLength - 3;

  //    Wait full package from uart
  waitDataFromUart(responsePayloadLength);

  //    Read package data from uart
  {
    uint8_t buffer[responsePayloadLength];

    size_t len = m_serial.readBytes(buffer, responsePayloadLength);
    if (len != responsePayloadLength) {
      log_w("Error read data from uart");
      return std::nullopt;
    }

    for (size_t i = 0; i < responsePayloadLength; i++) {
      data[i + 3] = buffer[i];
    }
  }

  uint8_t responseChecksum = data[responseLength - 1];
  uint8_t calculatedChecksum = calcChecksum(data, responseLength - 1);
  if (responseChecksum != calculatedChecksum) {
    log_w("The checksum does not match");
    return std::nullopt;
  }

  CommandResult result{
      .code = responseConfirm,
      .command = responseCommand,
      .checksum = responseChecksum,
      .data = data,
      .len = responseLength,
  };

  log_buf_i(data, responseLength);

  return result;
}

void Protocol::writeData(uint8_t const* data, size_t len) {
  if (not m_serial) { return; }

  m_serial.flush(false);
  m_serial.write(data, len);

  //  Wait echo data from ECU
  waitDataFromUart(len);

  //  Echo data's delete from RX buffer
  uint8_t buffer[len];
  m_serial.readBytes(buffer, len);

  log_buf_i(data, len);
}

void Protocol::wakeup() const {
  pinMode(m_uartTx, INPUT_PULLUP);

  log_i("Wakeup ECU...");

  digitalWrite(m_uartTx, 0);
  delay(70);
  digitalWrite(m_uartTx, 1);
  delay(130);
}

esp_err_t Protocol::initialize() {
  uint8_t wakeupMessage[] = {0xFE, 0x04, 0xFF, 0xFF};
  uint8_t initializeMessage[] = {0x72, 0x05, 0x00, 0xF0, 0x99};

  m_serial.begin(10400, SERIAL_8N1, m_uartRx, m_uartTx);

  log_i("Send wakeup message...");
  writeData(wakeupMessage, sizeof(wakeupMessage));

  delay(20);

  log_i("Send initialize message...");
  writeData(initializeMessage, sizeof(initializeMessage));

  auto data = readData();

  if (data == std::nullopt) { return ESP_ERR_INVALID_RESPONSE; }
  if (data->code != 0x02) { return ESP_ERR_INVALID_RESPONSE; }
  if (data->len != 0x04) { return ESP_ERR_INVALID_SIZE; }
  if (data->command != 0x00) { return ESP_ERR_INVALID_ARG; }
  if (data->checksum != 0xFA) { return ESP_ERR_INVALID_CRC; }

  return ESP_OK;
}

void Protocol::waitDataFromUart(size_t len) {
  while (m_serial.available() < len) { delay(1); }
}

//
// Created by jadjer on 26.08.22.
//

#include "Connection.hpp"

#include "driver/uart.h"
#include "esp_log.h"

static char const* TAG = "Connection";

Connection::Connection() {
  m_rx = 16;
  m_tx = 17;
  m_buffSize = 1024;
  m_uartConfig = {
      .baud_rate = 10400,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  };

  esp_err_t ret;

  // We won't use a buffer for sending data.
  ret = uart_driver_install(UART_NUM_2, m_buffSize * 2, 0, 0, nullptr, 0);
  if (ret) {
    ESP_LOGE(TAG, "%s init uart failed: %s\n", __func__, esp_err_to_name(ret));
  }

  ret = uart_param_config(UART_NUM_2, &m_uartConfig);
  if (ret) {
    ESP_LOGE(TAG, "%s uart set config failed: %s\n", __func__, esp_err_to_name(ret));
  }

  ret = uart_set_pin(UART_NUM_2, m_tx, m_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  if (ret) {
    ESP_LOGE(TAG, "%s uart set pin failed: %s\n", __func__, esp_err_to_name(ret));
  }
}

std::vector<uint8_t> Connection::readData(std::string const& logName) {
  setPinConfig();

//  std::vector<uint8_t> data(m_buffSize);

  auto data = (uint8_t *) malloc(m_buffSize);

  auto rxBytes = uart_read_bytes(UART_NUM_2, &data, (m_buffSize - 1), portMAX_DELAY);

  ESP_LOGI(logName.c_str(), "Read %d bytes", rxBytes);

  return {};
}

int Connection::writeString(std::string const& logName, std::string const& data) {
  setPinConfig();

  size_t len = data.length();
  int txBytes = uart_write_bytes(UART_NUM_2, &data, len);

  ESP_LOGI(logName.c_str(), "Wrote %d bytes", txBytes);

  return txBytes;
}

int Connection::writeData(std::string const& logName, std::vector<uint8_t> const& data) {
  setPinConfig();

  size_t len = data.size();
  int txBytes = uart_write_bytes(UART_NUM_2, &data, len);

  ESP_LOGI(logName.c_str(), "Wrote %d bytes", txBytes);

  return txBytes;
}

bool Connection::setPinConfig() const {
  esp_err_t ret = uart_set_pin(UART_NUM_2, m_tx, m_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  if (ret) {
    ESP_LOGE(TAG, "%s uart set pin failed: %s\n", __func__, esp_err_to_name(ret));
    return false;
  }

  return true;
}

//
// Created by jadjer on 24.08.22.
//

#include "protocol.h"
#include "utils.h"
#include <driver/gpio.h>
#include <driver/uart.h>
#include <esp_log.h>

#define TAG "Protocol"
#define UART_RX 16
#define UART_TX 17
#define UART_PORT_NUM 2
#define UART_BUFF_SIZE 1024

void wakeup() {
  ESP_LOGI(TAG, "Wakeup ECU...");

  gpio_config_t io_conf = {
      .intr_type = GPIO_INTR_DISABLE,
      .mode = GPIO_MODE_OUTPUT,
      .pin_bit_mask = 1ULL << UART_TX,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
  };

  gpio_config(&io_conf);

  gpio_set_level(UART_TX, 0);
  vTaskDelay(70);

  gpio_set_level(UART_TX, 1);
  vTaskDelay(130);
}

esp_err_t initialize() {
  ESP_LOGI(TAG, "Initialize ECU...");

  uint8_t wakeupMessage[] = {0xFE, 0x04, 0xFF, 0xFF};
  uint8_t initializeMessage[] = {0x72, 0x05, 0x00, 0xF0, 0x99};

  uart_config_t uart_config = {
      .baud_rate = 10400,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_APB,
  };

  ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, UART_BUFF_SIZE, 0, 0, NULL, 0));
  ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, UART_TX, UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

  ESP_LOGI(TAG, "Send wakeup message...");
  writeData(wakeupMessage, sizeof(wakeupMessage));

  vTaskDelay(20);

  ESP_LOGI(TAG, "Send initialize message...");
  writeData(initializeMessage, sizeof(initializeMessage));

  command_result_t* data = readData();

  if (data == NULL) { return ESP_FAIL; }
  if (data->code != 0x02) { return ESP_FAIL; }
  if (data->len != 0x04) { return ESP_FAIL; }
  if (data->command != 0x00) { return ESP_FAIL; }
  if (data->checksum != 0xFA) { return ESP_FAIL; }

  return ESP_OK;
}

esp_err_t connect() {
  ESP_LOGI(TAG, "Connect to ECU:");

  wakeup();
  return initialize();
}

command_result_t* readData() {
  //    Wait minimum package from uart
  while (m_serial.available() < 4) {
    delay(1);
  }

  uint8_t responseConfirm = m_serial.read();
  uint8_t responseLength = m_serial.read();
  uint8_t responseCommand = m_serial.read();

  if (responseLength == 0) {
    ESP_LOGI(TAG, "The message is very small");
    return NULL;
  }

  if (responseLength > UART_BUFF_SIZE) {
    ESP_LOGI(TAG, "The message is too big");
    return NULL;
  }

  //    Create buffer for read
  uint8_t *data = calloc(responseLength, sizeof (uint8_t));
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
    ESP_LOGI(TAG, "The checksum does not match");
    return NULL;
  }

  command_result_t* result = calloc(1, sizeof(command_result_t));
  result->code = responseConfirm;
  result->command = responseCommand;
  result->checksum = responseChecksum;
  result->data = data;
  result->len = responseLength;

//  log_buf_d(data, responseLength);

  return result;
}

void writeData(uint8_t const* data, size_t len) {
  ESP_ERROR_CHECK(uart_flush(UART_PORT_NUM));

  uart_write_bytes(UART_PORT_NUM, data, len);

  while (m_serial.available() < len) {
    delay(1);
  }

  for (size_t i = 0; i < len; i++) {
    uint8_t nextValue = m_serial.peek();
    if (nextValue == data[i]) {
      m_serial.read();
    }
  }

//  log_buf_d(data, len);
}

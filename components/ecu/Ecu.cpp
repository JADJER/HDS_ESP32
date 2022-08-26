//
// Created by jadjer on 24.08.22.
//

#include "Ecu.hpp"
#include "driver/gpio.h"
#include "esp_log.h"
#include <thread>

static char const* TAG = "Ecu";

ECU::ECU(std::shared_ptr<IConnection> const& connection) {
  m_rx = 16;
  m_tx = 17;

  m_connection = connection;
}

ECU::~ECU() = default;

void ECU::init() const {
  ESP_LOGI(TAG, "Initialize ECU...");

  wakeup();

  //  Send FE 04 FF FF
  m_connection->writeData(TAG, {0xFE, 0x04, 0xFF, 0xFF});
  auto data1 = m_connection->readData(TAG);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  //  Send 72 05 00 F0 99
  m_connection->writeData(TAG, {0x72, 0x05, 0x00, 0xF0, 0x99});
  auto data2 = m_connection->readData(TAG);
}

void ECU::wakeup() const {
  ESP_LOGI(TAG, "Wakeup");

  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << m_tx),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };

  esp_err_t ret = gpio_config(&io_conf);
  if (ret) {
    ESP_LOGE(TAG, "%s init gpio failed: %s\n", __func__, esp_err_to_name(ret));
  }

  gpio_set_level(static_cast<gpio_num_t>(m_tx), 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(70));

  gpio_set_level(static_cast<gpio_num_t>(m_tx), 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(130));
}

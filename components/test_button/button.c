#include <sys/cdefs.h>
#include <sys/queue.h>
//
// Created by jadjer on 30.08.22.
//

#include "button.h"
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TAG "Button"
#define BUTTON_PIN 0

static buttonPushCallback_t m_callback = NULL;
static uint8_t m_enable = 0;

void IRAM_ATTR gpioIsrHandler() {
  if (!m_enable) {
    m_enable = 1;
  }
}

_Noreturn void gpioEventTask() {
  while (1) {
    if (m_callback && m_enable) {
      m_callback();
      m_enable = 0;
      vTaskDelay(1);
    }
  }
}

esp_err_t buttonInit() {
  gpio_config_t buttonConfig = {
      .intr_type = GPIO_INTR_NEGEDGE,
      .mode = GPIO_MODE_INPUT,
      .pin_bit_mask = (1ULL << BUTTON_PIN),
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
  };

  esp_err_t err;

  err = gpio_config(&buttonConfig);
  if (err) { return err; }

  xTaskCreate(gpioEventTask, "gpio_task_example", 2048, NULL, 10, NULL);

  gpio_install_isr_service(ESP_INTR_FLAG_EDGE);
  gpio_isr_handler_add(BUTTON_PIN, gpioIsrHandler, (void*) BUTTON_PIN);

  return ESP_OK;
}

void buttonSetInterruptCallback(buttonPushCallback_t callback) {
  m_callback = callback;
}

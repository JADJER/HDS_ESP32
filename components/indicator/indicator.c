#include <sys/cdefs.h>
//
// Created by jadjer on 30.08.22.
//

#include "indicator.h"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LED_PIN 2

static TaskHandle_t m_taskHandle = NULL;

static void blinkTask(void* arg) {
  int delay = (int) arg;
  uint8_t enable = 0;

  while (1) {
    enable = !enable;

    ESP_ERROR_CHECK(gpio_set_level(LED_PIN, enable));
    vTaskDelay(delay / portTICK_PERIOD_MS);
  }
}

esp_err_t indicatorInit() {
  gpio_config_t ledConfig = {
      .intr_type = GPIO_INTR_DISABLE,
      .mode = GPIO_MODE_OUTPUT,
      .pin_bit_mask = (1ULL << LED_PIN),
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
  };

  esp_err_t err;

  err = gpio_config(&ledConfig);
  if (err) { return err; }

  return ESP_OK;
}

esp_err_t indicatorEnable() {
  if (m_taskHandle != NULL) {
    vTaskDelete(m_taskHandle);
  }

  esp_err_t err = gpio_set_level(LED_PIN, 0);

  return err;
}

esp_err_t indicatorDisable() {
  if (m_taskHandle != NULL) {
    vTaskDelete(m_taskHandle);
  }

  esp_err_t err = gpio_set_level(LED_PIN, 0);

  return err;
}

void indicatorBlink(int delayMs) {
  if (m_taskHandle != NULL) {
    vTaskDelete(m_taskHandle);
  }

  xTaskCreate(blinkTask, "pin_blink_task", 1024, (void*) delayMs, configMAX_PRIORITIES, &m_taskHandle);
  configASSERT(m_taskHandle);
}

//
// Created by jadjer on 30.08.22.
//

#include "Button.hpp"
#include <Arduino.h>

#define TAG "Button"

static bool buttonIsPressed = false;

void ARDUINO_ISR_ATTR isr() {
  buttonIsPressed = true;
}

Button::Button() {
  m_button = 0;
  m_isInitialised = false;
}

Button::~Button() {
  if (not m_isInitialised) { return; }

  detachInterrupt(m_button);
}

esp_err_t Button::init() {
  if (m_isInitialised) { return ESP_OK; }
  m_isInitialised = true;

  pinMode(m_button, INPUT_PULLUP);
  attachInterrupt(m_button, isr, FALLING);

  return ESP_OK;
}

bool Button::isPressed() {
  return buttonIsPressed;
}

void Button::resetState() {
  buttonIsPressed = false;
}

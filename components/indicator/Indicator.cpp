//
// Created by jadjer on 30.08.22.
//

#include "Indicator.hpp"

#include <Arduino.h>

Indicator::Indicator(int pin) : m_enable(false) {
  m_led = pin;

  pinMode(m_led, OUTPUT);
}

Indicator::~Indicator() {
  m_enable = false;
  m_thread.join();
}

void Indicator::enable(bool enable) {
  m_enable = false;
  m_thread.join();

  if (enable) {
    digitalWrite(m_led, 1);
  } else {
    digitalWrite(m_led, 0);
  }
}

void Indicator::blink(int delayMs) {
  m_enable = true;

  m_thread = std::thread([=]{
    while (m_enable) {
      digitalWrite(m_led, 1);
      delay(delayMs);
      digitalWrite(m_led, 0);
      delay(delayMs);
    }
  });
}

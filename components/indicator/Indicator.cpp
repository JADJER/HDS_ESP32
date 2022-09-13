//
// Created by jadjer on 30.08.22.
//

#include "Indicator.hpp"
#include <esp32-hal-gpio.h>

Indicator::Indicator(int pin) : m_enable(false) {
  m_led = pin;
  m_delay = 100;

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
  m_delay = delayMs;

  if (m_enable) { return; }

  m_enable = true;
  m_thread = std::thread([=] {
    while (m_enable) {
      digitalWrite(m_led, 1);
      delay(m_delay);

      digitalWrite(m_led, 0);
      delay(m_delay);
    }
  });
}

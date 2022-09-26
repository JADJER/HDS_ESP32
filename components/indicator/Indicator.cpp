// Copyright 2022 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Indicator.hpp"

#include <Arduino.h>

Indicator::Indicator(int pinNum) {
  m_pinNum = pinNum;
  m_threadEnable = false;

  pinMode(m_pinNum, OUTPUT);
}

Indicator::~Indicator() {
  m_threadEnable = false;
  if (m_thread.joinable()) { m_thread.join(); }
}

void Indicator::enable() {
  m_threadEnable = false;
  if (m_thread.joinable()) { m_thread.join(); }

  digitalWrite(m_pinNum, 1);
}

void Indicator::disable() {
  m_threadEnable = false;
  if (m_thread.joinable()) { m_thread.join(); }

  digitalWrite(m_pinNum, 0);
}

void Indicator::blink(int delayMs) {
  m_threadEnable = false;
  if (m_thread.joinable()) { m_thread.join(); }

  m_threadEnable = true;
  m_thread = std::thread(&Indicator::blinkTask, this, delayMs);
}

void Indicator::blinkErrorCode(int errorCode) {
  m_threadEnable = false;
  if (m_thread.joinable()) { m_thread.join(); }

  m_threadEnable = true;
  m_thread = std::thread(&Indicator::errorBlinkTask, this, errorCode);
}

void Indicator::blinkTask(int delayMs) const {
  while (m_threadEnable) {
    digitalWrite(m_pinNum, 1);
    delay(delayMs);
    digitalWrite(m_pinNum, 0);
    delay(delayMs);
  }
}

void Indicator::errorBlinkTask(int errorCode) const {
  int firstDigit = errorCode / 10;
  int secondDigit = errorCode % 10;

  while (m_threadEnable) {
    for (size_t i = 0; i < firstDigit; i++) {
      digitalWrite(m_pinNum, 1);
      delay(500);
      digitalWrite(m_pinNum, 0);
      delay(500);
    }

    delay(500);

    for (size_t i = 0; i < secondDigit; i++) {
      digitalWrite(m_pinNum, 1);
      delay(250);
      digitalWrite(m_pinNum, 0);
      delay(250);
    }

    delay(3000);
  }
}

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

//
// Created by jadjer on 26.09.22.
//

#include "ErrorCodeIndicator.hpp"

#include <Arduino.h>

ErrorCodeIndicator::ErrorCodeIndicator(int pinNum) : Indicator(pinNum) {}

ErrorCodeIndicator::~ErrorCodeIndicator() = default;

void ErrorCodeIndicator::enable() {}

void ErrorCodeIndicator::disable() {}

void ErrorCodeIndicator::blinkTask() {
  while (m_threadEnable) {
    int firstDigit = m_taskValue / 10;

    for (size_t i = 0; i < firstDigit; i++) {
      digitalWrite(m_pinNum, 1);
      delay(500);

      digitalWrite(m_pinNum, 0);
      delay(500);
    }
    delay(500);

    int secondDigit = m_taskValue % 10;

    for (size_t i = 0; i < secondDigit; i++) {
      digitalWrite(m_pinNum, 1);
      delay(250);

      digitalWrite(m_pinNum, 0);
      delay(250);
    }
    delay(3000);
  }
}

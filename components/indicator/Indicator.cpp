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
  m_delay = 500;
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

  digitalWrite(m_pinNum, HIGH);
}

void Indicator::disable() {
  m_threadEnable = false;
  if (m_thread.joinable()) { m_thread.join(); }

  digitalWrite(m_pinNum, LOW);
}

void Indicator::blink(int delayMs) {
  m_delay = delayMs;

  if (m_threadEnable) { return; }

  m_thread = std::thread(&Indicator::blinkTask, this, delayMs);
}

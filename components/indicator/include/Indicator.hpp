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
// Created by jadjer on 23.09.22.
//

#pragma once

#include <esp_err.h>
#include <thread>

/**
 * @brief
 */
class Indicator {
 public:
  explicit Indicator(int pinNum);
  ~Indicator();

 public:
  /**
   * @brief
   */
  void enable();

  /**
   * @brief
   */
  void disable();

  /**
   * @brief
   * @param delayMs
   */
  void blink(int delayMs);

  /**
   * @brief
   * @param errorCode
   */
  void blinkErrorCode(int errorCode);

 protected:
  int m_pinNum;
  std::thread m_thread;
  bool m_threadEnable;

 private:
  void blinkTask(int delayMs) const;
  void errorBlinkTask(int errorCode) const;
};

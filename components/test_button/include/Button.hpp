//
// Created by jadjer on 30.08.22.
//

#pragma once

#include <Arduino.h>

/**
 * @brief
 */
class Button {
 public:
  Button();
  explicit Button(int8_t buttonPin);
  ~Button();

 public:
  /**
   * @brief
   * @return
   */
  static bool isPressed();

  /**
   * @brief
   */
  static void resetState();

 private:
  int8_t m_buttonPin;
};

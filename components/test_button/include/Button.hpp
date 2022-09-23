//
// Created by jadjer on 30.08.22.
//

#pragma once

#include <esp_err.h>

/**
 * @brief
 */
class Button {
 public:
  Button();
  ~Button();

 public:
  /**
   * @brief
   * @return
   */
  [[nodiscard]] esp_err_t init();

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
  int8_t m_button;
  bool m_isInitialised;
};

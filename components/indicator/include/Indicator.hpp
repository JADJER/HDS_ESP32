//
// Created by jadjer on 30.08.22.
//

#pragma once

#include <atomic>
#include <thread>

class Indicator {
 public:
  explicit Indicator(int pin);
  ~Indicator();

 public:
  void enable(bool enable = true);
  void blink(int delayMs);

 private:
  int m_led;
  std::thread m_thread;
  std::atomic<bool> m_enable;
};

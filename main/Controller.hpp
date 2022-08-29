//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "Ble.hpp"
#include "Ecu.hpp"
#include "Indicator.hpp"

class Controller {
 public:
  Controller();
  ~Controller();

 public:
  [[noreturn]] void spin();

 private:
  BLE m_ble;
  ECU m_ecu;
  Indicator m_indicator;

 private:
  void spinOnce();
};

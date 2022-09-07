//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "Ble.hpp"
#include "Communication.hpp"
#include "Ecu.hpp"
#include "Indicator.hpp"

class Controller {
 public:
  Controller();
  ~Controller();

 public:
  [[noreturn]] void spin();

 private:
  bool m_ecuConnected;
  BLE m_ble;
  Communication m_communication;
  Ecu m_ecu;
  Indicator m_indicator;

 private:
  void spinOnce();
};

//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "Ble.hpp"
#include "Ecu.hpp"
#include "Engine.hpp"
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
  ECU m_ecu;
  Engine m_engine;
  Indicator m_indicator;

 private:
  void spinOnce();
};

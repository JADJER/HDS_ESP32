//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "Bluetooth.hpp"
#include "Button.hpp"
#include "ECU.hpp"
#include "Indicator.hpp"
#include "Protocol.hpp"
#include <esp_err.h>

class Controller {
 public:
  Controller();
  ~Controller();

 public:
  [[noreturn]] void spin();

 private:
  Indicator* m_indicator;
  Button m_button;
  Protocol m_protocol;
  ECU m_ecu;
  BluetoothServer* m_bluetoothServer;

 private:
  void spinOnce();
};

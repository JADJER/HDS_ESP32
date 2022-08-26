//
// Created by jadjer on 24.08.22.
//

#include "ConnectionFactory.hpp"
#include "Ecu.hpp"
#include <esp_system.h>
#include <iostream>

extern "C" void app_main(void) {
  auto connection = ConnectionFactory::createConnection();

  ECU ecu(connection);
  ecu.init();
}

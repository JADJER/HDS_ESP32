//
// Created by jadjer on 24.08.22.
//

#include "Ecu.hpp"
#include <iostream>

extern "C" void app_main(void) {
  ECU ecu;

  bool isConnected = ecu.connect();
  if (isConnected) {
    std::cout << "ECU is connected" << std::endl;
    ecu.test();

  } else {
    std::cout << "ECU connection failed" << std::endl;
  }
}

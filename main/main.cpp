//
// Created by jadjer on 24.08.22.
//

#include "Controller.hpp"
#include <Arduino.h>

extern "C" void app_main(void) {
  initArduino();

  Controller controller;
  controller.spin();
}

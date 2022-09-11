//
// Created by jadjer on 09.09.22.
//

#pragma once

#include <string>
#include <esp_err.h>

class Bluetooth {
 public:
  Bluetooth();
  ~Bluetooth();

 public:
  esp_err_t init();

 private:
  std::string m_tag;
};

//
// Created by jadjer on 08.09.22.
//

#pragma once

#include <esp_err.h>

class Mesh {
 public:
  Mesh();
  ~Mesh();

 public:
  esp_err_t init();
};

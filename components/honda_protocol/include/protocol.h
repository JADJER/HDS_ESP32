//
// Created by jadjer on 24.08.22.
//

#pragma once

#include "command_result.h"
#include <esp_err.h>

//Protocol(int8_t rx, int8_t tx);

esp_err_t connect();
command_result_t* readData();
void writeData(uint8_t const* data, size_t len);

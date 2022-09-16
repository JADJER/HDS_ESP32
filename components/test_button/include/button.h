//
// Created by jadjer on 30.08.22.
//

#pragma once

#include <esp_err.h>

typedef void (*buttonPushCallback_t)();

/**
 * @brief
 * @return
 */
esp_err_t buttonInit();

/**
 * @brief
 * @param callback
 */
void buttonSetInterruptCallback(buttonPushCallback_t callback);

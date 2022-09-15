//
// Created by jadjer on 30.08.22.
//

#pragma once

#include <esp_err.h>

/**
 * @brief
 * @return
 */
esp_err_t indicatorInit();

/**
 * @brief
 * @param enable
 */
esp_err_t indicatorEnable();

/**
 * @brief
 */
esp_err_t indicatorDisable();

/**
 * @brief
 * @param delayMs
 */
void indicatorBlink(int delayMs);

/**
 * @brief
 * @param code
 */
void indicatorBlinkErrorCode(uint8_t code);

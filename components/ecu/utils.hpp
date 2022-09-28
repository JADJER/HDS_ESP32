//
// Created by jadjer on 24.08.22.
//

#pragma once

#include <Arduino.h>

/**
 * @brief
 * @param val
 * @return
 */
float calcValueDivide256(int val);

/**
 * @brief
 * @param val
 * @return
 */
int calcValueMinus40(int val);

/**
 * @brief
 * @param val
 * @return
 */
float calcValueDivide16(int val);

/**
 * @brief
 * @param val
 * @return
 */
float calcValueDivide10(int val);

/**
 * @brief
 * @param data
 * @param len
 * @return
 */
uint16_t calcChecksum(uint8_t const* data, size_t len);

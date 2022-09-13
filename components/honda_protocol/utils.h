//
// Created by jadjer on 24.08.22.
//

#pragma once

#include <stdlib.h>

/**
 * @brief Calculate checksum
 * @param data Data array
 * @param len Array len
 * @return checksum
 */
uint16_t calcChecksum(uint8_t const* data, size_t len);

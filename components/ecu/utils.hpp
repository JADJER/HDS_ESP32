//
// Created by jadjer on 24.08.22.
//

#pragma once

float calcValueDivide256(int val);
float calcValueMinus40(int val);
float calcValueDivide10(int val);
float calcValueDivide16(int val);
uint8_t calcChecksum(uint8_t const* data, size_t start, size_t stop);
uint8_t calcChecksum(uint8_t const* data, size_t len);
float calcKPHtoMPH(int val);

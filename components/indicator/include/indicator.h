//
// Created by jadjer on 30.08.22.
//

#pragma once

typedef enum {
  LED_ENABLE,
  LED_DISABLE
} LedState;

void enable(LedState enable);
void blink(int delayMs);

//
// Created by jadjer on 30.08.22.
//

#pragma once

typedef enum {
  LED_ENABLE,
  LED_DISABLE
} led_state;

void enable(led_state enable);
void blink(int delayMs);

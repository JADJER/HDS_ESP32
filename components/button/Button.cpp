//
// Created by jadjer on 30.08.22.
//

#include "Button.hpp"

static bool buttonIsPressed = false;

void ARDUINO_ISR_ATTR isr() {
  if (not buttonIsPressed) {
    buttonIsPressed = true;
  }
}

Button::Button(int8_t buttonPin) {
  m_buttonPin = buttonPin;

  pinMode(m_buttonPin, INPUT_PULLUP);
  attachInterrupt(m_buttonPin, isr, FALLING);
}

Button::~Button() {
  detachInterrupt(m_buttonPin);
}

bool Button::isPressed() const {
  return buttonIsPressed;
}

void Button::resetState() {
  buttonIsPressed = false;
}

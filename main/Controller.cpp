//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include <Arduino.h>
#include <iostream>

Controller::Controller() : m_ble("HDS"), m_ecu(16, 17), m_indicator(2) {
  m_ble.createService("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30");
  m_ble.createCharacteristic("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30", "96902795-9837-4345-949f-706cdb7b76f6");
  m_ble.createCharacteristic("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30", "ce914ca7-5b8b-4133-af2f-27a15f7c7d89");
  m_ble.createCharacteristic("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30", "a4c6be92-0060-4702-a63a-5d7eb122eac8");
  m_ble.createCharacteristic("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30", "2b2ee537-9bb5-4cd3-9e44-72122f1c82ca");
  m_ble.start();
  m_ble.advertising();

  m_indicator.blink(500);

  bool isConnected = m_ecu.connect();
  if (isConnected) {
    std::cout << "ECU is connected" << std::endl;
    m_indicator.enable(true);
    m_ecu.test();

  } else {
    std::cout << "ECU connection failed" << std::endl;
    m_indicator.enable(false);
  }
}

Controller::~Controller() = default;

[[noreturn]] void Controller::spin() {
  while (true) {
    spinOnce();
    delay(100);
  }
}

void Controller::spinOnce() {
  m_ble.setValueUInt16("96902795-9837-4345-949f-706cdb7b76f6", m_ecu.getTest());
  m_ble.setValueFloat("ce914ca7-5b8b-4133-af2f-27a15f7c7d89", 12.45);
  m_ble.setValueInt("a4c6be92-0060-4702-a63a-5d7eb122eac8", 1);
  m_ble.setValueString("2b2ee537-9bb5-4cd3-9e44-72122f1c82ca", "Test");
}

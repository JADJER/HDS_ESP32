//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include <Arduino.h>
#include <iostream>

Controller::Controller() : m_ble("HDS"), m_ecu(16, 17), m_indicator(2) {
  m_ble.createService("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30"); //BASE
  m_ble.createCharacteristic("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30", "96902795-9837-4345-949f-706cdb7b76f6"); //RPPM
  m_ble.createCharacteristic("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30", "ce914ca7-5b8b-4133-af2f-27a15f7c7d89"); //SPEED
  m_ble.createCharacteristic("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30", "41ea8aa3-9541-4c36-949e-45d63a0da48b"); //BATTERY

  m_ble.createService("2b3c1105-ff38-4963-b5af-c2631372f9ad"); //TPS
  m_ble.createCharacteristic("2b3c1105-ff38-4963-b5af-c2631372f9ad", "a4c6be92-0060-4702-a63a-5d7eb122eac8"); //TPS_PERCENT
  m_ble.createCharacteristic("2b3c1105-ff38-4963-b5af-c2631372f9ad", "2b2ee537-9bb5-4cd3-9e44-72122f1c82ca"); //TPS_VOLTAGE

  m_ble.createService("7cdd9e93-844f-4313-b500-91e8a374c8a7"); //ECT
  m_ble.createCharacteristic("7cdd9e93-844f-4313-b500-91e8a374c8a7", "ddbea267-80cb-4bc3-992a-159bb45b32d2"); //ECT_TEMP
  m_ble.createCharacteristic("7cdd9e93-844f-4313-b500-91e8a374c8a7", "3bf76097-4b24-428a-81a2-bb2f073e273e"); //ECT_VOLTAGE

  m_ble.createService("1311712d-1ef6-4161-be8e-6b60aab4c643"); //IAT
  m_ble.createCharacteristic("1311712d-1ef6-4161-be8e-6b60aab4c643", "6b9679b7-3fc4-4f30-ae75-df2f2179dfca"); //IAT_TEMP
  m_ble.createCharacteristic("1311712d-1ef6-4161-be8e-6b60aab4c643", "63c6853f-1d91-49eb-bbe5-ec4a05174bdc"); //IAT_VOLTAGE

  m_ble.createService("5ed5a50d-7b41-4bd5-a937-dbd58b5e469f"); //MAP
  m_ble.createCharacteristic("5ed5a50d-7b41-4bd5-a937-dbd58b5e469f", "96f9b873-6c05-42d6-8245-06abffe4abfb"); //MAP_PRESSURE
  m_ble.createCharacteristic("5ed5a50d-7b41-4bd5-a937-dbd58b5e469f", "b7df0786-d4c3-43d9-a1ff-ba98af91ce52"); //MAP_VOLTAGE

  m_ble.createService("a5a87665-d39f-4460-8643-8e74081f6c7d"); //STATES
  m_ble.createCharacteristic("a5a87665-d39f-4460-8643-8e74081f6c7d", "7f6b5ec6-598e-4ad1-a19b-efea24c76906"); //ENGINE_STATE
  m_ble.createCharacteristic("a5a87665-d39f-4460-8643-8e74081f6c7d", "175e35aa-cb02-4440-b7be-7b92467c169d"); //SIDE_STATE

  m_ble.start();
  m_ble.advertising();

  m_indicator.blink(500);

  bool isConnected = m_ecu.connect();
  if (isConnected) {
    std::cout << "ECU is connected" << std::endl;
  } else {
    std::cout << "ECU connection failed" << std::endl;
  }

  m_indicator.enable(isConnected);
}

Controller::~Controller() = default;

[[noreturn]] void Controller::spin() {
  while (true) {
    spinOnce();
    delay(100);
  }
}

void Controller::spinOnce() {
  m_ecu.spinOnce();

  m_ble.setValueUInt16("96902795-9837-4345-949f-706cdb7b76f6", m_ecu.getRpm());
  m_ble.setValueUInt16("ce914ca7-5b8b-4133-af2f-27a15f7c7d89", m_ecu.getSpeed());
  m_ble.setValueFloat("41ea8aa3-9541-4c36-949e-45d63a0da48b", m_ecu.getBatteryVolts());

  m_ble.setValueUInt16("a4c6be92-0060-4702-a63a-5d7eb122eac8", m_ecu.getTpsPercent());
  m_ble.setValueUInt16("2b2ee537-9bb5-4cd3-9e44-72122f1c82ca", m_ecu.getTpsVolts());

  m_ble.setValueFloat("ddbea267-80cb-4bc3-992a-159bb45b32d2", m_ecu.getEctTemp());
  m_ble.setValueUInt16("3bf76097-4b24-428a-81a2-bb2f073e273e", m_ecu.getEctVolts());

  m_ble.setValueFloat("6b9679b7-3fc4-4f30-ae75-df2f2179dfca", m_ecu.getIatTemp());
  m_ble.setValueUInt16("63c6853f-1d91-49eb-bbe5-ec4a05174bdc", m_ecu.getIatVolts());

  m_ble.setValueFloat("96f9b873-6c05-42d6-8245-06abffe4abfb", m_ecu.getMapPressure());
  m_ble.setValueUInt16("b7df0786-d4c3-43d9-a1ff-ba98af91ce52", m_ecu.getMapVolts());

  m_ble.setValueUInt16("7f6b5ec6-598e-4ad1-a19b-efea24c76906", m_ecu.isStartedEngine());
  m_ble.setValueUInt16("175e35aa-cb02-4440-b7be-7b92467c169d", m_ecu.isClutchOff());
}
